#include "Profile.h"
#include <fstream>
#include <string>

const int INIT_SCORE = 1;
using namespace std;

Profiles::Profiles(MainForm^ form, String^ reportFileName, String^ reportFolderLocation)
{
	InitProfiles();
	this->form = form;
	this->fileName = reportFileName;
	this->reportFolderLocation = reportFolderLocation;
}

void Profiles::SetCurrentScore(int s)
{
	currentScore = s;
}

int Profiles::GetCurrentScore()
{
	return currentScore;
}

void Profiles::RecvReportData(String^ str)
{
	sw->WriteLine(str);
}

void Profiles::DeactivateProfileData()
{
	for (int i = 0; i < funcBool->Count; i++)
	{
		List<int>^ subB = funcBool[i];
		for (int j = 0; j < subB->Count; j++)
			subB[j] = 0;
	}

	if (currentScore == INIT_SCORE)
	{
		sw->WriteLine("This file is safe to use.\n");
		form->SetText("This file is safe to use.\n");
	}
		
	else if ((currentScore > INIT_SCORE) && (currentScore < 6))
	{
		sw->WriteLine("This file is suspected of being a threat.\n");
		form->SetText("This file is suspected of being a threat.\n");
	}
	else
	{
		sw->WriteLine("This file is a threat.\n");
		form->SetText("This file is a threat.\n");
	}

	CloseFile(this->sw);
}

void Profiles::RecvFunction(String^ s, String^ s2)
{
	for (int i = 0; i < funcName->Count; i++)
	{
		List<String^>^ subN = funcName[i];
		List<int>^ subB = funcBool[i];
		List<String^>^ subV = funcValue[i];
		for (int j = 2; j < subN->Count; j++)	//First 'j' is profile name, and second - score
			if ((subN[j]->Equals(s) && !subB[j - 2]) && ((subV[j - 2]->Equals("")) || (s2->Contains(subV[j - 2]))))	// if there is a value with the function && if s2 contains value and if functions conatins in funcName and not found before
			{
				subB[j - 2] = 1;
				subB[subB->Count - 1]++;
				j = subN->Count;
				if ((subB[subB->Count - 1] == subB->Count - 1) && (currentScore < int::Parse(subN[1])))
				{
					form->SetScore(int::Parse(subN[1]));
					currentScore = int::Parse(subN[1]);
					String^ str = "(!) Possible of malware \"" + subN[0] + "\" has been detected.\nA score " + subN[1] + " has been given to this profile.\n";
					form->SetText(str);
					sw->WriteLine(str);
				}
			}
	}
}

void Profiles::CloseFile(StreamWriter^ sw)
{
	sw->Close();
}

void Profiles::PrepareReportFile()
{
	sw = gcnew StreamWriter(String::Concat(this->reportFolderLocation, this->fileName));
	sw->WriteLine(DateTime::Now);
	sw->WriteLine("\t\t\t*** Report file ***\n");
}

void Profiles::PrepareScore()
{
	SetCurrentScore(INIT_SCORE);
	form->SetScore(GetCurrentScore());
}

void Profiles::InitProfiles()
{
	string line;
	ifstream myFile(profileFileName);
	if (myFile.is_open())
	{
		while (getline(myFile, line))
		{
			if (!line.empty() && line.at(0).Equals('#'))
			{
				List<String^>^ sub = gcnew List<String^>();
				sub->Add(gcnew String(line.substr(1).c_str()));

				getline(myFile, line);
				sub->Add(gcnew String(line.c_str()));
				funcName->Add(sub);

				List<String^>^ sub2 = gcnew List<String^>();
				funcValue->Add(sub2);
				List<int>^ bo = gcnew List<int>();
				funcBool->Add(bo);
			}
			else if (!line.empty())
			{
				if (line.find(" = ") != std::string::npos)
				{
					std::string f = line.substr(0, line.find(" = "));
					funcName[funcName->Count - 1]->Add(gcnew String(f.c_str()));

					funcBool[funcBool->Count - 1]->Add(0);

					std::string v = line.substr(line.find(" = ") + 3);
					funcValue[funcValue->Count - 1]->Add(gcnew String(v.c_str()));
				}
				else
				{
					funcName[funcName->Count - 1]->Add(gcnew String(line.c_str()));
					funcBool[funcBool->Count - 1]->Add(0);
					funcValue[funcValue->Count - 1]->Add("");
				}
			}
			else if (line.empty())
			{
				funcBool[funcBool->Count - 1]->Add(0);
			}
		}
		funcBool[funcBool->Count - 1]->Add(0);
		myFile.close();
	}
	else Profiles::form->SetText("(*) Unable to open profiles file.\n");

}