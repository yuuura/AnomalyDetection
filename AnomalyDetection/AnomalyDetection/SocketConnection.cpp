#include "SocketConnection.h"

	WinsockConnection::WinsockConnection(MainForm^ form)
	{
		initialized = FALSE;
		WinsockConnection::form = form;
		Initialize();
	}

	void  WinsockConnection::Initialize()
	{
		//WinSock Startup
		WSAData wsaData;
		WORD DllVersion = MAKEWORD(2, 1);
		if (WSAStartup(DllVersion, &wsaData) != 0) //If WSAStartup returns anything other than 0, then that means an error has occured in the WinSock Startup.
		{
			MessageBox::Show("WinSock startup failed.", "Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
			initialized = FALSE;
		}
		else initialized = TRUE;
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
				form->SetText(gcnew String("Failed to accept the client's connection.\n"));
				break;
			}
			else
			{
				form->SetText(gcnew String("\n ********* File start running *********\n\n"));
				int ans;
				while (true)
				{
					char MOTD[256];
					ans = recv(client, MOTD, sizeof(MOTD), NULL);	//Receive Message of the Day buffer into MOTD array
					if (ans == SOCKET_ERROR)
					{
						closesocket(client);
						form->SetText(gcnew String("\n ******** Program run completed ********.\n"));
						break;
					}
					form->SetText(gcnew String(MOTD));
				}

			}
		}

	}
