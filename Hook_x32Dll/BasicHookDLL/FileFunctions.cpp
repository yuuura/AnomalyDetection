#ifndef UNICODE
#define UNICODE
#endif

#define _CRT_SECURE_NO_WARNINGS
#define SLEEP_TIME 10
#include "FileFunctions.h"
#include "FileHandle.h"
#include "WinsockFunctions.h"
#include <fstream>
using namespace std;

// Seach Files
TdefOldFindFirstFileA OldFindFirstFileA;	//Multi-Char
TdefOldFindNextFileA OldFindNextFileA;		//Multi-Char
TdefOldFindFirstFileW OldFindFirstFileW;	//Unicode
TdefOldFindNextFileW OldFindNextFileW;		//Unicode

// Read/Write Files
TdefOldCreateFileA OldCreateFileA;			//Multi-Char
TdefOldCreateFileW OldCreateFileW;			//Unicode
TdefOldWriteFile OldWriteFile;
TdefOldReadFile OldReadFile;
TdefOldCloseHandle OldCloseHandle;

void AddCreateFileHandle(HANDLE CreateFileResult, LPSTR lpFileName, DWORD dwDesiredAccess, DWORD dwCreationDisposition)
{
	CreateFileHANDLE fileHandle;
	fileHandle.CreateFileHandle = CreateFileResult;
	fileHandle.lpFileName = lpFileName;
	fileHandle.dwDesiredAccess = dwDesiredAccess;
	fileHandle.dwCreationDisposition = dwCreationDisposition;
	FileHandleAdd(fileHandle);
}

//==========================================================================================================
// File handle
LPSTR UnicodeToMByte(LPCWSTR unicodeStr)
{
	// Get the required size of the buffer that receives the multiByte string.
	DWORD size = WideCharToMultiByte(CP_UTF8, 0, unicodeStr, -1, 0, 0, NULL, FALSE);
	//DWORD minSize;
	//minSize = WideCharToMultiByte(CP_UTF8, NULL, unicodeStr, -1, NULL, 0, NULL, FALSE);
	/*if (size < minSize)
	{
	return FALSE;
	}*/
	LPSTR multiByteStr = new char[256];
	// Convert string from Unicode to multi-byte.
	WideCharToMultiByte(CP_UTF8, NULL, unicodeStr, -1, multiByteStr, size, NULL, FALSE);
	return multiByteStr;
}

HANDLE WINAPI NewCreateFileA(LPCTSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
	char buffer[256] = "\n\nCreateFileA";
	send(Connection, buffer, sizeof(buffer), NULL);
	Sleep(SLEEP_TIME);
	//buffer[0] = 0; // Assign null terminate

	//Find out if file created or opened
	switch (dwCreationDisposition)
	{
	case CREATE_ALWAYS:
	case CREATE_NEW: strcpy(buffer, "\nNew file created: "); break;
	case OPEN_ALWAYS:
	case OPEN_EXISTING: strcpy(buffer, "\nExisting file openened: "); break;
	case TRUNCATE_EXISTING: strcpy(buffer, "\nTruncating existing file: "); break;
	}

	strcat(buffer, (const char*)(lpFileName));
	strcat(buffer, "\nFile Access:");
	/*char access[11];
	sprintf(access, "%#010x", (dwDesiredAccess & GENERIC_WRITE));
	strcat(MOTD, access);*/
	// File access
	if ((dwDesiredAccess & GENERIC_ALL) == GENERIC_ALL) strcat(buffer, " EXECUTE READ WRITE ");
	if ((dwDesiredAccess & GENERIC_EXECUTE) == GENERIC_EXECUTE) strcat(buffer, " EXECUTE ");
	if ((dwDesiredAccess & GENERIC_WRITE) == GENERIC_WRITE) strcat(buffer, " WRITE ");
	if ((dwDesiredAccess & GENERIC_READ) == GENERIC_READ) strcat(buffer, " READ ");
	send(Connection, buffer, sizeof(buffer), NULL);

	HANDLE OldCreateFileAResult = OldCreateFileA(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
	AddCreateFileHandle(OldCreateFileAResult, (char*)lpFileName, dwDesiredAccess, dwCreationDisposition);
	return OldCreateFileAResult;
}

HANDLE WINAPI NewCreateFileW(LPCTSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
	char buffer[256] = "\n\nCreateFileW";
	send(Connection, buffer, sizeof(buffer), NULL);
	Sleep(SLEEP_TIME);
	//buffer[0] = 0; // Assign null terminate
	
	//Find out if file created or opened
	switch (dwCreationDisposition)
	{
	case CREATE_ALWAYS:
	case CREATE_NEW: strcpy(buffer, "\nNew file created: "); break;
	case OPEN_ALWAYS:
	case OPEN_EXISTING: strcpy(buffer, "\nExisting file openened: "); break;
	case TRUNCATE_EXISTING: strcpy(buffer, "\nTruncating existing file: "); break;
	}
	
	strcat(buffer, UnicodeToMByte(lpFileName));
	strcat(buffer, "\nFile Access:");
	
	// File access
	if ((dwDesiredAccess & GENERIC_ALL) == GENERIC_ALL) strcat(buffer, " EXECUTE READ WRITE ");
	if ((dwDesiredAccess & GENERIC_EXECUTE) == GENERIC_EXECUTE) strcat(buffer, " EXECUTE ");
	if ((dwDesiredAccess & GENERIC_WRITE) == GENERIC_WRITE) strcat(buffer, " WRITE ");
	if ((dwDesiredAccess & GENERIC_READ) == GENERIC_READ) strcat(buffer, " READ ");
	send(Connection, buffer, sizeof(buffer), NULL);

	HANDLE OldCreateFileWResult = OldCreateFileW(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
	AddCreateFileHandle(OldCreateFileWResult, UnicodeToMByte(lpFileName), dwDesiredAccess, dwCreationDisposition);
	return OldCreateFileWResult;
}

BOOL WINAPI NewWriteFile(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped)
{
	char buffer[256] = "\n\nWriteFile";
	send(Connection, buffer, sizeof(buffer), NULL);
	Sleep(SLEEP_TIME);
	//buffer[0] = 0; // Assign null terminate

	strcpy(buffer, "\nWriting to a file: ");
	int index = FileHandleFindElement(hFile);
	if (index >= 0)
		strcat(buffer, fileHandleArray[index].lpFileName);
	else strcat(buffer, " (*) Error while reading file name.");
	send(Connection, buffer, sizeof(buffer), NULL);

	BOOL OldWriteFileResult = OldWriteFile(hFile, lpBuffer, nNumberOfBytesToWrite, lpNumberOfBytesWritten, lpOverlapped);
	return OldWriteFileResult;
}

BOOL WINAPI NewReadFile(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped)
{
	char buffer[256] = "\n\nReadFile";
	send(Connection, buffer, sizeof(buffer), NULL);
	Sleep(SLEEP_TIME);
	//buffer[0] = 0; // Assign null terminate

	strcpy(buffer, "\nReading file: ");
	int index = FileHandleFindElement(hFile);
	if (index >= 0)
		strcat(buffer, fileHandleArray[index].lpFileName);
	else strcat(buffer, " (*) Error while reading file name.");
	send(Connection, buffer, sizeof(buffer), NULL);

	BOOL OldReadFileResult = OldReadFile(hFile, lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead, lpOverlapped);
	return OldReadFileResult;
}

HANDLE WINAPI NewFindFirstFileA(LPCTSTR lpFileName, LPWIN32_FIND_DATA lpFindFileData)
{
	char buffer[256] = "\n\nFindFirstFileA";
	send(Connection, buffer, sizeof(buffer), NULL);
	Sleep(SLEEP_TIME);

	strcpy(buffer, "\nSearch first file: ");
	strcat(buffer, (char*)lpFileName);
	HANDLE OldFindFirstFileAResult = OldFindFirstFileA(lpFileName, lpFindFileData);
	if (OldFindFirstFileAResult == INVALID_HANDLE_VALUE)
		strcat(buffer, "\nFile search failed.");
	else
	{
		strcat(buffer, "\nFirst file found: ");
		strcat(buffer, (char*)lpFindFileData->cFileName);
	}
	send(Connection, buffer, sizeof(buffer), NULL);
	AddCreateFileHandle(OldFindFirstFileAResult, (char*)lpFileName, NULL, NULL);
	return OldFindFirstFileAResult;
}

HANDLE WINAPI NewFindFirstFileW(LPCTSTR lpFileName, LPWIN32_FIND_DATA lpFindFileData)
{
	char buffer[256] = "\n\nFindFirstFileW";
	send(Connection, buffer, sizeof(buffer), NULL);
	Sleep(SLEEP_TIME);

	strcpy(buffer, "\nSearch first file: ");
	strcat(buffer, UnicodeToMByte(lpFileName));
	HANDLE OldFindFirstFileWResult = OldFindFirstFileW(lpFileName, lpFindFileData);
	if (OldFindFirstFileWResult == INVALID_HANDLE_VALUE)
		strcat(buffer, "\nFile search failed.");
	else
	{
		strcat(buffer, "\nFirst file found: ");
		strcat(buffer, UnicodeToMByte(lpFindFileData->cFileName));
	}
	send(Connection, buffer, sizeof(buffer), NULL);
	AddCreateFileHandle(OldFindFirstFileWResult, UnicodeToMByte(lpFileName), NULL, NULL);
	return OldFindFirstFileWResult;
}

BOOL WINAPI NewFindNextFileA(HANDLE hFindFile, LPWIN32_FIND_DATA lpFindFileData)
{
	char buffer[256] = "\n\nFindNextFileA";
	send(Connection, buffer, sizeof(buffer), NULL);
	Sleep(SLEEP_TIME);

	strcpy(buffer, "\nSearch file: ");
	int index = FileHandleFindElement(hFindFile);
	if (index >= 0)
		strcat(buffer, fileHandleArray[index].lpFileName);
	else strcat(buffer, " (*) Error while reading file name.");
	BOOL OldFindNextFileAResult = OldFindNextFileA(hFindFile, lpFindFileData);
	if (OldFindNextFileAResult != 0)
	{
		strcat(buffer, "\nFile found: ");
		strcat(buffer, (char*)lpFindFileData->cFileName);
	}
	send(Connection, buffer, sizeof(buffer), NULL);
	return OldFindNextFileAResult;
}

BOOL WINAPI NewFindNextFileW(HANDLE hFindFile, LPWIN32_FIND_DATA lpFindFileData)
{
	char buffer[256] = "\n\nFindNextFileW";
	send(Connection, buffer, sizeof(buffer), NULL);
	Sleep(SLEEP_TIME);

	strcpy(buffer, "\nSearch file: ");
	int index = FileHandleFindElement(hFindFile);
	if (index >= 0)
		strcat(buffer, fileHandleArray[index].lpFileName);
	else strcat(buffer, " (*) Error while reading file name.");
	BOOL OldFindNextFileWResult = OldFindNextFileW(hFindFile, lpFindFileData);
	if (OldFindNextFileWResult != 0)
	{
		strcat(buffer, "\nFile found: ");
		strcat(buffer, UnicodeToMByte(lpFindFileData->cFileName));
	}
	else strcat(buffer, "\nNo files found.");
	send(Connection, buffer, sizeof(buffer), NULL);
	return OldFindNextFileWResult;
}

BOOL WINAPI NewCloseHandle(HANDLE hObject)
{
	char buffer[256] = "\n\nCloseHandle";
	send(Connection, buffer, sizeof(buffer), NULL);
	Sleep(SLEEP_TIME);

	BOOL OldCloseHandleResult = OldCloseHandle(hObject);
	strcpy(buffer, "\nClose file: ");
	int index = FileHandleFindElement(hObject);
	if (index >= 0)
		strcat(buffer, fileHandleArray[index].lpFileName);
	else strcat(buffer, " (*) Error while reading file name.");
	FileHandleDeleteElement(hObject);
	send(Connection, buffer, sizeof(buffer), NULL);
	return OldCloseHandleResult;
}

//==========================================================================================================