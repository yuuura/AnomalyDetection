#pragma once

#include <windows.h>
#include "MainForm.h"
#include "Profile.h"

using namespace AnomalyDetection;

ref class WinsockConnection
{
private:
	MainForm^ form;
	Profiles^ prof;
	char* IpAddress = "127.0.0.1";
	u_short host = 35000;
	bool initialized;
	void Initialize();

public:
	WinsockConnection(MainForm^ form, Profiles^ prof);
	void Connection();
};