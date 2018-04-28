#pragma once
#include "SocketConnection.h"
#include "MainForm.h"

#include <windows.h>
#include <string>

using namespace System;
using namespace System::Windows::Forms;
using namespace AnomalyDetection;
using namespace System::Threading;

#define DESCRIPTOR_SPACE 10 * sizeof(DWORD);
#define DLL_NAME "Hook_x86.dll";
//#define DLL_NAME "Message.dll";
#define DLL_FUNC_NAME "_DllMain@12";
//#define DLL_FUNC_NAME "HookMain";

const DWORD SectionSize = 1000;
const int FreeSpace = 8;	// Free space from the begining of section (numbrer of dwords)
char* SectionName = ".Hook";

//ref struct Globals
//{
//	WinsockConnection^ w = gcnew WinsockConnection(MainForm^ form);
//};

DWORD align(DWORD size, DWORD align, DWORD addr);
DWORD Rva2Offset(DWORD rva, PIMAGE_SECTION_HEADER psh, PIMAGE_NT_HEADERS pnt);
bool AddSection(HANDLE file, char *sectionName, DWORD sizeOfSection, DWORD fileSize, BYTE *pByte);
bool InjectDll(MainForm^ s, char *filepath, char *sectionName, DWORD sizeOfSection);
char* SystemStringToCharPointer(String^ string);
BOOL CheckSectionExist(char* filepath);
void OpenConnection(MainForm^ form);
