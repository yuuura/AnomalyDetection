#ifndef UNICODE
#define UNICODE
#endif

#define _CRT_SECURE_NO_WARNINGS

#include "Kernel32.h"
#include "FileHandle.h"
#include "Ws2_32.h"

#define SLEEP_TIME 10
#define BUFFER_SIZE 512

// Seach Files
//Multi-Char
TdefOldFindFirstFileA OldFindFirstFileA;	
TdefOldFindNextFileA OldFindNextFileA;
TdefOldCreateProcessA OldCreateProcessA;
//Unicode
TdefOldFindFirstFileW OldFindFirstFileW;	
TdefOldFindNextFileW OldFindNextFileW;
TdefOldCreateProcessW OldCreateProcessW;

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

// Convert string from Unicode to multi-byte.
LPSTR UnicodeToMByte(LPCWSTR unicodeStr)
{
	// Get the required size of the buffer that receives the multiByte string.
	DWORD size = WideCharToMultiByte(CP_UTF8, 0, unicodeStr, -1, 0, 0, NULL, FALSE);
	LPSTR multiByteStr = new char[BUFFER_SIZE];
	WideCharToMultiByte(CP_UTF8, NULL, unicodeStr, -1, multiByteStr, size, NULL, FALSE);
	return multiByteStr;
}

BOOL WINAPI NewCreateProcessA(LPCTSTR lpApplicationName, LPTSTR lpCommandLine, LPSECURITY_ATTRIBUTES lpProcessAttributes, LPSECURITY_ATTRIBUTES lpThreadAttributes, BOOL bInheritHandles, DWORD dwCreationFlags, LPVOID lpEnvironment, LPCTSTR lpCurrentDirectory, LPSTARTUPINFO lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation)
{
	char buffer[BUFFER_SIZE] = "CreateProcess ";
	strcat(buffer, "\n\nCreating process...");
	if (lpApplicationName != NULL)
	{
		strcat(buffer, "\nName of module to be executed: ");
		strcat(buffer, (const char*)(lpApplicationName));
	}
	if (lpCommandLine != NULL)
	{
		strcat(buffer, "\nCommand line to be executed: ");
		strcat(buffer, (const char*)(lpCommandLine));
	}
	if (lpCurrentDirectory != NULL)
	{
		strcat(buffer, "\nDirectory: ");
		strcat(buffer, (const char*)(lpCurrentDirectory));
	}
	
	BOOL OldCreateProcessAResult = OldCreateProcessA(lpApplicationName, lpCommandLine, lpProcessAttributes, lpThreadAttributes, bInheritHandles, dwCreationFlags, lpEnvironment, lpCurrentDirectory, lpStartupInfo, lpProcessInformation);
	if (OldCreateProcessAResult)
		strcat(buffer, "\nCreate process succeeded.");
	else strcat(buffer, "\nCreate process failed.");
	
	send(Connection, buffer, sizeof(buffer), NULL);
	Sleep(SLEEP_TIME);
	return OldCreateProcessAResult;
}

BOOL WINAPI NewCreateProcessW(LPCTSTR lpApplicationName, LPTSTR lpCommandLine, LPSECURITY_ATTRIBUTES lpProcessAttributes, LPSECURITY_ATTRIBUTES lpThreadAttributes, BOOL bInheritHandles, DWORD dwCreationFlags, LPVOID lpEnvironment, LPCTSTR lpCurrentDirectory, LPSTARTUPINFO lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation)
{
	char buffer[BUFFER_SIZE] = "CreateProcess ";
	strcat(buffer, "\n\nCreating process...");
	if (lpApplicationName != NULL)
	{
		strcat(buffer, "\nName of module to be executed: ");
		strcat(buffer, UnicodeToMByte(lpApplicationName));
	}
	if (lpCommandLine != NULL)
	{
		strcat(buffer, "\nCommand lien to be executed: ");
		strcat(buffer, UnicodeToMByte(lpCommandLine));
	}
	if (lpCurrentDirectory != NULL)
	{
		strcat(buffer, "\nDirectory: ");
		strcat(buffer, UnicodeToMByte(lpCurrentDirectory));
	}
	else {
		strcat(buffer, "\nDirectory: Current Directory.");
	}
	BOOL OldCreateProcessWResult = OldCreateProcessW(lpApplicationName, lpCommandLine, lpProcessAttributes, lpThreadAttributes, bInheritHandles, dwCreationFlags, lpEnvironment, lpCurrentDirectory, lpStartupInfo, lpProcessInformation);
	if (OldCreateProcessWResult)
		strcat(buffer, "\nCreate process succeeded.");
	else strcat(buffer, "\nCreate process failed.");
	
	send(Connection, buffer, sizeof(buffer), NULL);
	Sleep(SLEEP_TIME);
	return OldCreateProcessWResult;
}

HANDLE WINAPI NewCreateFileA(LPCTSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
	char buffer[BUFFER_SIZE] = "CreateFile ";
	//Find out if file created or opened
	switch (dwCreationDisposition)
	{
	case CREATE_ALWAYS:
	case CREATE_NEW: strcat(buffer, "\n\nNew file created: "); break;
	case OPEN_ALWAYS:
	case OPEN_EXISTING: strcat(buffer, "\n\nExisting file openened: "); break;
	case TRUNCATE_EXISTING: strcat(buffer, "\n\nTruncating existing file: "); break;
	}

	strcat(buffer, (const char*)(lpFileName));
	strcat(buffer, "\nFile Access:");
	
	// File access
	if ((dwDesiredAccess & GENERIC_ALL) == GENERIC_ALL) strcat(buffer, " EXECUTE READ WRITE ");
	if ((dwDesiredAccess & GENERIC_EXECUTE) == GENERIC_EXECUTE) strcat(buffer, " EXECUTE ");
	if ((dwDesiredAccess & GENERIC_WRITE) == GENERIC_WRITE) strcat(buffer, " WRITE ");
	if ((dwDesiredAccess & GENERIC_READ) == GENERIC_READ) strcat(buffer, " READ ");
	if ((dwDesiredAccess & FILE_ADD_FILE) == FILE_ADD_FILE) strcat(buffer, "  \"Create a file in the directory\" ");
	if ((dwDesiredAccess & FILE_ADD_SUBDIRECTORY) == FILE_ADD_SUBDIRECTORY) strcat(buffer, " \"Create a subdirectory\" ");
	if ((dwDesiredAccess & FILE_ALL_ACCESS) == FILE_ALL_ACCESS) strcat(buffer, " \"All possible access rights for a file\" ");
	if ((dwDesiredAccess & FILE_APPEND_DATA) == FILE_APPEND_DATA) strcat(buffer, " \"Append data to the file\" ");
	if ((dwDesiredAccess & FILE_CREATE_PIPE_INSTANCE) == FILE_CREATE_PIPE_INSTANCE) strcat(buffer, " \"Create a pipe\" ");
	if ((dwDesiredAccess & FILE_DELETE_CHILD) == FILE_DELETE_CHILD) strcat(buffer, " \"Delete a directory and all the files it contains, including read-only files\" ");
	if ((dwDesiredAccess & FILE_EXECUTE) == FILE_EXECUTE) strcat(buffer, " \"Execute the file\" ");
	if ((dwDesiredAccess & FILE_LIST_DIRECTORY) == FILE_LIST_DIRECTORY) strcat(buffer, " \"List the contents of the directory\" ");
	if ((dwDesiredAccess & FILE_READ_ATTRIBUTES) == FILE_READ_ATTRIBUTES) strcat(buffer, " \"Read file attributes\" ");
	if ((dwDesiredAccess & FILE_READ_DATA) == FILE_READ_DATA) strcat(buffer, " \"Read the corresponding file or directory data\" ");
	if ((dwDesiredAccess & FILE_READ_EA) == FILE_READ_EA) strcat(buffer, " \"Read extended file attributes\" ");
	if ((dwDesiredAccess & FILE_TRAVERSE) == FILE_TRAVERSE) strcat(buffer, " \"Right to traverse the directory.\" ");
	if ((dwDesiredAccess & FILE_WRITE_ATTRIBUTES) == FILE_WRITE_ATTRIBUTES) strcat(buffer, " \"Write file attributes\" ");
	if ((dwDesiredAccess & FILE_WRITE_DATA) == FILE_WRITE_DATA) strcat(buffer, " \"Write data to the file\" ");
	if ((dwDesiredAccess & FILE_WRITE_EA) == FILE_WRITE_EA) strcat(buffer, " \"Write extended file attributes\" ");
	if ((dwDesiredAccess & STANDARD_RIGHTS_READ) == STANDARD_RIGHTS_READ) strcat(buffer, " \"Read the information in the file or directory object's security descriptor\" ");
	if ((dwDesiredAccess & STANDARD_RIGHTS_WRITE) == STANDARD_RIGHTS_WRITE) strcat(buffer, " \"Write the information in the file or directory object's security descriptor\" ");
	
	send(Connection, buffer, sizeof(buffer), NULL);
	Sleep(SLEEP_TIME);
	HANDLE OldCreateFileAResult = OldCreateFileA(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
	AddCreateFileHandle(OldCreateFileAResult, (char*)lpFileName, dwDesiredAccess, dwCreationDisposition);
	return OldCreateFileAResult;
}

HANDLE WINAPI NewCreateFileW(LPCTSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile)
{
	char buffer[BUFFER_SIZE] = "CreateFile ";
	
	//Find out if file created or opened
	switch (dwCreationDisposition)
	{
	case CREATE_ALWAYS:
	case CREATE_NEW: strcat(buffer, "\n\nNew file created: "); break;
	case OPEN_ALWAYS:
	case OPEN_EXISTING: strcat(buffer, "\n\nExisting file openened: "); break;
	case TRUNCATE_EXISTING: strcat(buffer, "\n\nTruncating existing file: "); break;
	}
	
	strcat(buffer, UnicodeToMByte(lpFileName));
	strcat(buffer, "\nFile Access:");
	
	// File access
	if ((dwDesiredAccess & GENERIC_ALL) == GENERIC_ALL) strcat(buffer, " EXECUTE READ WRITE ");
	if ((dwDesiredAccess & GENERIC_EXECUTE) == GENERIC_EXECUTE) strcat(buffer, " EXECUTE ");
	if ((dwDesiredAccess & GENERIC_WRITE) == GENERIC_WRITE) strcat(buffer, " WRITE ");
	if ((dwDesiredAccess & GENERIC_READ) == GENERIC_READ) strcat(buffer, " READ ");
	if ((dwDesiredAccess & FILE_ADD_FILE) == FILE_ADD_FILE) strcat(buffer, "  \"Create a file in the directory\" ");
	if ((dwDesiredAccess & FILE_ADD_SUBDIRECTORY) == FILE_ADD_SUBDIRECTORY) strcat(buffer, " \"Create a subdirectory\" ");
	if ((dwDesiredAccess & FILE_ALL_ACCESS) == FILE_ALL_ACCESS) strcat(buffer, " \"All possible access rights for a file\" ");
	if ((dwDesiredAccess & FILE_APPEND_DATA) == FILE_APPEND_DATA) strcat(buffer, " \"Append data to the file\" ");
	if ((dwDesiredAccess & FILE_CREATE_PIPE_INSTANCE) == FILE_CREATE_PIPE_INSTANCE) strcat(buffer, " \"Create a pipe\" ");
	if ((dwDesiredAccess & FILE_DELETE_CHILD) == FILE_DELETE_CHILD) strcat(buffer, " \"Delete a directory and all the files it contains, including read-only files\" ");
	if ((dwDesiredAccess & FILE_EXECUTE) == FILE_EXECUTE) strcat(buffer, " \"Execute the file\" ");
	if ((dwDesiredAccess & FILE_LIST_DIRECTORY) == FILE_LIST_DIRECTORY) strcat(buffer, " \"List the contents of the directory\" ");
	if ((dwDesiredAccess & FILE_READ_ATTRIBUTES) == FILE_READ_ATTRIBUTES) strcat(buffer, " \"Read file attributes\" ");
	if ((dwDesiredAccess & FILE_READ_DATA) == FILE_READ_DATA) strcat(buffer, " \"Read the corresponding file or directory data\" ");
	if ((dwDesiredAccess & FILE_READ_EA) == FILE_READ_EA) strcat(buffer, " \"Read extended file attributes\" ");
	if ((dwDesiredAccess & FILE_TRAVERSE) == FILE_TRAVERSE) strcat(buffer, " \"Right to traverse the directory.\" ");
	if ((dwDesiredAccess & FILE_WRITE_ATTRIBUTES) == FILE_WRITE_ATTRIBUTES) strcat(buffer, " \"Write file attributes\" ");
	if ((dwDesiredAccess & FILE_WRITE_DATA) == FILE_WRITE_DATA) strcat(buffer, " \"Write data to the file\" ");
	if ((dwDesiredAccess & FILE_WRITE_EA) == FILE_WRITE_EA) strcat(buffer, " \"Write extended file attributes\" ");
	if ((dwDesiredAccess & STANDARD_RIGHTS_READ) == STANDARD_RIGHTS_READ) strcat(buffer, " \"Read the information in the file or directory object's security descriptor\" ");
	if ((dwDesiredAccess & STANDARD_RIGHTS_WRITE) == STANDARD_RIGHTS_WRITE) strcat(buffer, " \"Write the information in the file or directory object's security descriptor\" ");
	
	send(Connection, buffer, sizeof(buffer), NULL);
	Sleep(SLEEP_TIME);
	HANDLE OldCreateFileWResult = OldCreateFileW(lpFileName, dwDesiredAccess, dwShareMode, lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
	AddCreateFileHandle(OldCreateFileWResult, UnicodeToMByte(lpFileName), dwDesiredAccess, dwCreationDisposition);
	return OldCreateFileWResult;
}

BOOL WINAPI NewWriteFile(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped)
{
	BOOL OldWriteFileResult = OldWriteFile(hFile, lpBuffer, nNumberOfBytesToWrite, lpNumberOfBytesWritten, lpOverlapped);
	char buffer[BUFFER_SIZE] = "WriteFile ";
	int index = FileHandleFindElement(hFile);
	if (index >= 0)
	{
		if (fileHandleArray[index].isWritten)
			return OldWriteFileResult;
		strcat(buffer, "\n\nWriting to a file: ");
		strcat(buffer, fileHandleArray[index].lpFileName);
		FileHandleMarkAsWritten(hFile);
	}
	else strcat(buffer, "\n(*) Error while reading file name.");
	
	send(Connection, buffer, sizeof(buffer), NULL);
	Sleep(SLEEP_TIME);
	return OldWriteFileResult;
}

BOOL WINAPI NewReadFile(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped)
{
	BOOL OldReadFileResult = OldReadFile(hFile, lpBuffer, nNumberOfBytesToRead, lpNumberOfBytesRead, lpOverlapped);
	char buffer[BUFFER_SIZE] = "ReadFile ";
	int index = FileHandleFindElement(hFile);
	if (index >= 0)
	{
		FileHandleAddBuffer(hFile, lpBuffer);
		if(fileHandleArray[index].isRead)
			return OldReadFileResult;

		strcat(buffer, "\n\nReading file: ");
		strcat(buffer, fileHandleArray[index].lpFileName);
		FileHandleMarkAsRead(hFile);
	}	
	else strcat(buffer, "\n(*) Error while reading file name.");
	
	send(Connection, buffer, sizeof(buffer), NULL);
	Sleep(SLEEP_TIME);
	return OldReadFileResult;
}

HANDLE WINAPI NewFindFirstFileA(LPCTSTR lpFileName, LPWIN32_FIND_DATA lpFindFileData)
{
	char buffer[BUFFER_SIZE] = "FindFirstFile ";
	
	strcat(buffer, "\n\nSearch first file: ");
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
	Sleep(SLEEP_TIME);
	AddCreateFileHandle(OldFindFirstFileAResult, (char*)lpFileName, NULL, NULL);
	return OldFindFirstFileAResult;
}

HANDLE WINAPI NewFindFirstFileW(LPCTSTR lpFileName, LPWIN32_FIND_DATA lpFindFileData)
{
	char buffer[BUFFER_SIZE] = "FindFirstFile ";

	strcat(buffer, "\n\nSearch first file: ");
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
	Sleep(SLEEP_TIME);
	AddCreateFileHandle(OldFindFirstFileWResult, UnicodeToMByte(lpFileName), NULL, NULL);
	return OldFindFirstFileWResult;
}

BOOL WINAPI NewFindNextFileA(HANDLE hFindFile, LPWIN32_FIND_DATA lpFindFileData)
{
	char buffer[BUFFER_SIZE] = "FindNextFile ";
	
	strcat(buffer, "\n\nSearch file: ");
	int index = FileHandleFindElement(hFindFile);
	if (index >= 0)
		strcat(buffer, fileHandleArray[index].lpFileName);
	else strcat(buffer, "\n(*) Error while reading file name.");
	BOOL OldFindNextFileAResult = OldFindNextFileA(hFindFile, lpFindFileData);
	if (OldFindNextFileAResult != 0)
	{
		strcat(buffer, "\nFile found: ");
		strcat(buffer, (char*)lpFindFileData->cFileName);
	}
	
	send(Connection, buffer, sizeof(buffer), NULL);
	Sleep(SLEEP_TIME);
	return OldFindNextFileAResult;
}

BOOL WINAPI NewFindNextFileW(HANDLE hFindFile, LPWIN32_FIND_DATA lpFindFileData)
{
	char buffer[BUFFER_SIZE] = "FindNextFile ";

	strcat(buffer, "\n\nSearch file: ");
	int index = FileHandleFindElement(hFindFile);
	if (index >= 0)
		strcat(buffer, fileHandleArray[index].lpFileName);
	else strcat(buffer, "\n(*) Error while reading file name.");
	BOOL OldFindNextFileWResult = OldFindNextFileW(hFindFile, lpFindFileData);
	if (OldFindNextFileWResult != 0)
	{
		strcat(buffer, "\nFile found: ");
		strcat(buffer, UnicodeToMByte(lpFindFileData->cFileName));
	}
	else strcat(buffer, "\nNo files found.");
	send(Connection, buffer, sizeof(buffer), NULL);
	Sleep(SLEEP_TIME);
	return OldFindNextFileWResult;
}

BOOL WINAPI NewCloseHandle(HANDLE hObject)
{
	char buffer[BUFFER_SIZE] = "CloseHandle ";

	BOOL OldCloseHandleResult = OldCloseHandle(hObject);
	strcat(buffer, "\n\nClose file: ");
	int index = FileHandleFindElement(hObject);
	if (index >= 0)
		strcat(buffer, fileHandleArray[index].lpFileName);
	else strcat(buffer, " (*) Error while reading file name.");
	FileHandleDeleteElement(hObject);
	
	send(Connection, buffer, sizeof(buffer), NULL);
	Sleep(SLEEP_TIME);
	return OldCloseHandleResult;
}
