#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <omp.h>
#pragma comment(lib,"ws2_32.lib")

int startPort = 135;
int endPort = 141;
char* ip = "127.0.0.1";
SOCKET Socket;
WSADATA WsaDat;
SOCKADDR_IN server;

int core(int port)
{
	Socket = socket(AF_INET, SOCK_STREAM, 0);
	if (Socket == SOCKET_ERROR)
		return 0;

	server.sin_port = htons(port);
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr(ip);

	if (connect(Socket, (SOCKADDR*)(&server), sizeof(server)) != SOCKET_ERROR)
		closesocket(Socket);
		
	return 1;
}

int main()
{
	if (WSAStartup(MAKEWORD(2, 2), &WsaDat) != 0)
		return 0;
	
	for (int i = startPort; i < endPort; i++)
		core(i);
	
	WSACleanup();
	return 0;
}