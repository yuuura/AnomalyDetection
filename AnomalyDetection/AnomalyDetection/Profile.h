#pragma once
#include "MainForm.h"
using namespace AnomalyDetection;
using namespace System;
using namespace System::Collections::Generic;
using namespace System::IO;

ref class Profiles
{
private:
	MainForm^ form;
	List< List <String^>^ >^ funcName = gcnew List<List<String^>^>();			// Structure: | ProfileName | Score  | Func1 | Func2  |  ...  | FuncN |
	List< List <int>^ >^ funcBool = gcnew List<List<int>^>();					// Structure: |    Bool1    | Bool2  |  ...  | BoolN  | Count |
	List< List <String^>^ >^ funcValue = gcnew List<List<String^>^>();			// Structure: |    Value1   | Value2 |  ...  | ValueN |
	int currentScore = 1;
	String^ fileName;
	const char* profileFileName = "Profiles\\Profiles.txt";
	String^ reportFolderLocation;
	StreamWriter^ sw;
	void Profiles::CloseFile(StreamWriter^ sw);

public: 
	Profiles(MainForm^ form, String^, String^);
	void InitProfiles();
	void RecvFunction(String^ s, String^ s2);
	void SetCurrentScore(int s);
	int GetCurrentScore();
	void DeactivateProfileData();
	void RecvReportData(String^ str);
	void PrepareReportFile();
	void PrepareScore();
};