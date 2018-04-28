#pragma once
#include <Windows.h>

typedef struct
{
	HANDLE CreateFileHandle;
	LPSTR lpFileName;
	DWORD dwDesiredAccess;
	DWORD dwCreationDisposition;
} CreateFileHANDLE;

extern int maxArraySize;
extern int currentIndex;

extern CreateFileHANDLE* fileHandleArray;

extern "C" void FileHandleInitialize();
extern "C" void FileHandleAdd(CreateFileHANDLE fileHandle);
extern "C" void FileHandleDeleteElement(HANDLE fileHandle);
extern "C" int FileHandleFindElement(HANDLE fileHandle);