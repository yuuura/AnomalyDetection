#include "FileHandle.h"

using namespace std;

int maxArraySize = 1;
int currentIndex = 0;
CreateFileHANDLE* fileHandleArray;

//Initialize array
void FileHandleInitialize()
{
	fileHandleArray = new CreateFileHANDLE[maxArraySize];

	for (int i = 0; i < maxArraySize; i++)
	{
		fileHandleArray[i].CreateFileHandle = NULL;
		fileHandleArray[i].dwDesiredAccess = 0;
		fileHandleArray[i].lpFileName = "";
	}
}

// Adds element if not exist
void FileHandleAdd(CreateFileHANDLE fileHandle)
{
	// extend array if needed
	if (currentIndex == maxArraySize)
	{
		CreateFileHANDLE* tmpFileHandleArray = new CreateFileHANDLE[maxArraySize * 2];
		for (int i = 0; i < maxArraySize; i++)
			tmpFileHandleArray[i] = fileHandleArray[i];
		
		delete[] fileHandleArray;
		fileHandleArray = tmpFileHandleArray;
		maxArraySize *= 2;
	}

	//check if exist
	if (FileHandleFindElement(fileHandle.CreateFileHandle) < 0)
	{
		fileHandleArray[currentIndex].CreateFileHandle = fileHandle.CreateFileHandle;
		fileHandleArray[currentIndex].lpFileName = fileHandle.lpFileName;
		fileHandleArray[currentIndex++].dwDesiredAccess = fileHandle.dwDesiredAccess;
	}
}

//Delete element and shift an array
void FileHandleDeleteElement(HANDLE fileHandle)
{
	int i = FileHandleFindElement(fileHandle);
	for (; i < maxArraySize - 1; i++)
		fileHandleArray[i] = fileHandleArray[i + 1];
	currentIndex--;
}

//Find element and return index position or -1 if not found
int FileHandleFindElement(HANDLE fileHandle)
{
	int index = 0;
	bool end = false;
	while (index <= (maxArraySize-1) && !end)
	{
		if (fileHandleArray[index].CreateFileHandle == fileHandle)
		{
			return index;
			end = true;
		}
		else index++;
	}
	return -1;
}