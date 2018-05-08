#pragma once

typedef struct 
{
	CHAR *dll;
	CHAR *name;
	LPVOID proxy;
	LPVOID original;
	DWORD length;
} HOOK_ARRAY;

char* ip = "127.0.0.1";
int port = 35000;

BOOL HookFunction(CHAR *dll, CHAR *name, LPVOID proxy, LPVOID original, PDWORD length);
void HookAll();
void SafeMemcpyPadded(LPVOID destination, LPVOID source, DWORD size);
BOOL OpenConnection();
void CloseConnection();