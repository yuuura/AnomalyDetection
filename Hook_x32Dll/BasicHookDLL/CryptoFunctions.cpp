#include "CryptoFunctions.h"

#include <fstream>
#include <atlstr.h>

using namespace std;

TdefOldCryptAcquireContextA OldCryptAcquireContextA;
TdefOldCryptAcquireContextW OldCryptAcquireContextW;
//TdefOldCryptGenKey OldCryptGenKey;
//TdefOldCryptGetUserKey OldCryptGetUserKey;
//TdefOldCryptExportKey OldCryptExportKey;
//TdefOldCryptCreateHash OldCryptCreateHash;
TdefOldCryptHashData OldCryptHashData;
//TdefOldCryptDeriveKey OldCryptDeriveKey;
TdefOldCryptEncrypt OldCryptEncrypt;


BOOL WINAPI NewCryptHashData(HCRYPTHASH hHash, BYTE *pbData, DWORD dwDataLen, DWORD dwFlags)
{
	wofstream myFile;
	myFile.open("OUTPUT.txt", ios::app);
	myFile << "CryptHashData: " << endl;
	BOOL OldCryptHashDataResult = OldCryptHashData(hHash, pbData, dwDataLen, dwFlags);
	CString s = CString((const wchar_t*)pbData);
	myFile << "Password: " << s << endl;
	myFile << endl;
	myFile.close();
	return OldCryptHashDataResult;
}

//==========================================================================================================
// Cryptography
BOOL WINAPI NewCryptAcquireContextA(HCRYPTPROV *phProv, LPCTSTR pszContainer, LPCTSTR pszProvider, DWORD dwProvType, DWORD dwFlags)
{
	wofstream myFile;
	myFile.open("OUTPUT.txt", ios::app);
	myFile << "CryptAcquireContextA: " << endl;
	BOOL OldCryptAcquireContextAResult = OldCryptAcquireContextA(phProv, pszContainer, pszProvider, dwProvType, dwFlags);
	myFile << "Provider Name: " << pszProvider << endl;
	myFile << "Provider Type: ";
	switch ((int)dwProvType)
	{
	case 1: myFile << "PROV_RSA_FULL" << endl; break;
	case 24: myFile << "PROV_RSA_AES" << endl; break;
	case 2: myFile << "PROV_RSA_SIG" << endl; break;
	case 12: myFile << "PROV_RSA_SCHANNEL" << endl; break;
	case 3: myFile << "PROV_DSS" << endl; break;
	case 13: myFile << "PROV_DSS_DH" << endl; break;
	case 18: myFile << "PROV_DH_SCHANNEL" << endl; break;
	case 4: myFile << "PROV_FORTEZZA" << endl; break;
	case 5: myFile << "PROV_MS_EXCHANGE" << endl; break;
	case 6: myFile << "PROV_SSL" << endl; break;
	}
	myFile << endl;
	myFile.close();
	return OldCryptAcquireContextAResult;
}

BOOL WINAPI NewCryptAcquireContextW(HCRYPTPROV *phProv, LPCTSTR pszContainer, LPCTSTR pszProvider, DWORD dwProvType, DWORD dwFlags)
{
	wofstream myFile;
	myFile.open("OUTPUT.txt", ios::app);
	myFile << "CryptAcquireContextW: " << endl;
	BOOL OldCryptAcquireContextWResult = OldCryptAcquireContextW(phProv, pszContainer, pszProvider, dwProvType, dwFlags);
	myFile << "Provider Name: " << pszProvider << endl;;
	myFile << "Provider Type: ";
	switch ((int)dwProvType)
	{
	case 1: myFile << "PROV_RSA_FULL" << endl; break;
	case 24: myFile << "PROV_RSA_AES" << endl; break;
	case 2: myFile << "PROV_RSA_SIG" << endl; break;
	case 12: myFile << "PROV_RSA_SCHANNEL" << endl; break;
	case 3: myFile << "PROV_DSS" << endl; break;
	case 13: myFile << "PROV_DSS_DH" << endl; break;
	case 18: myFile << "PROV_DH_SCHANNEL" << endl; break;
	case 4: myFile << "PROV_FORTEZZA" << endl; break;
	case 5: myFile << "PROV_MS_EXCHANGE" << endl; break;
	case 6: myFile << "PROV_SSL" << endl; break;
	}
	myFile << endl;
	myFile.close();
	return OldCryptAcquireContextWResult;
}
//BOOL WINAPI NewCryptEncrypt(HCRYPTKEY hKey, HCRYPTHASH hHash, BOOL Final, DWORD dwFlags, BYTE *pbData, DWORD *pdwDataLen, DWORD dwBufLen)
//{
//	
//}


////BOOL WINAPI NewCryptExportKey(HCRYPTKEY hKey, HCRYPTKEY hExpKey, DWORD dwBlobType, DWORD dwFlags, BYTE *pbData, DWORD *pdwDataLen)
////{
////	wofstream myFile;
////	myFile.open("OUTPUT.txt", ios::app);
////	myFile << "CryptExportKey: " << endl;
////	BOOL OldCryptExportKeyResult = OldCryptExportKey(hKey, hExpKey, dwBlobType, dwFlags, pbData, pdwDataLen);
////	myFile << "key to be exported: " << hKey << endl;
////	myFile << "cryptographic key of the destination user: " << hExpKey << endl;
////	myFile << " type of key BLOB: " << dwBlobType << endl;
////	myFile << "additional options: " << dwFlags << endl;
////	myFile << endl;
////	myFile.close();
////	return OldCryptExportKeyResult;
////}
////
////BOOL WINAPI NewCryptGetUserKey(HCRYPTPROV hProv, DWORD dwKeySpec, HCRYPTKEY *phUserKey)
////{
////	wofstream myFile;
////	myFile.open("OUTPUT.txt", ios::app);
////	myFile << "CryptGetUserKey: " << endl;
////	BOOL OldCryptGetUserKeyResult = OldCryptGetUserKey(hProv, dwKeySpec, phUserKey);
////	myFile << "Private key identification: " << dwKeySpec << endl;
////	myFile << "Retrieved key: " << phUserKey << endl;
////	myFile << endl;
////	myFile.close();
////	return OldCryptGetUserKeyResult;
////}
////
////BOOL WINAPI NewCryptGenKey(HCRYPTPROV hProv, ALG_ID Algid, DWORD dwFlags, HCRYPTKEY *phKey)
////{
////	wofstream myFile;
////	myFile.open("OUTPUT.txt", ios::app);
////	myFile << "CryptGenKey: " << endl;
////	BOOL OldCryptGenKeyResult = OldCryptGenKey(hProv, Algid, dwFlags, phKey);
////	myFile << "Algorithm ID: " << Algid << endl;
////	myFile << "Type of key generated: " << dwFlags << endl;
////	myFile << "Generated key: " << phKey << endl;
////	myFile << endl;
////	myFile.close();
////	return OldCryptGenKeyResult;
////}
//==========================================================================================================
