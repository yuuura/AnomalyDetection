#ifndef UNICODE
#define UNICODE
#endif

#include <windows.h>
#include "FileHandle.h"
#include "Advapi32.h"
#include "Kernel32.h"
#include "Ws2_32.h"
#include "hook.h"
#include "Disassembler\hde32.h"

#define NO_INLINE_ASM
#define BUFFER_SIZE 512

LPVOID OriginalMemArea;

HOOK_ARRAY HookArray[] =
{	
	// Files
	{ "kernel32.dll", "CreateFileA", (LPVOID)&NewCreateFileA, &OldCreateFileA, 0 },
	{ "kernel32.dll", "CreateFileW", (LPVOID)&NewCreateFileW, &OldCreateFileW, 0 },
	{ "kernel32.dll", "WriteFile", (LPVOID)&NewWriteFile, &OldWriteFile, 0 },
	{ "kernel32.dll", "ReadFile", (LPVOID)&NewReadFile, &OldReadFile, 0 },
	{ "kernel32.dll", "CloseHandle", (LPVOID)&NewCloseHandle, &OldCloseHandle, 0 },
	{ "kernel32.dll", "FindFirstFileA", (LPVOID)&NewFindFirstFileA, &OldFindFirstFileA, 0, },
	{ "kernel32.dll", "FindNextFileA", (LPVOID)&NewFindNextFileA, &OldFindNextFileA, 0, },
	{ "kernel32.dll", "FindFirstFileW", (LPVOID)&NewFindFirstFileW, &OldFindFirstFileW, 0, },
	{ "kernel32.dll", "FindNextFileW", (LPVOID)&NewFindNextFileW, &OldFindNextFileW, 0, },
	{ "kernel32.dll", "CreateProcessA", (LPVOID)&NewCreateProcessA, &OldCreateProcessA, 0, },
	{ "kernel32.dll", "CreateProcessW", (LPVOID)&NewCreateProcessW, &OldCreateProcessW, 0, },

	// Winsock communication 
	{ "ws2_32.dll", "WSAStartup", (LPVOID)&NewWSAStartup, &OldWSAStartup, 0, },
	{ "ws2_32.dll", "socket", (LPVOID)&NewSocket, &OldSocket, 0, },
	{ "ws2_32.dll", "connect", (LPVOID)&NewConnect, &OldConnect, 0, },
	{ "ws2_32.dll", "inet_addr", (LPVOID)&NewInet_addr, &OldInet_addr, 0, },
	{ "ws2_32.dll", "htons", (LPVOID)&NewHtons, &OldHtons, 0, },
	
	// Cryptography & service
	{ "Advapi32.dll", "CryptAcquireContextA", (LPVOID)&NewCryptAcquireContextA, &OldCryptAcquireContextA, 0 },
	{ "Advapi32.dll", "CryptAcquireContextW", (LPVOID)&NewCryptAcquireContextW, &OldCryptAcquireContextW, 0 },
	{ "Advapi32.dll", "CryptHashData", (LPVOID)&NewCryptHashData, &OldCryptHashData, 0 },
	{ "Advapi32.dll", "CryptEncrypt", (LPVOID)&NewCryptEncrypt, &OldCryptEncrypt, 0 },
	{ "Advapi32.dll", "OpenSCManagerA", (LPVOID)&NewOpenSCManagerA, &OldOpenSCManagerA, 0 },
	{ "Advapi32.dll", "OpenSCManagerW", (LPVOID)&NewOpenSCManagerW, &OldOpenSCManagerW, 0 },
	{ "Advapi32.dll", "OpenServiceA", (LPVOID)&NewOpenServiceA, &OldOpenServiceA, 0 },
	{ "Advapi32.dll", "OpenServiceW", (LPVOID)&NewOpenServiceW, &OldOpenServiceW, 0 },
	{ "Advapi32.dll", "ControlService", (LPVOID)&NewControlService, &OldControlService, 0 },
};

void HookAll()
{
	int i, NumEntries = sizeof(HookArray) / sizeof(HOOK_ARRAY);

	//Needs 25 bytes for each hooked function to hold original byte + return jump
	OriginalMemArea = VirtualAlloc(NULL, 25 * NumEntries, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if(!OriginalMemArea)
		return;

	for (i = 0; i < NumEntries; i++)
	{
		//Split the allocated memory into a block of 25 bytes for each hooked function
		*(LPVOID *)HookArray[i].original = (LPVOID)((DWORD)OriginalMemArea + (i * 25));
		HookFunction(HookArray[i].dll, HookArray[i].name, HookArray[i].proxy, *(LPVOID *)HookArray[i].original, &HookArray[i].length);
	}
}

//We need to copy 5 bytes, but we can only do 2, 4, 8 atomically
//Pad buffer to 8 bytes then use lock cmpxchg8b instruction
void SafeMemcpyPadded(LPVOID destination, LPVOID source, DWORD size)
{
	BYTE SourceBuffer[8];
	if(size > 8)
		return;

	//Pad the source buffer with bytes from destination
	memcpy(SourceBuffer, destination, 8);
	memcpy(SourceBuffer, source, size);

	_InterlockedCompareExchange64((LONGLONG *)destination, *(LONGLONG *)SourceBuffer, *(LONGLONG *)destination);
}

BOOL HookFunction(CHAR *dll, CHAR *name, LPVOID proxy, LPVOID original, PDWORD length)
{
	LPVOID FunctionAddress;
	DWORD TrampolineLength = 0, OriginalProtection;
	hde32s disam;
	BYTE Jump[5] = {0xE9, 0x00, 0x00, 0x00, 0x00};

	FunctionAddress = GetProcAddress(GetModuleHandleA(dll), name);
	
	if(!FunctionAddress)
		return FALSE;

	//disassemble length of each instruction, until we have 5 or more bytes worth
	while(TrampolineLength < 5)
	{
		LPVOID InstPointer = (LPVOID)((DWORD)FunctionAddress + TrampolineLength);
		TrampolineLength += hde32_disasm(InstPointer, &disam);
	}

	//Build the trampoline buffer
	memcpy(original, FunctionAddress, TrampolineLength);
	*(DWORD *)(Jump+1) = ((DWORD)FunctionAddress + TrampolineLength) - ((DWORD)original + TrampolineLength + 5);
	memcpy((LPVOID)((DWORD)original+TrampolineLength), Jump, 5);

	//Make sure the function is writable
	if(!VirtualProtect(FunctionAddress, TrampolineLength, PAGE_EXECUTE_READWRITE, &OriginalProtection))
		return FALSE;

	//Build and atomically write the hook
	*(DWORD *)(Jump+1) = (DWORD)proxy - (DWORD)FunctionAddress - 5;
	SafeMemcpyPadded(FunctionAddress, Jump, 5);

	//Restore the original page protection
	VirtualProtect(FunctionAddress, TrampolineLength, OriginalProtection, &OriginalProtection);

	//Clear CPU instruction cache
	FlushInstructionCache(GetCurrentProcess(), FunctionAddress, TrampolineLength);

	*length = TrampolineLength;

	return TRUE;
}

BOOL OpenConnection()
{
	//Winsock Startup
	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 1);
	if (WSAStartup(DllVersion, &wsaData) != 0) //If WSAStartup returns anything other than 0, then that means an error has occured in the WinSock Startup.
	{
		MessageBoxA(NULL, "Winsock startup failed", "Winsock Error", MB_OK | MB_ICONERROR);
		return FALSE;
	}

	SOCKADDR_IN addr; //Address to be binded to our Connection socket
	int sizeofaddr = sizeof(addr); //Need sizeofaddr for the connect function
	addr.sin_addr.s_addr = inet_addr(ip); //Address = localhost (this pc)
	addr.sin_port = htons(port); //Port = 35000
	addr.sin_family = AF_INET; //IPv4 Socket

	Connection = socket(AF_INET, SOCK_STREAM, NULL); //Set Connection socket
	if (connect(Connection, (SOCKADDR*)&addr, sizeofaddr) != 0) //If we are unable to connect...
	{
		MessageBoxA(NULL, "Failed to open connect with server. Run \"Anomaly Detection\" program first.", "Connection Error", MB_OK | MB_ICONERROR);
		return FALSE;
	}
	return TRUE;
}

void CloseConnection()
{
	closesocket(Connection);
}

extern "C" __declspec(dllexport)
BOOL WINAPI DllMain(HINSTANCE hDll, DWORD dwReason, LPVOID lpReserved)
{
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		FileHandleInitialize();
		if(OpenConnection())
			HookAll();
		else exit(0);
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		CloseConnection();
	}
	return TRUE;
}