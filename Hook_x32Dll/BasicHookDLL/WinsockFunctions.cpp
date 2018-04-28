// link with Ws2_32.lib
#pragma comment(lib,"ws2_32.lib")

#include "WinsockFunctions.h"
#include <fstream>
using namespace std;

SOCKET Connection;

// Winsock communication
TdefOldWSAStartup OldWSAStartup;
TdefOldSocket OldSocket;
TdefOldConnect OldConnect;
//TdefOldSend OldSend;
TdefOldRecv OldRecv;
TdefOldInet_addr OldInet_addr;
TdefOldHtons OldHtons;

unsigned long WINAPI NewInet_addr(const char *cp)
{
	wofstream myFile;
	myFile.open("OUTPUT.txt", ios::app);
	myFile << "Winsock Inet_addr: " << endl;
	myFile << cp << endl;
	unsigned long OldInet_addrResult = OldInet_addr(cp);
	myFile << endl;
	myFile.close();
	return OldInet_addrResult;
}

SOCKET WINAPI NewSocket(int af, int type, int protocol)
{
	wofstream myFile;
	myFile.open("OUTPUT.txt", ios::app);
	myFile << "Winsock socket: " << endl;
	myFile << af << ", " << type << ", " << protocol << endl;
	SOCKET OldSocketResult = OldSocket(af, type, protocol);
	myFile << endl;
	myFile.close();
	return OldSocketResult;
}

u_short WINAPI NewHtons(u_short hostshort)
{
	wofstream myFile;
	myFile.open("OUTPUT.txt", ios::app);
	myFile << "Winsock htons: " << endl;
	myFile << hostshort << endl;
	u_short OldHtonsResult = OldHtons(hostshort);
	myFile << endl;
	myFile.close();
	return OldHtonsResult;
}

//int WINAPI NewSend(SOCKET s, const char *buf, int len, int flags)
//{
//	wofstream myFile;
//	myFile.open("OUTPUT.txt", ios::app);
//	myFile << "Winsock send: " << endl;
//	int OldSendResult = OldSend(s, buf, len, flags);
//	myFile << s << ", " << buf << ", " << len << ", " << flags << endl;
//	myFile << endl;
//	myFile.close();
//	return OldSendResult;
//}

int WINAPI NewRecv(SOCKET s, char *buf, int len, int flags)
{
	wofstream myFile;
	myFile.open("OUTPUT.txt", ios::app);
	myFile << "Winsock recv: " << endl;
	int OldRecvResult = OldRecv(s, buf, len, flags);
	myFile << s << ", " << buf << ", " << len << ", " << flags << ", " << OldRecvResult << endl;
	myFile << endl;
	myFile.close();
	return OldRecvResult;
}

int WINAPI NewWSAStartup(WORD wVersionRequested, LPWSADATA lpWSAData)
{
	wofstream myFile;
	myFile.open("OUTPUT.txt", ios::app);
	myFile << "Winsock WSAStartup: " << endl;
	int OldWSAStartupResult = OldWSAStartup(wVersionRequested, lpWSAData);
	myFile << wVersionRequested << ", " << lpWSAData << endl;
	myFile << endl;
	myFile.close();
	return OldWSAStartupResult;
}

int WINAPI NewConnect(SOCKET s, const struct sockaddr *name, int namelen)
{
	wofstream myFile;
	myFile.open("OUTPUT.txt", ios::app);
	myFile << "Winsock connect:" << endl;
	if ((int)name->sa_family == AF_INET)
	{
		myFile << "IPv4" << endl;
	}
	myFile << s << ", " << (SOCKADDR*)name->sa_family << ", " << (int)name->sa_family << ", " << namelen << endl;

	int oldConnectResult = OldConnect(s, name, namelen);
	myFile << oldConnectResult;
	myFile << endl;
	myFile.close();
	return oldConnectResult;
}