#ifndef UNICODE
#define UNICODE
#endif

// link with Ws2_32.lib
#pragma comment(lib,"ws2_32.lib")

#include <windows.h>
#include <stdio.h>
#include <intrin.h>
#include <fstream>
#include <iostream>
#include <string>
#include "atlbase.h"
#include <atlstr.h>
 
#include "hook.h"
#include "Disassembler\hde32.h"
#include "Disassembler\hde64.h"
using namespace std;
// use _InterlockedCompareExchange64 instead of inline ASM (depends on compiler)
#define NO_INLINE_ASM

TdefOldMessageBoxA OldMessageBoxA;
TdefOldMessageBoxW OldMessageBoxW;

TdefOldCreateFileA OldCreateFileA;
TdefOldCreateFileW OldCreateFileW;
TdefOldWriteFile OldWriteFile;
TdefOldReadFile OldReadFile;

// Winsock communication
TdefOldWSAStartup OldWSAStartup;
TdefOldSocket OldSocket;
TdefOldConnect OldConnect;
//TdefOldSend OldSend;
TdefOldRecv OldRecv;
TdefOldInet_addr OldInet_addr;
TdefOldHtons OldHtons;

// Seach Files
TdefOldFindFirstFileA OldFindFirstFileA;	//Multi-Char
TdefOldFindNextFileA OldFindNextFileA;		//Multi-Char
TdefOldFindFirstFileW OldFindFirstFileW;	//Unicode
TdefOldFindNextFileW OldFindNextFileW;		//Unicode

// Cryptography
TdefOldCryptAcquireContextA OldCryptAcquireContextA;
TdefOldCryptAcquireContextW OldCryptAcquireContextW;
//TdefOldCryptGenKey OldCryptGenKey;
//TdefOldCryptGetUserKey OldCryptGetUserKey;
//TdefOldCryptExportKey OldCryptExportKey;
//TdefOldCryptCreateHash OldCryptCreateHash;
TdefOldCryptHashData OldCryptHashData;
//TdefOldCryptDeriveKey OldCryptDeriveKey;
//TdefOldCryptEncrypt OldCryptEncrypt;

LPVOID OriginalMemArea;
CreateFileHANDLE CreateFileHandle;

HOOK_ARRAY HookArray[] =
{
	{ "user32.dll", "MessageBoxA", (LPVOID)&NewMessageBoxA, &OldMessageBoxA, 0 },
	{ "user32.dll", "MessageBoxW", (LPVOID)&NewMessageBoxW, &OldMessageBoxW, 0 },
	{ "kernel32.dll", "CreateFileA", (LPVOID)&NewCreateFileA, &OldCreateFileA, 0 },
	{ "kernel32.dll", "CreateFileW", (LPVOID)&NewCreateFileW, &OldCreateFileW, 0 },
	{ "kernel32.dll", "WriteFile", (LPVOID)&NewWriteFile, &OldWriteFile, 0 },
	{ "kernel32.dll", "ReadFile", (LPVOID)&NewReadFile, &OldReadFile, 0 },
	
	// Winsock communication 
	{ "ws2_32.dll", "WSAStartup", (LPVOID)&NewWSAStartup, &OldWSAStartup, 0, },
	{ "ws2_32.dll", "socket", (LPVOID)&NewSocket, &OldSocket, 0, },
	{ "ws2_32.dll", "connect", (LPVOID)&NewConnect, &OldConnect, 0, },
	//{ "ws2_32.dll", "send", (LPVOID)&NewSend, &OldSend, 0, },
	{ "ws2_32.dll", "recv", (LPVOID)&NewRecv, &OldRecv, 0, },
	{ "ws2_32.dll", "inet_addr", (LPVOID)&NewInet_addr, &OldInet_addr, 0, },
	{ "ws2_32.dll", "htons", (LPVOID)&NewHtons, &OldHtons, 0, },

	// Search Files
	{ "kernel32.dll", "FindFirstFileA", (LPVOID)&NewFindFirstFileA, &OldFindFirstFileA, 0, },
	{ "kernel32.dll", "FindNextFileA", (LPVOID)&NewFindNextFileA, &OldFindNextFileA, 0, },
	{ "kernel32.dll", "FindFirstFileW", (LPVOID)&NewFindFirstFileW, &OldFindFirstFileW, 0, },
	{ "kernel32.dll", "FindNextFileW", (LPVOID)&NewFindNextFileW, &OldFindNextFileW, 0, },

	// Cryptography
	{ "Advapi32.dll", "CryptAcquireContextA", (LPVOID)&NewCryptAcquireContextA, &OldCryptAcquireContextA, 0 },
	{ "Advapi32.dll", "CryptAcquireContextW", (LPVOID)&NewCryptAcquireContextW, &OldCryptAcquireContextW, 0 },
	/*{ "Advapi32.dll", "CryptGenKey", (LPVOID)&NewCryptGenKey, &OldCryptGenKey, 0 },
	{ "Advapi32.dll", "CryptGetUserKey", (LPVOID)&NewCryptGetUserKey, &OldCryptGetUserKey, 0 },
	{ "Advapi32.dll", "CryptExportKey", (LPVOID)&NewCryptExportKey, &OldCryptExportKey, 0 },
	{ "Advapi32.dll", "CryptCreateHash", (LPVOID)&NewCryptCreateHash, &OldCryptCreateHash, 0 },*/
	{ "Advapi32.dll", "CryptHashData", (LPVOID)&NewCryptHashData, &OldCryptHashData, 0 },
	/*{ "Advapi32.dll", "CryptDeriveKey", (LPVOID)&NewCryptDeriveKey, &OldCryptDeriveKey, 0 },
	{ "Advapi32.dll", "CryptEncrypt", (LPVOID)&NewCryptEncrypt, &OldCryptEncrypt, 0 },*/
};

void HookAll()
{
	int i, NumEntries = sizeof(HookArray) / sizeof(HOOK_ARRAY);

	//Needs 25 bytes for each hooked function to hold original byte + return jump
	OriginalMemArea = VirtualAlloc(NULL, 25 * NumEntries, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
	if(!OriginalMemArea)
		return;
	
	wofstream myFile;
	myFile.open("OUTPUT.txt", ios::app);
	myFile << "Func list:" << endl;

	for (i = 0; i < NumEntries; i++)
	{
		//Split the allocated memory into a block of 25 bytes for each hooked function
		*(LPVOID *)HookArray[i].original = (LPVOID)((DWORD)OriginalMemArea + (i * 25));
		if(HookFunction(HookArray[i].dll, HookArray[i].name, HookArray[i].proxy, *(LPVOID *)HookArray[i].original, &HookArray[i].length))
		{
			myFile << HookArray[i].name << endl;
		}
	}
	myFile << endl;
	myFile.close();
}

void UnhookAll()
{
	int i, NumEntries = sizeof(HookArray) / sizeof(HOOK_ARRAY);

	for(i = 0; i < NumEntries; i++)
		UnhookFunction(HookArray[i].dll, HookArray[i].name, *(LPVOID *)HookArray[i].original, HookArray[i].length); 

	VirtualFree(OriginalMemArea, 0, MEM_RELEASE);
}

//BOOL WINAPI NewCryptExportKey(HCRYPTKEY hKey, HCRYPTKEY hExpKey, DWORD dwBlobType, DWORD dwFlags, BYTE *pbData, DWORD *pdwDataLen)
//{
//	wofstream myFile;
//	myFile.open("OUTPUT.txt", ios::app);
//	myFile << "CryptExportKey: " << endl;
//	BOOL OldCryptExportKeyResult = OldCryptExportKey(hKey, hExpKey, dwBlobType, dwFlags, pbData, pdwDataLen);
//	myFile << "key to be exported: " << hKey << endl;
//	myFile << "cryptographic key of the destination user: " << hExpKey << endl;
//	myFile << " type of key BLOB: " << dwBlobType << endl;
//	myFile << "additional options: " << dwFlags << endl;
//	myFile << endl;
//	myFile.close();
//	return OldCryptExportKeyResult;
//}
//
//BOOL WINAPI NewCryptGetUserKey(HCRYPTPROV hProv, DWORD dwKeySpec, HCRYPTKEY *phUserKey)
//{
//	wofstream myFile;
//	myFile.open("OUTPUT.txt", ios::app);
//	myFile << "CryptGetUserKey: " << endl;
//	BOOL OldCryptGetUserKeyResult = OldCryptGetUserKey(hProv, dwKeySpec, phUserKey);
//	myFile << "Private key identification: " << dwKeySpec << endl;
//	myFile << "Retrieved key: " << phUserKey << endl;
//	myFile << endl;
//	myFile.close();
//	return OldCryptGetUserKeyResult;
//}
//
//BOOL WINAPI NewCryptGenKey(HCRYPTPROV hProv, ALG_ID Algid, DWORD dwFlags, HCRYPTKEY *phKey)
//{
//	wofstream myFile;
//	myFile.open("OUTPUT.txt", ios::app);
//	myFile << "CryptGenKey: " << endl;
//	BOOL OldCryptGenKeyResult = OldCryptGenKey(hProv, Algid, dwFlags, phKey);
//	myFile << "Algorithm ID: " << Algid << endl;
//	myFile << "Type of key generated: " << dwFlags << endl;
//	myFile << "Generated key: " << phKey << endl;
//	myFile << endl;
//	myFile.close();
//	return OldCryptGenKeyResult;
//}

void AddCreateFileHandle(HANDLE CreateFileResult, LPCTSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
	CreateFileHandle.CreateFileHandle = CreateFileResult;
	CreateFileHandle.lpFileName = lpFileName;
	CreateFileHandle.dwDesiredAccess = dwDesiredAccess;
	CreateFileHandle.dwShareMode = dwShareMode;
	CreateFileHandle.lpSecurityAttributes = lpSecurityAttributes;
	CreateFileHandle.dwCreationDisposition = dwCreationDisposition;
	CreateFileHandle.dwFlagsAndAttributes = dwFlagsAndAttributes;
	CreateFileHandle.hTemplateFile = hTemplateFile;
}

HANDLE WINAPI NewCreateFileA(LPCTSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
	//char MOTD[256] = "CreateFileA\n"; //Create buffer with message of the day
	//send(Connection, MOTD, sizeof(MOTD), NULL); //Send MOTD buffer
	wofstream myFile;
	myFile.open("OUTPUT.txt", ios::app);
	myFile << "CreateFileA:" << endl;
	myFile << "File name: " << lpFileName << endl;
	myFile << "Desired access: " << dwDesiredAccess << endl;
	myFile << "Share mode: " << dwShareMode << endl;
	myFile << "Creation disposition: " << dwCreationDisposition << endl;
	myFile << "File attribute: " << dwFlagsAndAttributes << endl;
	HANDLE OldCreateFileAResult = OldCreateFileA(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
	myFile << endl;
	myFile.close();
	AddCreateFileHandle(OldCreateFileAResult, lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
	return OldCreateFileAResult;
}

HANDLE WINAPI NewCreateFileW(LPCTSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
	char MOTD[256] = "CreateFileW\n"; //Create buffer with message of the day
	send(Connection, MOTD, sizeof(MOTD), NULL); //Send MOTD buffer
	wofstream myFile;
	myFile.open("OUTPUT.txt", ios::app);
	myFile << "CreateFileW:" << endl;
	myFile << "File name: " << lpFileName << endl;
	myFile << "Desired access: " << dwDesiredAccess << endl;
	myFile << "Share mode: " << dwShareMode << endl;
	myFile << "Creation disposition: " << dwCreationDisposition << endl;
	myFile << "File attribute: " << dwFlagsAndAttributes << endl;
	HANDLE OldCreateFileWResult = OldCreateFileW(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
	myFile << endl;
	myFile.close();
	AddCreateFileHandle(OldCreateFileWResult, lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
	return OldCreateFileWResult;
}

BOOL WINAPI NewWriteFile(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped)
{
	wofstream myFile;
	myFile.open("OUTPUT.txt", ios::app);
	myFile << "WriteFile: " << endl;
	BOOL OldWriteFileResult = OldWriteFile(hFile, lpBuffer, nNumberOfBytesToWrite, lpNumberOfBytesWritten, lpOverlapped);
	if (hFile == CreateFileHandle.CreateFileHandle)
	{
		myFile << "Write to file: " << CreateFileHandle.lpFileName << endl;
	}
	myFile << "Data writed: " << lpBuffer << " Number of bytes: " << lpNumberOfBytesWritten << endl;
	myFile << endl;
	myFile.close();
	return OldWriteFileResult;
}

BOOL WINAPI NewReadFile(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped)
{
	wofstream myFile;
	myFile.open("OUTPUT.txt", ios::app);
	myFile << "ReadFile: " << endl;
	BOOL OldReadFileResult = OldReadFile(hFile, lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead, lpOverlapped);
	if (hFile == CreateFileHandle.CreateFileHandle)
	{
		myFile << "Read from file: " << CreateFileHandle.lpFileName << endl;
	}
	myFile << "Data Read: " << lpBuffer << " Number of bytes: " << lpNumberOfBytesRead << endl;
	myFile << endl;
	myFile.close();
	return OldReadFileResult;
}

BOOL WINAPI NewCryptHashData(HCRYPTHASH hHash, BYTE *pbData, DWORD dwDataLen, DWORD dwFlags)
{
	wofstream myFile;
	myFile.open("OUTPUT.txt", ios::app);
	myFile << "CryptHashData: " << endl;
	BOOL OldCryptHashDataResult = OldCryptHashData(hHash, pbData, dwDataLen, dwFlags);
	CString s = CString((const wchar_t*)pbData);
	myFile << "Password: " << s << endl;
	myFile << endl;
	myFile.close();
	return OldCryptHashDataResult;
}

BOOL WINAPI NewCryptAcquireContextA(HCRYPTPROV *phProv, LPCTSTR pszContainer, LPCTSTR pszProvider, DWORD dwProvType, DWORD dwFlags)
{
	wofstream myFile;
	myFile.open("OUTPUT.txt", ios::app);
	myFile << "CryptAcquireContextA: " << endl;
	BOOL OldCryptAcquireContextAResult = OldCryptAcquireContextA(phProv, pszContainer, pszProvider, dwProvType, dwFlags);
	myFile << "Provider Name: " << pszProvider << endl;
	myFile << "Provider Type: ";
	switch ((int)dwProvType)
	{
	case 1: myFile << "PROV_RSA_FULL" << endl; break;
	case 24: myFile << "PROV_RSA_AES" << endl; break;
	case 2: myFile << "PROV_RSA_SIG" << endl; break;
	case 12: myFile << "PROV_RSA_SCHANNEL" << endl; break;
	case 3: myFile << "PROV_DSS" << endl; break;
	case 13: myFile << "PROV_DSS_DH" << endl; break;
	case 18: myFile << "PROV_DH_SCHANNEL" << endl; break;
	case 4: myFile << "PROV_FORTEZZA" << endl; break;
	case 5: myFile << "PROV_MS_EXCHANGE" << endl; break;
	case 6: myFile << "PROV_SSL" << endl; break;
	}
	myFile << endl;
	myFile.close();
	return OldCryptAcquireContextAResult;
}

BOOL WINAPI NewCryptAcquireContextW(HCRYPTPROV *phProv, LPCTSTR pszContainer, LPCTSTR pszProvider, DWORD dwProvType, DWORD dwFlags)
{
	wofstream myFile;
	myFile.open("OUTPUT.txt", ios::app);
	myFile << "CryptAcquireContextW: " << endl;
	BOOL OldCryptAcquireContextWResult = OldCryptAcquireContextW(phProv, pszContainer, pszProvider, dwProvType, dwFlags);
	myFile << "Provider Name: " << pszProvider << endl;;
	myFile << "Provider Type: ";
	switch ((int)dwProvType)
	{
	case 1: myFile << "PROV_RSA_FULL" << endl; break;
	case 24: myFile << "PROV_RSA_AES" << endl; break;
	case 2: myFile << "PROV_RSA_SIG" << endl; break;
	case 12: myFile << "PROV_RSA_SCHANNEL" << endl; break;
	case 3: myFile << "PROV_DSS" << endl; break;
	case 13: myFile << "PROV_DSS_DH" << endl; break;
	case 18: myFile << "PROV_DH_SCHANNEL" << endl; break;
	case 4: myFile << "PROV_FORTEZZA" << endl; break;
	case 5: myFile << "PROV_MS_EXCHANGE" << endl; break;
	case 6: myFile << "PROV_SSL" << endl; break;
	}
	myFile << endl;
	myFile.close();
	return OldCryptAcquireContextWResult;
}

BOOL WINAPI NewFindNextFileA(HANDLE hFindFile, LPWIN32_FIND_DATA lpFindFileData)
{
	wofstream myFile;
	myFile.open("OUTPUT.txt", ios::app);
	myFile << "Search Files: " << endl;
	BOOL OldFindNextFileAResult = OldFindNextFileA(hFindFile, lpFindFileData);
	myFile << "File found: " << lpFindFileData->cFileName << endl;
	myFile << endl;
	myFile.close();
	return OldFindNextFileAResult;
}

HANDLE WINAPI NewFindFirstFileA(LPCTSTR lpFileName, LPWIN32_FIND_DATA lpFindFileData)
{
	wofstream myFile;
	myFile.open("OUTPUT.txt", ios::app);
	myFile << "Search Files: " << endl;
	myFile << "Directory: " << lpFileName << endl;
	HANDLE OldFindFirstFileAResult = OldFindFirstFileA(lpFileName, lpFindFileData);
	myFile << "File found: " << lpFindFileData->cFileName << endl;
	myFile << endl;
	myFile.close();
	return OldFindFirstFileAResult;
}

BOOL WINAPI NewFindNextFileW(HANDLE hFindFile, LPWIN32_FIND_DATA lpFindFileData)
{
	wofstream myFile;
	myFile.open("OUTPUT.txt", ios::app);
	myFile << "Search Files: " << endl;
	BOOL OldFindNextFileWResult = OldFindNextFileW(hFindFile, lpFindFileData);
	myFile << "File found: " << lpFindFileData->cFileName << endl;
	myFile << endl;
	myFile.close();
	return OldFindNextFileWResult;
}

HANDLE WINAPI NewFindFirstFileW(LPCTSTR lpFileName, LPWIN32_FIND_DATA lpFindFileData)
{
	wofstream myFile;
	myFile.open("OUTPUT.txt", ios::app);
	myFile << "Search Files: " << endl;
	myFile << "Directory: " << lpFileName << endl;
	HANDLE OldFindFirstFileWResult = OldFindFirstFileW(lpFileName, lpFindFileData);
	myFile << "File found: " << lpFindFileData->cFileName << endl;
	myFile << endl;
	myFile.close();
	return OldFindFirstFileWResult;
}

unsigned long WINAPI NewInet_addr(const char *cp)
{
	wofstream myFile;
	myFile.open("OUTPUT.txt", ios::app);
	myFile << "Winsock Inet_addr: " << endl;
	myFile << cp << endl;
	unsigned long OldInet_addrResult = OldInet_addr(cp);
	myFile << endl;
	myFile.close();
	return OldInet_addrResult;
}

SOCKET WINAPI NewSocket(int af, int type, int protocol)
{
	wofstream myFile;
	myFile.open("OUTPUT.txt", ios::app);
	myFile << "Winsock socket: " << endl;
	myFile << af << ", " << type << ", " << protocol << endl;
	SOCKET OldSocketResult = OldSocket(af, type, protocol);
	myFile << endl;
	myFile.close();
	return OldSocketResult;
}

u_short WINAPI NewHtons(u_short hostshort)
{
	wofstream myFile;
	myFile.open("OUTPUT.txt", ios::app);
	myFile << "Winsock htons: " << endl;
	myFile << hostshort << endl;
	unsigned long OldHtonsResult = OldHtons(hostshort);
	myFile << endl;
	myFile.close();
	return OldHtonsResult;
}

//int WINAPI NewSend(SOCKET s, const char *buf, int len, int flags)
//{
//	wofstream myFile;
//	myFile.open("OUTPUT.txt", ios::app);
//	myFile << "Winsock send: " << endl;
//	int OldSendResult = OldSend(s, buf, len, flags);
//	myFile << s << ", " << buf << ", " << len << ", " << flags << endl;
//	myFile << endl;
//	myFile.close();
//	return OldSendResult;
//}

int WINAPI NewRecv(SOCKET s, char *buf, int len, int flags)
{
	wofstream myFile;
	myFile.open("OUTPUT.txt", ios::app);
	myFile << "Winsock recv: " << endl;
	int OldRecvResult = OldRecv(s, buf, len, flags);
	myFile << s << ", " << buf << ", " << len << ", " << flags << ", " << OldRecvResult << endl;
	myFile << endl;
	myFile.close();
	return OldRecvResult;
}

int WINAPI NewWSAStartup(WORD wVersionRequested, LPWSADATA lpWSAData)
{
	wofstream myFile;
	myFile.open("OUTPUT.txt", ios::app);
	myFile << "Winsock WSAStartup: " << endl;
	int OldWSAStartupResult = OldWSAStartup(wVersionRequested, lpWSAData);
	myFile << wVersionRequested << ", " << lpWSAData << endl;
	myFile << endl;
	myFile.close();
	return OldWSAStartupResult;
}

int WINAPI NewConnect(SOCKET s, const struct sockaddr *name, int namelen)
{
	wofstream myFile;
	myFile.open("OUTPUT.txt", ios::app);
	myFile << "Winsock connect:" << endl;
	if ((int)name->sa_family == AF_INET)
	{
		myFile << "IPv4" << endl;
	}
	myFile << s << ", " << (SOCKADDR*)name->sa_family << ", " << (int)name->sa_family << ", " << namelen << endl;

	int oldConnectResult = OldConnect(s, name, namelen);
	myFile << oldConnectResult;
	myFile << endl;
	myFile.close();
	return oldConnectResult;
}

int WINAPI NewMessageBoxA(HWND hWnd, LPCSTR lpText, LPCTSTR lpCaption, UINT uType)
{
	ofstream myFile;
	myFile.open("OUTPUT.txt");
	myFile << "MSG A" << endl;

	myFile << lpText << ", " << lpCaption << endl;

	int OldMessageBoxAResult = OldMessageBoxA(hWnd, lpText, lpCaption, uType);
	myFile << endl;
	myFile.close();
	//printf("MessageBoxA called!\ntitle: %s\ntext: %s\n\n", lpCaption, lpText);
	return OldMessageBoxAResult;
}

int WINAPI NewMessageBoxW(HWND hWnd, LPWSTR lpText, LPCTSTR lpCaption, UINT uType)
{
	ofstream myFile;
	myFile.open("OUTPUT.txt");
	myFile << "MSG W" << endl;
	myFile << endl;
	myFile.close();
	//printf("MessageBoxW called!\ntitle: %ws\ntext: %ws\n\n", lpCaption, lpText);
	return OldMessageBoxW(hWnd, lpText, lpCaption, uType);
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

#ifndef NO_INLINE_ASM
	__asm 
	{
		lea esi, SourceBuffer;
		mov edi, destination;

		mov eax, [edi];
		mov edx, [edi+4];
		mov ebx, [esi];
		mov ecx, [esi+4];

		lock cmpxchg8b[edi];
	}
#else
	_InterlockedCompareExchange64((LONGLONG *)destination, *(LONGLONG *)SourceBuffer, *(LONGLONG *)destination);
#endif
}

BOOL HookFunction(CHAR *dll, CHAR *name, LPVOID proxy, LPVOID original, PDWORD length)
{
	LPVOID FunctionAddress;
	DWORD TrampolineLength = 0, OriginalProtection;
	//hde32s disam;																				// x32 change
	hde64s disam;																				// x64 change
	BYTE Jump[5] = {0xE9, 0x00, 0x00, 0x00, 0x00};

	FunctionAddress = GetProcAddress(GetModuleHandleA(dll), name);
	
	if(!FunctionAddress)
		return FALSE;

//	printf("Address: %p\n", FunctionAddress);

	//disassemble length of each instruction, until we have 5 or more bytes worth
	while(TrampolineLength < 5)
	{
		LPVOID InstPointer = (LPVOID)((DWORD64)FunctionAddress + TrampolineLength);				// in x32 -> DWORD ||| in x64 -> DWORD64
		//TrampolineLength += hde32_disasm(InstPointer, &disam);								// x32 change
		TrampolineLength += hde64_disasm(InstPointer, &disam);									// x64 change
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

BOOL UnhookFunction(CHAR *dll, CHAR *name, LPVOID original, DWORD length)
{
	LPVOID FunctionAddress;
	DWORD OriginalProtection;

	FunctionAddress = GetProcAddress(GetModuleHandleA(dll), name);
	if(!FunctionAddress)
		return FALSE;

	if(!VirtualProtect(FunctionAddress, length, PAGE_EXECUTE_READWRITE, &OriginalProtection))
		return FALSE;

	SafeMemcpyPadded(FunctionAddress, original, length);

	VirtualProtect(FunctionAddress, length, PAGE_EXECUTE_READWRITE, &OriginalProtection);

	FlushInstructionCache(GetCurrentProcess(), FunctionAddress, length);

	return TRUE;
}

BOOL OpenConnection()
{
	//Winsock Startup
	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 1);
	if (WSAStartup(DllVersion, &wsaData) != 0) //If WSAStartup returns anything other than 0, then that means an error has occured in the WinSock Startup.
	{
		MessageBoxA(NULL, "Winsock startup failed", "Client Error", MB_OK | MB_ICONERROR);
		return FALSE;
	}

	SOCKADDR_IN addr; //Address to be binded to our Connection socket
	int sizeofaddr = sizeof(addr); //Need sizeofaddr for the connect function
	addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //Address = localhost (this pc)
	addr.sin_port = htons(35000); //Port = 35782
	addr.sin_family = AF_INET; //IPv4 Socket

	Connection = socket(AF_INET, SOCK_STREAM, NULL); //Set Connection socket
	if (connect(Connection, (SOCKADDR*)&addr, sizeofaddr) != 0) //If we are unable to connect...
	{
		MessageBoxA(NULL, "Failed to Connect", "Client Error", MB_OK | MB_ICONERROR);
		return FALSE; //Failed to Connnameect
	}
	else
	{
		char MOTD[256] = "Client program connected successfuly.\n\n"; //Create buffer with message of the day
		send(Connection, MOTD, sizeof(MOTD), NULL); //Send MOTD buffer
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
		if(OpenConnection())
			HookAll();
		else exit(0);
//		MessageBoxA(NULL, "Hello A", "MsgBoxA Test", MB_OK);
		//MessageBoxA(NULL, "World", "MsgBoxA Test", MB_OK);

	//	MessageBoxW(NULL, L"Hello W", L"MsgBoxW Test", MB_OK);
		//MessageBoxW(NULL, L"World", L"MsgBoxW Test", MB_OK);

	//	runExternalProcessSuspended();

	//	UnhookAll();
	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		/*wofstream myFile;
		myFile.open("OUTPUT.txt", ios::app);
		myFile << "FUCK OFF\n" << endl;
		myFile.close();
		UnhookAll();*/
		CloseConnection();
	}
	return TRUE;
}

