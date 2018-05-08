// link with Ws2_32.lib
#pragma comment(lib,"ws2_32.lib")
#include "SocketConnection.h"
#include <string>
using namespace System;
using namespace System::Threading;
#define BUFFER_SIZE 512

WinsockConnection::WinsockConnection(MainForm^ form, Profiles^ prof)
{
	initialized = FALSE;
	WinsockConnection::form = form;
	WinsockConnection::prof = prof;
	Initialize();
}

void  WinsockConnection::Initialize()
{
	//WinSock Startup
	WSAData wsaData;
	WORD DllVersion = MAKEWORD(2, 1);
	if (WSAStartup(DllVersion, &wsaData) != 0) //If WSAStartup returns anything other than 0, then that means an error has occured in the WinSock Startup.
	{
		form->SetText("(*) WinSock startup failed.\n");
		initialized = false;
	}
	else initialized = true;
}

void  WinsockConnection::Connection()
{
	if (!initialized)
		return;

	SOCKADDR_IN addr; //Address that we will bind our listening socket to
	int addrlen = sizeof(addr); //length of the address (required for accept call)
	addr.sin_addr.s_addr = inet_addr(IpAddress); //Broadcast locally
	addr.sin_port = htons(host); //Port
	addr.sin_family = AF_INET; //IPv4 Socket

	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL); //Create socket to listen for new connections
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr)); //Bind the address to the socket
	listen(sListen, SOMAXCONN); //Places sListen socket in a state in which it is listening for an incoming connection. Note:SOMAXCONN = Socket Oustanding Max Connections
	while (true)
	{
		SOCKET client; //Socket to hold the client's connection
		client = accept(sListen, (SOCKADDR*)&addr, &addrlen); //Accept a new connection
		form->ClearScreen();
		if (client == 0) //If accepting the client connection failed
		{
			form->SetText(gcnew String("(*) Failed to accept the client's connection.\n"));
			break;
		}
		else
		{
			prof->PrepareScore();
			prof->PrepareReportFile();
			form->SetText(gcnew String("File start running.\n"));
			int ans;
			while (true)
			{
				char Message[BUFFER_SIZE];
				ans = recv(client, Message, sizeof(Message), NULL);	//Receive Message of the Day buffer into MOTD array
				if (ans == SOCKET_ERROR)
				{
					closesocket(client);
					form->SetText(gcnew String("File run completed.\n ============================================\n"));
					prof->DeactivateProfileData();
					form->SetText(gcnew String("Report created. To open click \"View Report\".\n"));
					break;
				}
				std::string str(Message);
				std::string f = str.substr(0, str.find(" "));
				std::string i = str.substr(str.find(" ") + 1);
				prof->RecvFunction(gcnew String(f.c_str()), gcnew String(i.c_str()));
				prof->RecvReportData(gcnew String(i.c_str()));
			}
		}
	}
}
