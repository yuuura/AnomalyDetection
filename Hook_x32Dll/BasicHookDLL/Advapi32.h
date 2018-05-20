#pragma once

#include <Windows.h>

// Cryptography
typedef BOOL(WINAPI *TdefOldCryptAcquireContextA)(HCRYPTPROV *phProv, LPCTSTR pszContainer, LPCTSTR pszProvider, DWORD dwProvType, DWORD dwFlags);
typedef BOOL(WINAPI *TdefOldCryptAcquireContextW)(HCRYPTPROV *phProv, LPCTSTR pszContainer, LPCTSTR pszProvider, DWORD dwProvType, DWORD dwFlags);
typedef BOOL(WINAPI *TdefOldCryptHashData)(HCRYPTHASH hHash, BYTE *pbData, DWORD dwDataLen, DWORD dwFlags);
typedef BOOL(WINAPI *TdefOldCryptEncrypt)(HCRYPTKEY hKey, HCRYPTHASH hHash, BOOL Final, DWORD dwFlags, BYTE *pbData, DWORD *pdwDataLen, DWORD dwBufLen);

//Services
typedef SC_HANDLE(WINAPI *TdefOldOpenSCManagerA)(LPCTSTR lpMachineName, LPCTSTR lpDatabaseName, DWORD dwDesiredAccess);
typedef SC_HANDLE(WINAPI *TdefOldOpenSCManagerW)(LPCTSTR lpMachineName, LPCTSTR lpDatabaseName, DWORD dwDesiredAccess);
typedef SC_HANDLE(WINAPI *TdefOldOpenServiceA)(SC_HANDLE hSCManager, LPCTSTR lpServiceName, DWORD dwDesiredAccess);
typedef SC_HANDLE(WINAPI *TdefOldOpenServiceW)(SC_HANDLE hSCManager, LPCWSTR lpServiceName, DWORD dwDesiredAccess);
typedef BOOL(WINAPI *TdefOldControlService)(SC_HANDLE hService, DWORD dwControl, LPSERVICE_STATUS lpServiceStatus);

extern "C"
{
	// Cryptography
	BOOL WINAPI NewCryptAcquireContextA(HCRYPTPROV *phProv, LPCTSTR pszContainer, LPCTSTR pszProvider, DWORD dwProvType, DWORD dwFlags);			// Used to acquire a handle to a particular key container within a particular cryptographic service provider (CSP)
	BOOL WINAPI NewCryptAcquireContextW(HCRYPTPROV *phProv, LPCTSTR pszContainer, LPCTSTR pszProvider, DWORD dwProvType, DWORD dwFlags);			// Used to acquire a handle to a particular key container within a particular cryptographic service provider (CSP)											
	BOOL WINAPI NewCryptEncrypt(HCRYPTKEY hKey, HCRYPTHASH hHash, BOOL Final, DWORD dwFlags, BYTE *pbData, DWORD *pdwDataLen, DWORD dwBufLen);		// encrypts data
	BOOL WINAPI NewCryptHashData(HCRYPTHASH hHash, BYTE *pbData, DWORD dwDataLen, DWORD dwFlags);													// Adds data to a specified hash object		
	// Services
	SC_HANDLE WINAPI NewOpenSCManagerA(LPCTSTR lpMachineName, LPCTSTR lpDatabaseName, DWORD dwDesiredAccess);
	SC_HANDLE WINAPI NewOpenSCManagerW(LPCTSTR lpMachineName, LPCTSTR lpDatabaseName, DWORD dwDesiredAccess);
	SC_HANDLE WINAPI NewOpenServiceA(SC_HANDLE hSCManager, LPCTSTR lpServiceName, DWORD dwDesiredAccess);
	SC_HANDLE WINAPI NewOpenServiceW(SC_HANDLE hSCManager, LPCWSTR lpServiceName, DWORD dwDesiredAccess);
	BOOL WINAPI NewControlService(SC_HANDLE hService, DWORD dwControl, LPSERVICE_STATUS lpServiceStatus);
}
	extern "C" TdefOldCryptAcquireContextA OldCryptAcquireContextA;
	extern "C" TdefOldCryptAcquireContextW OldCryptAcquireContextW;
	extern "C" TdefOldCryptHashData OldCryptHashData;
	extern "C" TdefOldCryptEncrypt OldCryptEncrypt;

	extern "C" TdefOldOpenSCManagerA OldOpenSCManagerA;
	extern "C" TdefOldOpenSCManagerW OldOpenSCManagerW;
	extern "C" TdefOldOpenServiceA OldOpenServiceA;
	extern "C" TdefOldOpenServiceW OldOpenServiceW;
	extern "C" TdefOldControlService OldControlService;

