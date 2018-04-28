#pragma once

#include <Windows.h>

// Search Files
typedef HANDLE(WINAPI *TdefOldFindFirstFileA)(LPCTSTR lpFileName, LPWIN32_FIND_DATA lpFindFileData);
typedef BOOL(WINAPI *TdefOldFindNextFileA)(HANDLE hFindFile, LPWIN32_FIND_DATA lpFindFileData);
typedef HANDLE(WINAPI *TdefOldFindFirstFileW)(LPCTSTR lpFileName, LPWIN32_FIND_DATA lpFindFileData);
typedef BOOL(WINAPI *TdefOldFindNextFileW)(HANDLE hFindFile, LPWIN32_FIND_DATA lpFindFileData);

typedef HANDLE(WINAPI *TdefOldCreateFileA)(LPCTSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
typedef HANDLE(WINAPI *TdefOldCreateFileW)(LPCTSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
typedef BOOL(WINAPI *TdefOldWriteFile)(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped);
typedef BOOL(WINAPI *TdefOldReadFile)(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped);
typedef BOOL(WINAPI *TdefOldCloseHandle)(HANDLE hObject);

extern "C" HANDLE WINAPI NewFindFirstFileA(LPCTSTR lpFileName, LPWIN32_FIND_DATA lpFindFileData);
extern "C" BOOL WINAPI NewFindNextFileA(HANDLE hFindFile, LPWIN32_FIND_DATA lpFindFileData);
extern "C" HANDLE WINAPI NewFindFirstFileW(LPCTSTR lpFileName, LPWIN32_FIND_DATA lpFindFileData);
extern "C" BOOL WINAPI NewFindNextFileW(HANDLE hFindFile, LPWIN32_FIND_DATA lpFindFileData);

extern "C" HANDLE WINAPI NewCreateFileA(LPCTSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
extern "C" HANDLE WINAPI NewCreateFileW(LPCTSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
extern "C" BOOL WINAPI NewWriteFile(HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped);
extern "C" BOOL WINAPI NewReadFile(HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped);
extern "C" BOOL WINAPI NewCloseHandle(HANDLE hObject);


// Seach Files
extern "C" TdefOldFindFirstFileA OldFindFirstFileA;	//Multi-Char
extern "C" TdefOldFindNextFileA OldFindNextFileA;		//Multi-Char
extern "C" TdefOldFindFirstFileW OldFindFirstFileW;	//Unicode
extern "C" TdefOldFindNextFileW OldFindNextFileW;		//Unicode

// Read/Write Files
extern "C" TdefOldCreateFileA OldCreateFileA;
extern "C" TdefOldCreateFileW OldCreateFileW;
extern "C" TdefOldWriteFile OldWriteFile;
extern "C" TdefOldReadFile OldReadFile;
extern "C" TdefOldCloseHandle OldCloseHandle;