#include <Windows.h>

typedef void(WINAPI *DllFunction)();

char* dllName = "DLL.dll";
char* funcName = "createFileFunc";

int main()
{
	HINSTANCE hinstDLL = LoadLibrary(dllName);
	if (hinstDLL == NULL)
	{
		return 0;
	}
	DllFunction MsgBox = (DllFunction)GetProcAddress(hinstDLL, funcName);
	if (MsgBox != NULL)
	{
		MsgBox();
		MessageBox(0, "I am good file", "Good file", MB_OK);
	}
	else MessageBox(0, "Dll file not found", "dll not found", MB_OK);
	FreeLibrary(hinstDLL);
	return 0;
}