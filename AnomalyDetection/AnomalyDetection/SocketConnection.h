#pragma once

// link with Ws2_32.lib
#pragma comment(lib,"ws2_32.lib")

#include <windows.h>
#include "MainForm.h"

using namespace AnomalyDetection;
using namespace System::Threading;

ref class WinsockConnection
{
private:
	MainForm^ form;
	char* IpAddress = "127.0.0.1";
	u_short host = 35000;
	BOOL initialized;
	void Initialize();

public:
	WinsockConnection(MainForm^ form);
	void Connection();
	//void WinsockConnection::Set()
};