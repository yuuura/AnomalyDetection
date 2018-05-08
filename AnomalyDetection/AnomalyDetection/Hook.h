#pragma once
#include <Windows.h>
#include "MainForm.h"

using namespace AnomalyDetection;

extern char* SectionName;
extern const DWORD SectionSize;
extern char* DLL_NAME;

DWORD align(DWORD size, DWORD align, DWORD addr);
DWORD Rva2Offset(DWORD rva, PIMAGE_SECTION_HEADER psh, PIMAGE_NT_HEADERS pnt);
void AddSection(HANDLE file, char *sectionName, DWORD sizeOfSection, DWORD fileSize, BYTE *pByte);
extern "C"
{
	bool InjectDll(MainForm^ s, char *filepath, char *sectionName, DWORD sizeOfSection);
	bool CheckSectionExist(char* filepath);
}