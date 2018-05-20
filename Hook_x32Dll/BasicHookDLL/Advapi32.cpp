#define _CRT_SECURE_NO_WARNINGS

#include "Advapi32.h"
#include "Ws2_32.h"
#include "FileHandle.h"
#include "Kernel32.h"
#include <atlstr.h>

#define SLEEP_TIME 10
#define BUFFER_SIZE 512

// Cryptography
TdefOldCryptAcquireContextA OldCryptAcquireContextA;	//Multi-Char
TdefOldCryptAcquireContextW OldCryptAcquireContextW;	//Unicode
TdefOldCryptHashData OldCryptHashData;
TdefOldCryptEncrypt OldCryptEncrypt;
// Services
TdefOldOpenSCManagerA OldOpenSCManagerA;				//Multi-Char
TdefOldOpenSCManagerW OldOpenSCManagerW;				//Unicode
TdefOldOpenServiceA OldOpenServiceA;					//Multi-Char
TdefOldOpenServiceW OldOpenServiceW;					//Unicode
TdefOldControlService OldControlService;

SC_HANDLE srvHandle = NULL;
LPCTSTR srvName = NULL;

BOOL WINAPI NewControlService(SC_HANDLE hService, DWORD dwControl, LPSERVICE_STATUS lpServiceStatus)
{
	char buffer[BUFFER_SIZE] = "ControlService ";
	strcat(buffer, "\n\nSending control code to a service...");
	
	if (srvHandle == hService)
	{
		strcat(buffer, "\nService: ");
		strcat(buffer, srvName);
	}

	strcat(buffer, "\nAction: ");
	switch (dwControl)
	{
	case SERVICE_CONTROL_CONTINUE: strcat(buffer, "\"Resume service from pause.\""); break;
	case SERVICE_CONTROL_INTERROGATE: strcat(buffer, "\"It should report its current status information to the service control manager.\""); break;
	case SERVICE_CONTROL_NETBINDADD: strcat(buffer, "\"Notifies a network service that there is a new component for binding.\""); break;
	case SERVICE_CONTROL_NETBINDDISABLE: strcat(buffer, "\"Notifies a network service that one of its bindings has been disabled.\""); break;
	case SERVICE_CONTROL_NETBINDENABLE: strcat(buffer, "\"Notifies a network service that a disabled binding has been enabled.\""); break;
	case SERVICE_CONTROL_NETBINDREMOVE: strcat(buffer, "\"Notifies a network service that a component for binding has been removed.\""); break;
	case SERVICE_CONTROL_PARAMCHANGE: strcat(buffer, "\"Notifies a service that its startup parameters have changed.\""); break;
	case SERVICE_CONTROL_PAUSE: strcat(buffer, "\"Pause the service.\""); break;
	case SERVICE_CONTROL_STOP: strcat(buffer, "\"Stop the service.\""); break;
	}

	BOOL OldControlServiceResult = OldControlService(hService, dwControl, lpServiceStatus);
	if (OldControlServiceResult)
		strcat(buffer, "\nAction service performed.");
	else strcat(buffer, "\nAction service failed.");

	send(Connection, buffer, sizeof(buffer), NULL);
	Sleep(SLEEP_TIME);
	return OldControlServiceResult;
}

SC_HANDLE WINAPI NewOpenServiceA(SC_HANDLE hSCManager, LPCTSTR lpServiceName, DWORD dwDesiredAccess)
{
	char buffer[BUFFER_SIZE] = "OpenService ";
	strcat(buffer, "\n\nOpening service...");
	strcat(buffer, "\nService name: ");
	strcat(buffer, (const char*)lpServiceName);
	strcat(buffer, "\nDesired access: ");

	if ((dwDesiredAccess & SERVICE_ALL_ACCESS) == SERVICE_ALL_ACCESS) strcat(buffer, "\"All access rights.\"");
	if ((dwDesiredAccess & SERVICE_CHANGE_CONFIG) == SERVICE_CHANGE_CONFIG) strcat(buffer, "\"Change the service configuration.\"");
	if ((dwDesiredAccess & SERVICE_ENUMERATE_DEPENDENTS) == SERVICE_ENUMERATE_DEPENDENTS) strcat(buffer, "\"Enumerate all the services dependent on the service.\"");
	if ((dwDesiredAccess & SERVICE_INTERROGATE) == SERVICE_INTERROGATE) strcat(buffer, "\"Ask the service to report its status immediately.\"");
	if ((dwDesiredAccess & SERVICE_PAUSE_CONTINUE) == SERVICE_PAUSE_CONTINUE) strcat(buffer, "\"Pause or continue the service.\"");
	if ((dwDesiredAccess & SERVICE_QUERY_CONFIG) == SERVICE_QUERY_CONFIG) strcat(buffer, "\"Query the service configuration.\"");
	if ((dwDesiredAccess & SERVICE_QUERY_STATUS) == SERVICE_QUERY_STATUS) strcat(buffer, "\"Ask the service control manager about the status of the service and receive notification when a service changes status.\"");
	if ((dwDesiredAccess & SERVICE_START) == SERVICE_START) strcat(buffer, "\"Start the service.\"");
	if ((dwDesiredAccess & SERVICE_STOP) == SERVICE_STOP) strcat(buffer, "\"Stop the service.\"");
	if ((dwDesiredAccess & SERVICE_USER_DEFINED_CONTROL) == SERVICE_USER_DEFINED_CONTROL) strcat(buffer, "\"Specify a user-defined control code.\"");

	SC_HANDLE OldOpenServiceAResult = OldOpenServiceA(hSCManager, lpServiceName, dwDesiredAccess);
	srvHandle = OldOpenServiceAResult;
	srvName = (const char*)lpServiceName;
	if (OldOpenServiceAResult != NULL)
		strcat(buffer, "\nOpen service succeeded.");
	else strcat(buffer, "\nOpen service failed.");

	send(Connection, buffer, sizeof(buffer), NULL);
	Sleep(SLEEP_TIME);
	return OldOpenServiceAResult;
}

SC_HANDLE WINAPI NewOpenServiceW(SC_HANDLE hSCManager, LPCWSTR lpServiceName, DWORD dwDesiredAccess)
{
	char buffer[BUFFER_SIZE] = "OpenService ";
	strcat(buffer, "\n\nOpening service...");
	strcat(buffer, "\nService name: ");
	strcat(buffer, UnicodeToMByte(lpServiceName));
	strcat(buffer, "\nDesired access: ");

	if ((dwDesiredAccess & SERVICE_ALL_ACCESS) == SERVICE_ALL_ACCESS) strcat(buffer, "\"All access rights.\"");
	if ((dwDesiredAccess & SERVICE_CHANGE_CONFIG) == SERVICE_CHANGE_CONFIG) strcat(buffer, "\"Change the service configuration.\"");
	if ((dwDesiredAccess & SERVICE_ENUMERATE_DEPENDENTS) == SERVICE_ENUMERATE_DEPENDENTS) strcat(buffer, "\"Enumerate all the services dependent on the service.\"");
	if ((dwDesiredAccess & SERVICE_INTERROGATE) == SERVICE_INTERROGATE) strcat(buffer, "\"Ask the service to report its status immediately.\"");
	if ((dwDesiredAccess & SERVICE_PAUSE_CONTINUE) == SERVICE_PAUSE_CONTINUE) strcat(buffer, "\"Pause or continue the service.\"");
	if ((dwDesiredAccess & SERVICE_QUERY_CONFIG) == SERVICE_QUERY_CONFIG) strcat(buffer, "\"Query the service configuration.\"");
	if ((dwDesiredAccess & SERVICE_QUERY_STATUS) == SERVICE_QUERY_STATUS) strcat(buffer, "\"Ask the service control manager about the status of the service and receive notification when a service changes status.\"");
	if ((dwDesiredAccess & SERVICE_START) == SERVICE_START) strcat(buffer, "\"Start the service.\"");
	if ((dwDesiredAccess & SERVICE_STOP) == SERVICE_STOP) strcat(buffer, "\"Stop the service.\"");
	if ((dwDesiredAccess & SERVICE_USER_DEFINED_CONTROL) == SERVICE_USER_DEFINED_CONTROL) strcat(buffer, "\"Specify a user-defined control code.\"");

	SC_HANDLE OldOpenServiceWResult = OldOpenServiceW(hSCManager, lpServiceName, dwDesiredAccess);
	srvHandle = OldOpenServiceWResult;
	srvName = UnicodeToMByte(lpServiceName);
	if (OldOpenServiceWResult != NULL)
		strcat(buffer, "\nOpen service succeeded.");
	else strcat(buffer, "\n Open service failed.");
	
	send(Connection, buffer, sizeof(buffer), NULL);
	Sleep(SLEEP_TIME);
	return OldOpenServiceWResult;
}

SC_HANDLE WINAPI NewOpenSCManagerA(LPCTSTR lpMachineName, LPCTSTR lpDatabaseName, DWORD dwDesiredAccess)
{
	char buffer[BUFFER_SIZE] = "OpenSCManager ";
	strcat(buffer, "\n\nEstablishing a connection to the service control manager...");
	if (lpMachineName != NULL)
	{
		strcat(buffer, "\nTarget machine: ");
		strcat(buffer, (const char*)lpMachineName);
	}
	else
		strcat(buffer, "\nTarget machine: Local machine.");
	
	strcat(buffer, "\nDesired access: ");
	switch (dwDesiredAccess)
	{
	case SC_MANAGER_ALL_ACCESS: strcat(buffer, "\"All access reights.\""); break;
	case SC_MANAGER_CREATE_SERVICE: strcat(buffer, "\"Create a service object and add it to the database.\""); break;
	case SC_MANAGER_CONNECT: strcat(buffer, "\"Connect to the service control manager\""); break;
	case SC_MANAGER_ENUMERATE_SERVICE: strcat(buffer, "\"List the services that are in the database and receive notification when any service is created or deleted.\""); break;
	case SC_MANAGER_LOCK: strcat(buffer, "\"Acquire a lock on the database.\""); break;
	case SC_MANAGER_MODIFY_BOOT_CONFIG: strcat(buffer, "\"Call the NotifyBootConfigStatus function.\""); break;
	case SC_MANAGER_QUERY_LOCK_STATUS: strcat(buffer, "\"Retrieve the lock status information for the database.\""); break;
	}

	SC_HANDLE OldOpenSCManagerAResult = OldOpenSCManagerA(lpMachineName, lpDatabaseName, dwDesiredAccess);
	if (OldOpenSCManagerAResult != NULL)
		strcat(buffer, "\nConnection to service control manager succeeded.");
	else strcat(buffer, "\nConnection to service control manager failed.");
	
	send(Connection, buffer, sizeof(buffer), NULL);
	Sleep(SLEEP_TIME);
	return OldOpenSCManagerAResult;
}

SC_HANDLE WINAPI NewOpenSCManagerW(LPCTSTR lpMachineName, LPCTSTR lpDatabaseName, DWORD dwDesiredAccess)
{
	char buffer[BUFFER_SIZE] = "OpenSCManager ";
	strcat(buffer, "\n\nEstablishing a connection to the service control manager...");
	if (lpMachineName != NULL)
	{
		strcat(buffer, "\nTarget machine: ");
		strcat(buffer, UnicodeToMByte((LPCWSTR)lpMachineName));
	}
	else
		strcat(buffer, "\nTarget machine: Local machine.");
	
	strcat(buffer, "\nDesired access: ");
	switch (dwDesiredAccess)
	{
	case SC_MANAGER_ALL_ACCESS: strcat(buffer, "\"All access reights.\"");
	case SC_MANAGER_CREATE_SERVICE: strcat(buffer, "\"Create a service object and add it to the database.\"");
	case SC_MANAGER_CONNECT: strcat(buffer, "\"Connect to the service control manager\"");
	case SC_MANAGER_ENUMERATE_SERVICE: strcat(buffer, "\"List the services that are in the database and receive notification when any service is created or deleted.\"");
	case SC_MANAGER_LOCK: strcat(buffer, "\"Acquire a lock on the database.\"");
	case SC_MANAGER_MODIFY_BOOT_CONFIG: strcat(buffer, "\"Call the NotifyBootConfigStatus function.\"");
	case SC_MANAGER_QUERY_LOCK_STATUS: strcat(buffer, "\"Retrieve the lock status information for the database.\"");
	}

	SC_HANDLE OldOpenSCManagerWResult = OldOpenSCManagerW(lpMachineName, lpDatabaseName, dwDesiredAccess);
	/*if (!OldOpenSCManagerWResult)
		strcat(buffer, "\nConnection to service control manager succeeded.");
	else strcat(buffer, "\nConnection to service control manager failed.");*/
	
	send(Connection, buffer, sizeof(buffer), NULL);
	Sleep(SLEEP_TIME);
	return OldOpenSCManagerWResult;
}

BOOL WINAPI NewCryptHashData(HCRYPTHASH hHash, BYTE *pbData, DWORD dwDataLen, DWORD dwFlags)
{
	char buffer[BUFFER_SIZE] = "CryptHashData ";
	strcat(buffer, "\n\nHash data: ");
	CString s = CString((const char*)pbData);
	strcat(buffer, s);

	BOOL OldCryptHashDataResult = OldCryptHashData(hHash, pbData, dwDataLen, dwFlags);
	if (OldCryptHashDataResult)
		strcat(buffer, "\nHashing data succeeds.");
	else strcat(buffer, "\nHashing data fails.");

	send(Connection, buffer, sizeof(buffer), NULL);
	Sleep(SLEEP_TIME);
	return OldCryptHashDataResult;
}

BOOL WINAPI NewCryptAcquireContextA(HCRYPTPROV *phProv, LPCTSTR pszContainer, LPCTSTR pszProvider, DWORD dwProvType, DWORD dwFlags)
{
	char buffer[BUFFER_SIZE] = "CryptAcquireContext ";
	strcat(buffer, "\n\nCryptography service provider: ");
	CString s = CString((const char*)pszProvider);
	strcat(buffer, s);
	strcat(buffer, "\nProvider type: ");
	switch (dwProvType)
	{
	case PROV_RSA_FULL: strcat(buffer, "PROV_RSA_FULL"); break;
	case PROV_RSA_AES: strcat(buffer, "PROV_RSA_AES"); break;
	case PROV_RSA_SIG: strcat(buffer, "PROV_RSA_SIG"); break;
	case PROV_RSA_SCHANNEL: strcat(buffer, "PROV_RSA_SCHANNEL"); break;
	case PROV_DSS: strcat(buffer, "PROV_DSS"); break;
	case PROV_DSS_DH: strcat(buffer, "PROV_DSS_DH"); break;
	case PROV_DH_SCHANNEL: strcat(buffer, "PROV_DH_SCHANNEL"); break;
	case PROV_FORTEZZA: strcat(buffer, "PROV_FORTEZZA"); break;
	case PROV_MS_EXCHANGE: strcat(buffer, "PROV_MS_EXCHANGE"); break;
	case PROV_SSL: strcat(buffer, "PROV_SSL"); break;
	}

	BOOL OldCryptAcquireContextAResult = OldCryptAcquireContextA(phProv, pszContainer, pszProvider, dwProvType, dwFlags);
	if (OldCryptAcquireContextAResult)
		strcat(buffer, "\nRequiring Cryptography service provider succeeds.");
	else strcat(buffer, "\nRequiring Cryptography service provider fails.");

	send(Connection, buffer, sizeof(buffer), NULL);
	Sleep(SLEEP_TIME);
	return OldCryptAcquireContextAResult;
}

BOOL WINAPI NewCryptAcquireContextW(HCRYPTPROV *phProv, LPCTSTR pszContainer, LPCTSTR pszProvider, DWORD dwProvType, DWORD dwFlags)
{
	char buffer[BUFFER_SIZE] = "CryptAcquireContext ";
	strcat(buffer, "\n\nCryptography service provider: ");
	strcat(buffer, UnicodeToMByte((LPCWSTR)pszProvider));
	strcat(buffer, "\nProvider type: ");

	switch (dwProvType)
	{
	case PROV_RSA_FULL: strcat(buffer, "PROV_RSA_FULL"); break;
	case PROV_RSA_AES: strcat(buffer, "PROV_RSA_AES"); break;
	case PROV_RSA_SIG: strcat(buffer, "PROV_RSA_SIG"); break;
	case PROV_RSA_SCHANNEL: strcat(buffer, "PROV_RSA_SCHANNEL"); break;
	case PROV_DSS: strcat(buffer, "PROV_DSS"); break;
	case PROV_DSS_DH: strcat(buffer, "PROV_DSS_DH"); break;
	case PROV_DH_SCHANNEL: strcat(buffer, "PROV_DH_SCHANNEL"); break;
	case PROV_FORTEZZA: strcat(buffer, "PROV_FORTEZZA"); break;
	case PROV_MS_EXCHANGE: strcat(buffer, "PROV_MS_EXCHANGE"); break;
	case PROV_SSL: strcat(buffer, "PROV_SSL"); break;
	}

	BOOL OldCryptAcquireContextWResult = OldCryptAcquireContextW(phProv, pszContainer, UnicodeToMByte((LPCWSTR)pszProvider), dwProvType, dwFlags);
	if (OldCryptAcquireContextWResult)
		strcat(buffer, "\nRequiring Cryptography service provider succeeds.");
	else strcat(buffer, "\nRequiring Cryptography service provider fails.");

	send(Connection, buffer, sizeof(buffer), NULL);
	Sleep(SLEEP_TIME);
	return OldCryptAcquireContextWResult;
}
BOOL WINAPI NewCryptEncrypt(HCRYPTKEY hKey, HCRYPTHASH hHash, BOOL Final, DWORD dwFlags, BYTE *pbData, DWORD *pdwDataLen, DWORD dwBufLen)
{
	// To avoid loop we check if this is last encrypted buffer without checking bytes before
	if (Final)
	{
		char buffer[BUFFER_SIZE] = "CryptEncrypt ";
		strcat(buffer, "\n\nEncrypting data.");
		int index = FileHandleFindFileByBuffer((LPVOID)pbData);
		if (index >= 0)
		{
			strcat(buffer, "\nEncrypt File: ");
			strcat(buffer, fileHandleArray[index].lpFileName);
		}
		send(Connection, buffer, sizeof(buffer), NULL);
		Sleep(SLEEP_TIME);
	}

	BOOL OldCryptEncryptResult = OldCryptEncrypt(hKey, hHash, Final, dwFlags, pbData, pdwDataLen, dwBufLen);
	return OldCryptEncryptResult;
}