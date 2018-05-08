#include "Hook.h"
#include <stdexcept>

#define DESCRIPTOR_SPACE 10 * sizeof(DWORD);
#define DLL_FUNC_NAME "_DllMain@12";
const int MAX_ALLOWED_SEC_NAME = 8;

const int FreeSpace = 8;	// Free space from the begining of section (numbrer of dwords)
char* SectionName = ".Hook";
char* DLL_NAME = "Hook_x86.dll";
const DWORD SectionSize = 500;

bool CheckSectionExist(char* filepath)
{
	HANDLE file = CreateFile(filepath, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (file == INVALID_HANDLE_VALUE)
		throw std::exception("(*) Error open source file.\n");

	DWORD fileSize = GetFileSize(file, NULL);
	BYTE *pByte = new BYTE[fileSize];
	DWORD dw;
	
	ReadFile(file, pByte, fileSize, &dw, NULL);
	PIMAGE_DOS_HEADER dos = (PIMAGE_DOS_HEADER)pByte;
	if (dos->e_magic != IMAGE_DOS_SIGNATURE)
		throw std::exception("(*) Invalid PE file. Use only *.exe files.\n");

	PIMAGE_FILE_HEADER FH = (PIMAGE_FILE_HEADER)(pByte + dos->e_lfanew + sizeof(DWORD));
	PIMAGE_SECTION_HEADER SH = (PIMAGE_SECTION_HEADER)(pByte + dos->e_lfanew + sizeof(IMAGE_NT_HEADERS));
	if (strcmp((char*)(SH[FH->NumberOfSections - 1].Name), SectionName) == 0)
	{
		CloseHandle(file);
		return true;
	}
	else { CloseHandle(file); return false; }
}
DWORD align(DWORD size, DWORD align, DWORD addr) {
	if (!(size % align))
		return addr + size;
	return addr + (size / align + 1) * align;
}

// Convert Virtual Address to File Offset
DWORD Rva2Offset(DWORD rva, PIMAGE_SECTION_HEADER psh, PIMAGE_NT_HEADERS pnt)
{
	size_t i = 0;
	PIMAGE_SECTION_HEADER pSeh;
	if (rva == 0)
		return (rva);
	pSeh = psh;
	for (i = 0; i < pnt->FileHeader.NumberOfSections; i++)
	{
		if (rva >= pSeh->VirtualAddress && rva < pSeh->VirtualAddress + pSeh->Misc.VirtualSize)
			break;
		pSeh++;
	}
	return (rva - pSeh->VirtualAddress + pSeh->PointerToRawData);
}

void AddSection(HANDLE file, char *sectionName, DWORD sizeOfSection, DWORD fileSize, BYTE *pByte)
{
	PIMAGE_DOS_HEADER dos = (PIMAGE_DOS_HEADER)pByte;
	if (dos->e_magic != IMAGE_DOS_SIGNATURE)
		throw std::exception("(*) Invalid PE file. Use only *.exe files.\n");
	
	PIMAGE_FILE_HEADER FH = (PIMAGE_FILE_HEADER)(pByte + dos->e_lfanew + sizeof(DWORD));
	PIMAGE_OPTIONAL_HEADER OH = (PIMAGE_OPTIONAL_HEADER)(pByte + dos->e_lfanew + sizeof(DWORD) + sizeof(IMAGE_FILE_HEADER));
	PIMAGE_SECTION_HEADER SH = (PIMAGE_SECTION_HEADER)(pByte + dos->e_lfanew + sizeof(IMAGE_NT_HEADERS));

	ZeroMemory(&SH[FH->NumberOfSections], sizeof(IMAGE_SECTION_HEADER));
	CopyMemory(&SH[FH->NumberOfSections].Name, sectionName, MAX_ALLOWED_SEC_NAME);
	//We use 8 bytes for section name,cause it is the maximum allowed section name size

	//lets insert all the required information about our new PE section
	SH[FH->NumberOfSections].Misc.VirtualSize = align(sizeOfSection, OH->SectionAlignment, 0);
	SH[FH->NumberOfSections].VirtualAddress = align(SH[FH->NumberOfSections - 1].Misc.VirtualSize, OH->SectionAlignment, SH[FH->NumberOfSections - 1].VirtualAddress);
	SH[FH->NumberOfSections].SizeOfRawData = align(sizeOfSection, OH->FileAlignment, 0);
	SH[FH->NumberOfSections].PointerToRawData = align(SH[FH->NumberOfSections - 1].SizeOfRawData, OH->FileAlignment, SH[FH->NumberOfSections - 1].PointerToRawData);
	SH[FH->NumberOfSections].Characteristics = 0xE00000E0;
	/*
	0xE00000E0 = IMAGE_SCN_MEM_WRITE |
	IMAGE_SCN_CNT_CODE  |
	IMAGE_SCN_CNT_UNINITIALIZED_DATA  |
	IMAGE_SCN_MEM_EXECUTE |
	IMAGE_SCN_CNT_INITIALIZED_DATA |
	IMAGE_SCN_MEM_READ
	*/
	SetFilePointer(file, SH[FH->NumberOfSections].PointerToRawData + SH[FH->NumberOfSections].SizeOfRawData, NULL, FILE_BEGIN);
	//end the file right here,on the last section + it's own size
	SetEndOfFile(file);
	//now lets change the size of the image,to correspond to our modifications
	//by adding a new section,the image size is bigger now
	OH->SizeOfImage = SH[FH->NumberOfSections].VirtualAddress + SH[FH->NumberOfSections].Misc.VirtualSize;
	//and we added a new section,so we change the NOS too
	FH->NumberOfSections += 1;

	//disable the ASLR
	if ((OH->DllCharacteristics & 0x40) == 0x40)
		OH->DllCharacteristics = OH->DllCharacteristics ^ 0x40;
	//disable the DEP
	if ((OH->DllCharacteristics & 0x100) == 0x100)
		OH->DllCharacteristics = OH->DllCharacteristics ^ 0x100;

	SetFilePointer(file, 0, NULL, FILE_BEGIN);
	//and finaly,we add all the modifications to the file
	DWORD dw;
	WriteFile(file, pByte, fileSize, &dw, NULL);

}

bool InjectDll(MainForm^ s, char *filepath, char *sectionName, DWORD sizeOfSection)
{
	HANDLE file = CreateFile(filepath, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (file == INVALID_HANDLE_VALUE)
		throw std::exception("(*) Error open source file.\n");

	DWORD fileSize = GetFileSize(file, NULL);
	BYTE *pByte = new BYTE[fileSize];
	DWORD dw;
	//lets read the entire file,so we can use the PE information
	ReadFile(file, pByte, fileSize, &dw, NULL);

	// Try to create a new section
	try
	{
		AddSection(file, sectionName, sizeOfSection, fileSize, pByte);
	}
	catch (...)
	{
		throw;
	}

	PIMAGE_DOS_HEADER dos = (PIMAGE_DOS_HEADER)pByte;
	//Import Directory RVA
	PIMAGE_NT_HEADERS nt = (PIMAGE_NT_HEADERS)(pByte + dos->e_lfanew);
	PIMAGE_SECTION_HEADER first = IMAGE_FIRST_SECTION(nt);
	PIMAGE_IMPORT_DESCRIPTOR image_import_descriptor;
	image_import_descriptor = (PIMAGE_IMPORT_DESCRIPTOR)((DWORD_PTR)pByte + \
		Rva2Offset(nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress, first, nt));
	PIMAGE_SECTION_HEADER last = first + (nt->FileHeader.NumberOfSections - 1);

	// Change Import directory entry point to ours
	nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress = last->VirtualAddress + FreeSpace * sizeof(DWORD);//last->VirtualAddress
	SetFilePointer(file, 0, NULL, FILE_BEGIN);
	//add the modifications to the file
	WriteFile(file, pByte, fileSize, &dw, NULL);

	DWORD z = 0;
	SetFilePointer(file, last->PointerToRawData, NULL, FILE_BEGIN);
	for (int i = 0; i < FreeSpace; i++)
		WriteFile(file, (LPCVOID*)&z, sizeof(DWORD), &dw, 0);

	// Copy IMAGE_IMPORT_DESCRIPTOR to a new sector
	int word_num = FreeSpace;
	while (image_import_descriptor->Name != NULL)
	{
		WriteFile(file, (LPCVOID*)&(image_import_descriptor)->Characteristics, sizeof(DWORD), &dw, 0);
		WriteFile(file, (LPCVOID*)&(image_import_descriptor)->TimeDateStamp, sizeof(DWORD), &dw, 0);
		WriteFile(file, (LPCVOID*)&(image_import_descriptor)->ForwarderChain, sizeof(DWORD), &dw, 0);
		WriteFile(file, (LPCVOID*)&(image_import_descriptor)->Name, sizeof(DWORD), &dw, 0);
		WriteFile(file, (LPCVOID*)&(image_import_descriptor)->FirstThunk, sizeof(DWORD), &dw, 0);
		image_import_descriptor++; //advance to next IMAGE_IMPORT_DESCRIPTOR
		word_num += 5;
	}

	char* dll_name = DLL_NAME;
	char* dll_func = DLL_FUNC_NAME;
	DWORD NewImageImportDescriptor = last->VirtualAddress + word_num * sizeof(DWORD);
	DWORD DllName = NewImageImportDescriptor + DESCRIPTOR_SPACE;
	DWORD DllFuncName = DllName + strlen(dll_name) + 1;
	DWORD DllFuncAddress = DllFuncName + strlen(dll_func) + 2 + 1;
	DWORD Address = DllFuncAddress + 2 * sizeof(DWORD);
	DWORD zero = 0;
	DWORD AddressValue = 0x10001020;

	// Create a new descriptor for our dll
	WriteFile(file, (LPCVOID*)&DllFuncAddress, sizeof(DWORD), &dw, 0);
	WriteFile(file, (LPCVOID*)&zero, sizeof(DWORD), &dw, 0);
	WriteFile(file, (LPCVOID*)&zero, sizeof(DWORD), &dw, 0);
	WriteFile(file, (LPCVOID*)&DllName, sizeof(DWORD), &dw, 0);
	WriteFile(file, (LPCVOID*)&Address, sizeof(DWORD), &dw, 0);
	WriteFile(file, (LPCVOID*)&zero, sizeof(DWORD), &dw, 0);
	WriteFile(file, (LPCVOID*)&zero, sizeof(DWORD), &dw, 0);
	WriteFile(file, (LPCVOID*)&zero, sizeof(DWORD), &dw, 0);
	WriteFile(file, (LPCVOID*)&zero, sizeof(DWORD), &dw, 0);
	WriteFile(file, (LPCVOID*)&zero, sizeof(DWORD), &dw, 0);
	WriteFile(file, dll_name, strlen(dll_name), &dw, 0);
	WriteFile(file, (LPCVOID*)&zero, 3, &dw, 0);
	WriteFile(file, dll_func, strlen(dll_func), &dw, 0);
	WriteFile(file, (LPCVOID*)&zero, 1, &dw, 0);
	WriteFile(file, (LPCVOID*)&DllFuncName, sizeof(DWORD), &dw, 0);
	WriteFile(file, (LPCVOID*)&zero, sizeof(DWORD), &dw, 0);
	WriteFile(file, (LPCVOID*)&AddressValue, sizeof(DWORD), &dw, 0);

	CloseHandle(file);
	return true;
}