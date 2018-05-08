#pragma once
#include <Windows.h>

typedef struct
{
	HANDLE CreateFileHandle;
	LPSTR lpFileName;
	DWORD dwDesiredAccess;
	DWORD dwCreationDisposition;
	LPVOID lpBuffer;
	bool isRead;
	bool isWritten;
} CreateFileHANDLE;

extern int maxArraySize;
extern int currentIndex;
extern CreateFileHANDLE* fileHandleArray;

extern "C"
{
	void FileHandleInitialize();
	void FileHandleAdd(CreateFileHANDLE fileHandle);
	void FileHandleDeleteElement(HANDLE fileHandle);
	int FileHandleFindElement(HANDLE fileHandle);
	void FileHandleMarkAsRead(HANDLE fileHandle);
	void FileHandleMarkAsWritten(HANDLE fileHandle);
	void FileHandleAddBuffer(HANDLE fileHandle, LPVOID lpBuffer);
	int FileHandleFindFileByBuffer(LPVOID lpBuffer);
}