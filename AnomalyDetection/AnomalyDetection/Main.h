#pragma once
#include <windows.h>
#include "MainForm.h"

using namespace AnomalyDetection;

char* SystemStringToCharPointer(String^ string);
bool CheckFileCompatibility(char* fileName);
char* BackupFile(char* fileName);
void CopyDll(char* fileName);

// This delegate enables asynchronous calls for setting  
// the text property on a TextBox control.  
delegate void StringArgReturningVoidDelegate(String^ text);
delegate void IntArgReturningVoidDelegate(int score);
