#pragma once

#include <Windows.h>

// Cryptography
typedef BOOL(WINAPI *TdefOldCryptAcquireContextA)(HCRYPTPROV *phProv, LPCTSTR pszContainer, LPCTSTR pszProvider, DWORD dwProvType, DWORD dwFlags);
typedef BOOL(WINAPI *TdefOldCryptAcquireContextW)(HCRYPTPROV *phProv, LPCTSTR pszContainer, LPCTSTR pszProvider, DWORD dwProvType, DWORD dwFlags);

//typedef BOOL (WINAPI *TdefOldCryptGenKey)(HCRYPTPROV hProv, ALG_ID Algid, DWORD dwFlags, HCRYPTKEY *phKey);
//typedef BOOL (WINAPI *TdefOldCryptGetUserKey)(HCRYPTPROV hProv, DWORD dwKeySpec, HCRYPTKEY *phUserKey);
//typedef BOOL (WINAPI *TdefOldCryptExportKey)(HCRYPTKEY hKey, HCRYPTKEY hExpKey, DWORD dwBlobType, DWORD dwFlags, BYTE *pbData, DWORD *pdwDataLen);
//
//typedef BOOL (WINAPI *TdefOldCryptCreateHash)(HCRYPTPROV hProv, ALG_ID Algid, HCRYPTKEY hKey, DWORD dwFlags, HCRYPTHASH *phHash);
typedef BOOL(WINAPI *TdefOldCryptHashData)(HCRYPTHASH hHash, BYTE *pbData, DWORD dwDataLen, DWORD dwFlags);
//typedef BOOL (WINAPI *TdefOldCryptDeriveKey)(HCRYPTPROV hProv, ALG_ID Algid, HCRYPTHASH hBaseData, DWORD dwFlags, HCRYPTKEY *phKey);
typedef BOOL(WINAPI *TdefOldCryptEncrypt)(HCRYPTKEY hKey, HCRYPTHASH hHash, BOOL Final, DWORD dwFlags, BYTE *pbData, DWORD *pdwDataLen, DWORD dwBufLen);

// Cryptography
extern "C" BOOL WINAPI NewCryptAcquireContextA(HCRYPTPROV *phProv, LPCTSTR pszContainer, LPCTSTR pszProvider, DWORD dwProvType, DWORD dwFlags);			// Used to acquire a handle to a particular key container within a particular cryptographic service provider (CSP)
extern "C" BOOL WINAPI NewCryptAcquireContextW(HCRYPTPROV *phProv, LPCTSTR pszContainer, LPCTSTR pszProvider, DWORD dwProvType, DWORD dwFlags);			// Used to acquire a handle to a particular key container within a particular cryptographic service provider (CSP)
																																				//BOOL WINAPI NewCryptGenKey( HCRYPTPROV hProv, ALG_ID Algid, DWORD dwFlags, HCRYPTKEY *phKey );													// Generates a random cryptographic session key or a public/private key pair
																																				//BOOL WINAPI NewCryptGetUserKey( HCRYPTPROV hProv, DWORD dwKeySpec, HCRYPTKEY *phUserKey );														// Retrieves a handle of one of a user's two public/private key pairs
																																				//BOOL WINAPI NewCryptExportKey( HCRYPTKEY hKey, HCRYPTKEY hExpKey, DWORD dwBlobType,  DWORD dwFlags, BYTE *pbData, DWORD *pdwDataLen );			// Exports a cryptographic key or a key pair from a cryptographic service provider (CSP) in a secure manner
																																				//BOOL WINAPI NewCryptCreateHash( HCRYPTPROV hProv, ALG_ID Algid, HCRYPTKEY hKey, DWORD dwFlags, HCRYPTHASH *phHash );							// Initiates the hashing of a stream of data
																																				//BOOL WINAPI NewCryptDeriveKey( HCRYPTPROV hProv, ALG_ID Algid, HCRYPTHASH hBaseData, DWORD dwFlags, HCRYPTKEY *phKey );							// generates cryptographic session keys derived from a base data value
//BOOL WINAPI NewCryptEncrypt(HCRYPTKEY hKey, HCRYPTHASH hHash, BOOL Final, DWORD dwFlags, BYTE *pbData, DWORD *pdwDataLen, DWORD dwBufLen);				// encrypts data
extern "C" BOOL WINAPI NewCryptHashData(HCRYPTHASH hHash, BYTE *pbData, DWORD dwDataLen, DWORD dwFlags);											// Adds data to a specified hash object		


																																		//Cryptography
extern "C" TdefOldCryptAcquireContextA OldCryptAcquireContextA;
extern "C" TdefOldCryptAcquireContextW OldCryptAcquireContextW;
//TdefOldCryptGenKey OldCryptGenKey;
//TdefOldCryptGetUserKey OldCryptGetUserKey;
//TdefOldCryptExportKey OldCryptExportKey;
//TdefOldCryptCreateHash OldCryptCreateHash;
extern "C" TdefOldCryptHashData OldCryptHashData;
//TdefOldCryptDeriveKey OldCryptDeriveKey;
extern "C" TdefOldCryptEncrypt OldCryptEncrypt;