#pragma once
#include <windows.h>
#include <vector>

bool MyEncryptFile(LPTSTR szSource, LPTSTR szDestination, LPTSTR szPassword);

void MyHandleError(LPTSTR psz, int nErrorNumber);

extern "C" int Encrypt(std::vector<std::string> fileNames, char* password);