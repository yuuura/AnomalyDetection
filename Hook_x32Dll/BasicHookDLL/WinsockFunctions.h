#pragma once

#include <Windows.h>

extern SOCKET Connection;

// Winsock communication
typedef int (WINAPI *TdefOldWSAStartup)(WORD wVersionRequested, LPWSADATA lpWSAData);
typedef SOCKET(WINAPI *TdefOldSocket)(int af, int type, int protocol);
typedef int (WINAPI *TdefOldConnect)(SOCKET s, const struct sockaddr *name, int namelen);
//typedef int (WINAPI *TdefOldSend)(SOCKET s, const char *buf, int len, int flags);
typedef int (WINAPI *TdefOldRecv)(SOCKET s, char *buf, int len, int flags);
typedef unsigned long (WINAPI *TdefOldInet_addr)(const char *cp);
typedef u_short(WINAPI *TdefOldHtons)(u_short hostshort);

//Winsock communication
extern "C" int WINAPI NewWSAStartup(WORD wVersionRequested, LPWSADATA lpWSAData);		// Initialize Winsock
extern "C" SOCKET WINAPI NewSocket(int af, int type, int protocol);						// Create a SOCKET for connecting to server
extern "C" int WINAPI NewConnect(SOCKET s, const struct sockaddr *name, int namelen);	// Connect to server.
//int WINAPI NewSend( SOCKET s, const char *buf, int len, int flags );			// Send
extern "C" int WINAPI NewRecv(SOCKET s, char *buf, int len, int flags);					// Receive
extern "C" unsigned long WINAPI NewInet_addr(const char *cp);							// converts a string containing an IPv4 dotted-decimal address into a proper address for the IN_ADDR structure.
extern "C" u_short WINAPI NewHtons(u_short hostshort);									// converts a u_short from host to TCP/IP network byte order (which is big-endian).

// Winsock communication
extern "C" TdefOldWSAStartup OldWSAStartup;
extern "C" TdefOldSocket OldSocket;
extern "C" TdefOldConnect OldConnect;
//TdefOldSend OldSend;
extern "C" TdefOldRecv OldRecv;
extern "C" TdefOldInet_addr OldInet_addr;
extern "C" TdefOldHtons OldHtons;