#define _CRT_SECURE_NO_WARNINGS
#include "Main.h"
#include "SocketConnection.h"
#include "Hook.h"
#include "Profile.h"
#include <stdexcept>

using namespace System;
using namespace System::Windows::Forms;
using namespace System::Threading;
using namespace System::Diagnostics;

const char* dllFolderlocation = "DllToInject\\";
const char* reportFolderLocation = "Report\\";
const char* reportFileName = "Report.txt";

void MainForm::btnBrowse_Click(System::Object^ sender, System::EventArgs^ e)
{
	OpenFileDialog^ openFileDialog = gcnew OpenFileDialog;
	openFileDialog->Filter = "All files (*.exe; *.scr)|*.exe;*.scr|Exe files (*.exe)|*.exe|Screen saver files (*.scr)|*.scr";
	openFileDialog->ShowDialog();
	txtBoxFile->Text = openFileDialog->FileName;
}
void MainForm::btnViewReport_Click(System::Object^  sender, System::EventArgs^  e)
{
	if (File::Exists(String::Concat(gcnew String(reportFolderLocation), gcnew String(reportFileName))))
		Process::Start(String::Concat(gcnew String(reportFolderLocation), gcnew String(reportFileName)));
	else SetText("Report file does not exist.\n");
}
void MainForm::btnRun_Click(System::Object^ sender, System::EventArgs^ e)
{	
	ClearScreen();
	if (txtBoxFile->Text != "")
		try
	{
		// Check whether file is .exe and x86
		if (CheckFileCompatibility(SystemStringToCharPointer(txtBoxFile->Text)))
			if (CheckSectionExist(SystemStringToCharPointer(txtBoxFile->Text)))
				SetText(gcnew String("Dll already injected. Just run your file.\n"));
			else
			{
				char* newFileName = BackupFile(SystemStringToCharPointer(txtBoxFile->Text));
				CopyDll(newFileName);
				SetText(String::Concat("A new file \"", gcnew String(newFileName), "\" created.\n"));
				if (InjectDll(this, newFileName, SectionName, SectionSize))
					SetText(String::Concat(gcnew String("Run a \""), gcnew String(newFileName), gcnew String("\" file!\n")));
			}
	}
	catch (const std::exception& e)
	{
		SetText(gcnew String(e.what()));
	}
	else SetText(gcnew String("(*) No file chosen.\n"));
}
void MainForm::SetScore(int score)
{
	if (txtScore->InvokeRequired)
	{
		IntArgReturningVoidDelegate^ d = gcnew IntArgReturningVoidDelegate(this, &MainForm::SetScore);
		this->Invoke(d, gcnew array<Object^> { score });
	}
	else {
		txtScore->Text = gcnew String(score.ToString());
	}
}
void MainForm::SetText(String^ string)
{
	if (txtBoxMultiline->InvokeRequired)
	{
		StringArgReturningVoidDelegate^ d = gcnew StringArgReturningVoidDelegate(this, &MainForm::SetText);
		this->Invoke(d, gcnew array<Object^> { string });
	}
	else txtBoxMultiline->AppendText(string);
}

void MainForm::ClearScreen()
{
	if (txtBoxMultiline->InvokeRequired)
		txtBoxMultiline->Invoke(gcnew ClearScreenUI(this, &MainForm::ClearScreen));
	else txtBoxMultiline->Clear();
}

// Copy dll file to path where .exe
void CopyDll(char* fileName)
{
	std::string str(fileName);
	std::string s = str.substr(0, str.find_last_of("\\")) + "\\" + DLL_NAME;
	std::string dstr(DLL_NAME);
	std::string d = dllFolderlocation + dstr;
	CopyFile(d.c_str(), s.c_str(), FALSE);
}

// Backup original file before injecting
char* BackupFile(char* fileName)
{
	std::string str(fileName);
	std::string s= str.substr(str.find("."));
	if(str.substr(str.find(".")) == ".scr")
		s = str.substr(0, str.find(".")) + "_injected.scr";
	else if(str.substr(str.find(".")) == ".exe")
		s = str.substr(0, str.find(".")) + "_injected.exe";
	else throw std::exception("(*) The file is not compatible for scan.\n");
	CopyFile(fileName, s.c_str(), FALSE);
	char *cstr = new char[s.length() + 1];
	strcpy(cstr, s.c_str());
	return cstr;
}

bool CheckFileCompatibility(char* fileName)
{
	DWORD lpBinaryType[100];
	if ((GetBinaryType(fileName, lpBinaryType) == 0) && lpBinaryType != SCS_32BIT_BINARY) {
		throw std::exception("(*) The file is not compatible for scan.\n");
	}
	return true;
}

char* SystemStringToCharPointer(String^ string)
{
	array<Byte> ^chars = System::Text::Encoding::ASCII->GetBytes(string);
	pin_ptr<Byte> charsPointer = &(chars[0]);
	char *filePath = reinterpret_cast<char *>(static_cast<unsigned char *>(charsPointer));
	std::string native(filePath, chars->Length);
	return filePath;
}

[STAThread]
int main() {
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	MainForm ^form = gcnew MainForm();
	
	// Prepare profile data
	Profiles^ profile = gcnew Profiles(form, gcnew String(reportFileName), gcnew String(reportFolderLocation));

	// Initiate a connection with a client
	Thread^ thread1 = gcnew Thread(gcnew ThreadStart(gcnew WinsockConnection(form, profile), &WinsockConnection::Connection));
	thread1->IsBackground = true;
	thread1->Start();

	Application::Run(form);
	return 0;
}

