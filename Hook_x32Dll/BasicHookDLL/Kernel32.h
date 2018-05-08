#pragma once

#include <Windows.h>

// Search Files
typedef HANDLE(WINAPI *TdefOldFindFirstFileA)(LPCTSTR lpFileName, LPWIN32_FIND_DATA lpFindFileData);
typedef BOOL(WINAPI *TdefOldFindNextFileA)(HANDLE hFindFile, LPWIN32_FIND_DATA lpFindFileData);
typedef HANDLE(WINAPI *TdefOldFindFirstFileW)(LPCTSTR lpFileName, LPWIN32_FIND_DATA lpFindFileData);
typedef BOOL(WINAPI *TdefOldFindNextFileW)(HANDLE hFindFile, LPWIN32_FIND_DATA lpFindFileData);

// Read/Write Files
typedef HANDLE(WINAPI *TdefOldCreateFileA)(LPCTSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
typedef HANDLE(WINAPI *TdefOldCreateFileW)(LPCTSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
typedef BOOL(WINAPI *TdefOldWriteFile)(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped);
typedef BOOL(WINAPI *TdefOldReadFile)(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped);
typedef BOOL(WINAPI *TdefOldCloseHandle)(HANDLE hObject);

//Create Process
typedef BOOL(WINAPI *TdefOldCreateProcessA)(LPCTSTR lpApplicationName, LPTSTR lpCommandLine, LPSECURITY_ATTRIBUTES lpProcessAttributes, LPSECURITY_ATTRIBUTES lpThreadAttributes, BOOL bInheritHandles, DWORD dwCreationFlags, LPVOID lpEnvironment, LPCTSTR lpCurrentDirectory, LPSTARTUPINFO lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation);
typedef BOOL(WINAPI *TdefOldCreateProcessW)(LPCTSTR lpApplicationName, LPTSTR lpCommandLine, LPSECURITY_ATTRIBUTES lpProcessAttributes, LPSECURITY_ATTRIBUTES lpThreadAttributes, BOOL bInheritHandles, DWORD dwCreationFlags, LPVOID lpEnvironment, LPCTSTR lpCurrentDirectory, LPSTARTUPINFO lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation);

extern "C"
{
	// Search Files
	HANDLE WINAPI NewFindFirstFileA(LPCTSTR lpFileName, LPWIN32_FIND_DATA lpFindFileData);
	BOOL WINAPI NewFindNextFileA(HANDLE hFindFile, LPWIN32_FIND_DATA lpFindFileData);
	HANDLE WINAPI NewFindFirstFileW(LPCTSTR lpFileName, LPWIN32_FIND_DATA lpFindFileData);
	BOOL WINAPI NewFindNextFileW(HANDLE hFindFile, LPWIN32_FIND_DATA lpFindFileData);
	
	// Read/Write Files
	HANDLE WINAPI NewCreateFileA(LPCTSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
	HANDLE WINAPI NewCreateFileW(LPCTSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
	BOOL WINAPI NewWriteFile(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped);
	BOOL WINAPI NewReadFile(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped);
	BOOL WINAPI NewCloseHandle(HANDLE hObject);
	
	//Create Process
	BOOL WINAPI NewCreateProcessA(LPCTSTR lpApplicationName, LPTSTR lpCommandLine, LPSECURITY_ATTRIBUTES lpProcessAttributes, LPSECURITY_ATTRIBUTES lpThreadAttributes, BOOL bInheritHandles, DWORD dwCreationFlags, LPVOID lpEnvironment, LPCTSTR lpCurrentDirectory, LPSTARTUPINFO lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation);
	BOOL WINAPI NewCreateProcessW(LPCTSTR lpApplicationName, LPTSTR lpCommandLine, LPSECURITY_ATTRIBUTES lpProcessAttributes, LPSECURITY_ATTRIBUTES lpThreadAttributes, BOOL bInheritHandles, DWORD dwCreationFlags, LPVOID lpEnvironment, LPCTSTR lpCurrentDirectory, LPSTARTUPINFO lpStartupInfo, LPPROCESS_INFORMATION lpProcessInformation);

	LPSTR UnicodeToMByte(LPCWSTR unicodeStr);
}

// Search Files
extern "C" TdefOldFindFirstFileA OldFindFirstFileA;	//Multi-Char
extern "C" TdefOldFindNextFileA OldFindNextFileA;		//Multi-Char
extern "C" TdefOldFindFirstFileW OldFindFirstFileW;	//Unicode
extern "C" TdefOldFindNextFileW OldFindNextFileW;		//Unicode

// Create Process
extern "C" TdefOldCreateProcessA OldCreateProcessA;		//Multi-Char
extern "C" TdefOldCreateProcessW OldCreateProcessW;		//Unicode

// Read/Write Files
extern "C" TdefOldCreateFileA OldCreateFileA;
extern "C" TdefOldCreateFileW OldCreateFileW;
extern "C" TdefOldWriteFile OldWriteFile;
extern "C" TdefOldReadFile OldReadFile;
extern "C" TdefOldCloseHandle OldCloseHandle;
