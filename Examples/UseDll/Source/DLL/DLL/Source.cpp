#include "Header.h"
#include <windows.h>

void createFileFunc()
{
	HANDLE hFile = CreateFile(L"FILE_FROM_DLL.txt", GENERIC_EXECUTE | GENERIC_ALL,
		FILE_SHARE_READ, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, NULL);
	CloseHandle(hFile);
}
