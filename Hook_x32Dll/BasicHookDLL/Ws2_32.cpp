#define _CRT_SECURE_NO_WARNINGS

// link with Ws2_32.lib
#pragma comment(lib,"ws2_32.lib")
#include "Ws2_32.h"
#include <fstream>

#define SLEEP_TIME 10
#define BUFFER_SIZE 512

// Winsock communication
TdefOldWSAStartup OldWSAStartup;
TdefOldSocket OldSocket;
TdefOldConnect OldConnect;
TdefOldInet_addr OldInet_addr;
TdefOldHtons OldHtons;

SOCKET Connection;

// Resolve IP address
unsigned long WINAPI NewInet_addr(const char *cp)
{
	char buffer[BUFFER_SIZE] = "Inet_addr ";
	
	strcat(buffer, "\n\nConnecting with IP address: ");
	strcat(buffer, cp);
	
	send(Connection, buffer, sizeof(buffer), NULL);
	Sleep(SLEEP_TIME);
	unsigned long OldInet_addrResult = OldInet_addr(cp);
	return OldInet_addrResult;
}

SOCKET WINAPI NewSocket(int af, int type, int protocol)
{
	char buffer[BUFFER_SIZE] = "Socket ";
	
	strcat(buffer, "\n\nAddress family: ");
	switch (af)
	{
	case AF_UNSPEC: strcat(buffer, "Not specified."); break;
	case AF_INET: strcat(buffer, "IPv4"); break;
	case AF_IPX: strcat(buffer, "IPX/SPX"); break;
	case AF_APPLETALK: strcat(buffer, "AppleTalk"); break;
	case AF_NETBIOS: strcat(buffer, "NetBIOS"); break;
	}

	strcat(buffer, "\nProtocol: ");
	switch (af)
	{
	case IPPROTO_ICMP: strcat(buffer, "ICMP."); break;
	case IPPROTO_IGMP: strcat(buffer, "IGMP"); break;
	case IPPROTO_TCP: strcat(buffer, "TCP"); break;
	case IPPROTO_UDP: strcat(buffer, "UDP"); break;
	}

	SOCKET OldSocketResult = OldSocket(af, type, protocol);
	if (OldSocketResult == INVALID_SOCKET)
		strcat(buffer, "\nThis socket connection failed.");

	send(Connection, buffer, sizeof(buffer), NULL);
	Sleep(SLEEP_TIME);
	return OldSocketResult;
}

// Resolve Port address
u_short WINAPI NewHtons(u_short hostshort)
{
	char buffer[BUFFER_SIZE] = "htons ";
	
	strcat(buffer, "\n\nPort: ");
	char port[11];
	sprintf(port, "%d", hostshort);
	strcat(buffer, port);
	u_short OldHtonsResult = OldHtons(hostshort);
	
	send(Connection, buffer, sizeof(buffer), NULL);
	Sleep(SLEEP_TIME);
	return OldHtonsResult;
}

int WINAPI NewWSAStartup(WORD wVersionRequested, LPWSADATA lpWSAData)
{
	char buffer[BUFFER_SIZE] = "WSAStartup ";
	
	strcat(buffer, "\n\nWinsock initialization started.");
	int OldWSAStartupResult = OldWSAStartup(wVersionRequested, lpWSAData);
	if (OldWSAStartupResult != 0)
		strcat(buffer, "\nWinsock initialization failed.");
	else strcat(buffer, "\nWinsock initialization completed.");
	
	send(Connection, buffer, sizeof(buffer), NULL);
	Sleep(SLEEP_TIME);
	return OldWSAStartupResult;
}

int WINAPI NewConnect(SOCKET s, const struct sockaddr *name, int namelen)
{
	char buffer[BUFFER_SIZE] = "Connect ";

	int oldConnectResult = OldConnect(s, name, namelen);
	if(oldConnectResult != 0)
		strcat(buffer, "\n\nConnection to socket failed.");
	else strcat(buffer, "\n\nConnection to socket completed.");
	
	send(Connection, buffer, sizeof(buffer), NULL);
	Sleep(SLEEP_TIME);
	return oldConnectResult;
}