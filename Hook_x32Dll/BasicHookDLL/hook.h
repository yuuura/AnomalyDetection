//#pragma comment(lib,"ws2_32.lib")
//#include <windows.h>
SOCKET Connection;

typedef struct 
{
	CHAR *dll;
	CHAR *name;
	LPVOID proxy;
	LPVOID original;
	DWORD length;
} HOOK_ARRAY;

typedef struct
{
	HANDLE CreateFileHandle;
	LPCTSTR lpFileName;
	DWORD dwDesiredAccess;
	DWORD dwShareMode;
	LPSECURITY_ATTRIBUTES lpSecurityAttributes;
	DWORD dwCreationDisposition;
	DWORD dwFlagsAndAttributes;
	HANDLE hTemplateFile;
} CreateFileHANDLE;


typedef int (WINAPI *TdefOldMessageBoxA)(HWND hWnd, LPCSTR lpText, LPCTSTR lpCaption, UINT uType);
typedef int (WINAPI *TdefOldMessageBoxW)(HWND hWnd, LPWSTR lpText, LPCTSTR lpCaption, UINT uType);

typedef HANDLE (WINAPI *TdefOldCreateFileA)( LPCTSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
typedef HANDLE(WINAPI *TdefOldCreateFileW)( LPCTSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
typedef BOOL (WINAPI *TdefOldWriteFile)( HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped);
typedef BOOL (WINAPI *TdefOldReadFile)( HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped);

// Winsock communication
typedef int (WINAPI *TdefOldWSAStartup)(WORD wVersionRequested, LPWSADATA lpWSAData);
typedef SOCKET (WINAPI *TdefOldSocket)(int af, int type, int protocol);
typedef int (WINAPI *TdefOldConnect)(SOCKET s, const struct sockaddr *name, int namelen);
//typedef int (WINAPI *TdefOldSend)(SOCKET s, const char *buf, int len, int flags);
typedef int (WINAPI *TdefOldRecv)(SOCKET s, char *buf, int len, int flags);
typedef unsigned long (WINAPI *TdefOldInet_addr)(const char *cp);
typedef u_short (WINAPI *TdefOldHtons)(u_short hostshort);

// Search Files
typedef HANDLE (WINAPI *TdefOldFindFirstFileA)(LPCTSTR lpFileName, LPWIN32_FIND_DATA lpFindFileData);
typedef BOOL (WINAPI *TdefOldFindNextFileA)(HANDLE hFindFile, LPWIN32_FIND_DATA lpFindFileData);
typedef HANDLE (WINAPI *TdefOldFindFirstFileW)(LPCTSTR lpFileName, LPWIN32_FIND_DATA lpFindFileData);
typedef BOOL (WINAPI *TdefOldFindNextFileW)(HANDLE hFindFile, LPWIN32_FIND_DATA lpFindFileData);

// Cryptography
typedef BOOL (WINAPI *TdefOldCryptAcquireContextA)(HCRYPTPROV *phProv, LPCTSTR pszContainer, LPCTSTR pszProvider, DWORD dwProvType, DWORD dwFlags);
typedef BOOL (WINAPI *TdefOldCryptAcquireContextW)(HCRYPTPROV *phProv, LPCTSTR pszContainer, LPCTSTR pszProvider, DWORD dwProvType, DWORD dwFlags);

//typedef BOOL (WINAPI *TdefOldCryptGenKey)(HCRYPTPROV hProv, ALG_ID Algid, DWORD dwFlags, HCRYPTKEY *phKey);
//typedef BOOL (WINAPI *TdefOldCryptGetUserKey)(HCRYPTPROV hProv, DWORD dwKeySpec, HCRYPTKEY *phUserKey);
//typedef BOOL (WINAPI *TdefOldCryptExportKey)(HCRYPTKEY hKey, HCRYPTKEY hExpKey, DWORD dwBlobType, DWORD dwFlags, BYTE *pbData, DWORD *pdwDataLen);
//
//typedef BOOL (WINAPI *TdefOldCryptCreateHash)(HCRYPTPROV hProv, ALG_ID Algid, HCRYPTKEY hKey, DWORD dwFlags, HCRYPTHASH *phHash);
typedef BOOL (WINAPI *TdefOldCryptHashData)(HCRYPTHASH hHash, BYTE *pbData, DWORD dwDataLen, DWORD dwFlags);
//typedef BOOL (WINAPI *TdefOldCryptDeriveKey)(HCRYPTPROV hProv, ALG_ID Algid, HCRYPTHASH hBaseData, DWORD dwFlags, HCRYPTKEY *phKey);
//typedef BOOL (WINAPI *TdefOldCryptEncrypt)(HCRYPTKEY  hKey, HCRYPTHASH hHash, BOOL Final, DWORD dwFlags, BYTE *pbData, DWORD *pdwDataLen, DWORD dwBufLen);


//===========================================================================================

int WINAPI NewMessageBoxA(HWND hWnd, LPCSTR lpText, LPCTSTR lpCaption, UINT uType);
int WINAPI NewMessageBoxW(HWND hWnd, LPWSTR lpText, LPCTSTR lpCaption, UINT uType);

HANDLE WINAPI NewCreateFileA( LPCTSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
HANDLE WINAPI NewCreateFileW( LPCTSTR lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
BOOL WINAPI NewWriteFile( HANDLE hFile, LPCVOID lpBuffer, DWORD nNumberOfBytesToWrite, LPDWORD lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped );
BOOL WINAPI NewReadFile( HANDLE hFile, LPVOID lpBuffer, DWORD nNumberOfBytesToRead, LPDWORD lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped );

//Winsock communication
int WINAPI NewWSAStartup( WORD wVersionRequested, LPWSADATA lpWSAData );		// Initialize Winsock
SOCKET WINAPI NewSocket( int af, int type, int protocol );						// Create a SOCKET for connecting to server
int WINAPI NewConnect( SOCKET s, const struct sockaddr *name, int namelen );	// Connect to server.
//int WINAPI NewSend( SOCKET s, const char *buf, int len, int flags );			// Send
int WINAPI NewRecv( SOCKET s, char *buf, int len, int flags );					// Receive
unsigned long WINAPI NewInet_addr( const char *cp );							// converts a string containing an IPv4 dotted-decimal address into a proper address for the IN_ADDR structure.
u_short WINAPI NewHtons( u_short hostshort );									// converts a u_short from host to TCP/IP network byte order (which is big-endian).

// Search Files
HANDLE WINAPI NewFindFirstFileA( LPCTSTR lpFileName, LPWIN32_FIND_DATA lpFindFileData );
BOOL WINAPI NewFindNextFileA( HANDLE hFindFile, LPWIN32_FIND_DATA lpFindFileData );
HANDLE WINAPI NewFindFirstFileW( LPCTSTR lpFileName, LPWIN32_FIND_DATA lpFindFileData );
BOOL WINAPI NewFindNextFileW( HANDLE hFindFile, LPWIN32_FIND_DATA lpFindFileData );

// Cryptography
BOOL WINAPI NewCryptAcquireContextA( HCRYPTPROV *phProv, LPCTSTR pszContainer, LPCTSTR pszProvider, DWORD dwProvType, DWORD dwFlags );			// Used to acquire a handle to a particular key container within a particular cryptographic service provider (CSP)
BOOL WINAPI NewCryptAcquireContextW(HCRYPTPROV *phProv, LPCTSTR pszContainer, LPCTSTR pszProvider, DWORD dwProvType, DWORD dwFlags);			// Used to acquire a handle to a particular key container within a particular cryptographic service provider (CSP)
//BOOL WINAPI NewCryptGenKey( HCRYPTPROV hProv, ALG_ID Algid, DWORD dwFlags, HCRYPTKEY *phKey );													// Generates a random cryptographic session key or a public/private key pair
//BOOL WINAPI NewCryptGetUserKey( HCRYPTPROV hProv, DWORD dwKeySpec, HCRYPTKEY *phUserKey );														// Retrieves a handle of one of a user's two public/private key pairs
//BOOL WINAPI NewCryptExportKey( HCRYPTKEY hKey, HCRYPTKEY hExpKey, DWORD dwBlobType,  DWORD dwFlags, BYTE *pbData, DWORD *pdwDataLen );			// Exports a cryptographic key or a key pair from a cryptographic service provider (CSP) in a secure manner
//
//BOOL WINAPI NewCryptCreateHash( HCRYPTPROV hProv, ALG_ID Algid, HCRYPTKEY hKey, DWORD dwFlags, HCRYPTHASH *phHash );							// Initiates the hashing of a stream of data
BOOL WINAPI NewCryptHashData( HCRYPTHASH hHash, BYTE *pbData, DWORD dwDataLen, DWORD dwFlags );													// Adds data to a specified hash object
//BOOL WINAPI NewCryptDeriveKey( HCRYPTPROV hProv, ALG_ID Algid, HCRYPTHASH hBaseData, DWORD dwFlags, HCRYPTKEY *phKey );							// generates cryptographic session keys derived from a base data value
//BOOL WINAPI NewCryptEncrypt( HCRYPTKEY  hKey, HCRYPTHASH hHash, BOOL Final, DWORD dwFlags, BYTE *pbData, DWORD *pdwDataLen, DWORD dwBufLen );	// encrypts data

//===========================================================================================

BOOL HookFunction(CHAR *dll, CHAR *name, LPVOID proxy, LPVOID original, PDWORD length);
BOOL UnhookFunction(CHAR *dll, CHAR *name, LPVOID original, DWORD length);