#pragma once

namespace AnomalyDetection {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for MainForm
	/// </summary>
	public ref class MainForm : public System::Windows::Forms::Form
	{
	public:
		MainForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MainForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Button^  btnBrowse;
	protected:

	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::TextBox^  txtBoxFile;

	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::GroupBox^  groupBox2;
	private: System::Windows::Forms::Button^  btnViewReport;

	private: System::Windows::Forms::TextBox^  txtScore;



	private: System::Windows::Forms::Label^  label3;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Button^  btnRun;

	private: System::Windows::Forms::Label^  label4;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::OpenFileDialog^  openFileDialog;
	private: System::Windows::Forms::RichTextBox^  txtBoxMultiline;
	protected:

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->btnBrowse = (gcnew System::Windows::Forms::Button());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->txtBoxFile = (gcnew System::Windows::Forms::TextBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->groupBox2 = (gcnew System::Windows::Forms::GroupBox());
			this->txtBoxMultiline = (gcnew System::Windows::Forms::RichTextBox());
			this->btnViewReport = (gcnew System::Windows::Forms::Button());
			this->txtScore = (gcnew System::Windows::Forms::TextBox());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->btnRun = (gcnew System::Windows::Forms::Button());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->openFileDialog = (gcnew System::Windows::Forms::OpenFileDialog());
			this->groupBox1->SuspendLayout();
			this->groupBox2->SuspendLayout();
			this->SuspendLayout();
			// 
			// btnBrowse
			// 
			this->btnBrowse->Location = System::Drawing::Point(309, 37);
			this->btnBrowse->Name = L"btnBrowse";
			this->btnBrowse->Size = System::Drawing::Size(75, 22);
			this->btnBrowse->TabIndex = 0;
			this->btnBrowse->Text = L"Browse";
			this->btnBrowse->UseVisualStyleBackColor = true;
			this->btnBrowse->Click += gcnew System::EventHandler(this, &MainForm::btnBrowse_Click);
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->txtBoxFile);
			this->groupBox1->Controls->Add(this->btnBrowse);
			this->groupBox1->Controls->Add(this->label1);
			this->groupBox1->Location = System::Drawing::Point(12, 97);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(389, 68);
			this->groupBox1->TabIndex = 1;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Scan file";
			// 
			// txtBoxFile
			// 
			this->txtBoxFile->Location = System::Drawing::Point(7, 38);
			this->txtBoxFile->Name = L"txtBoxFile";
			this->txtBoxFile->Size = System::Drawing::Size(296, 20);
			this->txtBoxFile->TabIndex = 1;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(6, 22);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(71, 13);
			this->label1->TabIndex = 3;
			this->label1->Text = L"Choose a file:";
			// 
			// groupBox2
			// 
			this->groupBox2->Controls->Add(this->txtBoxMultiline);
			this->groupBox2->Controls->Add(this->btnViewReport);
			this->groupBox2->Controls->Add(this->txtScore);
			this->groupBox2->Controls->Add(this->label3);
			this->groupBox2->Controls->Add(this->label2);
			this->groupBox2->Location = System::Drawing::Point(12, 171);
			this->groupBox2->Name = L"groupBox2";
			this->groupBox2->Size = System::Drawing::Size(389, 207);
			this->groupBox2->TabIndex = 2;
			this->groupBox2->TabStop = false;
			this->groupBox2->Text = L"Scan results";
			// 
			// txtBoxMultiline
			// 
			this->txtBoxMultiline->Location = System::Drawing::Point(10, 40);
			this->txtBoxMultiline->Name = L"txtBoxMultiline";
			this->txtBoxMultiline->ReadOnly = true;
			this->txtBoxMultiline->Size = System::Drawing::Size(293, 161);
			this->txtBoxMultiline->TabIndex = 9;
			this->txtBoxMultiline->Text = L"";
			// 
			// btnViewReport
			// 
			this->btnViewReport->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->btnViewReport->Location = System::Drawing::Point(309, 117);
			this->btnViewReport->Name = L"btnViewReport";
			this->btnViewReport->Size = System::Drawing::Size(75, 84);
			this->btnViewReport->TabIndex = 8;
			this->btnViewReport->Text = L"View Report";
			this->btnViewReport->UseVisualStyleBackColor = true;
			this->btnViewReport->Click += gcnew System::EventHandler(this, &MainForm::btnViewReport_Click);
			// 
			// txtScore
			// 
			this->txtScore->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(192)), static_cast<System::Int32>(static_cast<System::Byte>(255)),
				static_cast<System::Int32>(static_cast<System::Byte>(192)));
			this->txtScore->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 41.25F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->txtScore->Location = System::Drawing::Point(309, 40);
			this->txtScore->MaxLength = 2;
			this->txtScore->Name = L"txtScore";
			this->txtScore->ReadOnly = true;
			this->txtScore->Size = System::Drawing::Size(74, 70);
			this->txtScore->TabIndex = 7;
			this->txtScore->TextAlign = System::Windows::Forms::HorizontalAlignment::Center;
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label3->Location = System::Drawing::Point(321, 16);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(51, 20);
			this->label3->TabIndex = 5;
			this->label3->Text = L"Score";
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 12, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label2->Location = System::Drawing::Point(6, 16);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(90, 20);
			this->label2->TabIndex = 4;
			this->label2->Text = L"Information";
			// 
			// btnRun
			// 
			this->btnRun->BackColor = System::Drawing::Color::Wheat;
			this->btnRun->Font = (gcnew System::Drawing::Font(L"Bookman Old Style", 12));
			this->btnRun->Location = System::Drawing::Point(118, 384);
			this->btnRun->Name = L"btnRun";
			this->btnRun->Size = System::Drawing::Size(173, 31);
			this->btnRun->TabIndex = 3;
			this->btnRun->Text = L"Run";
			this->btnRun->UseVisualStyleBackColor = false;
			this->btnRun->Click += gcnew System::EventHandler(this, &MainForm::btnRun_Click);
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 15, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label4->Location = System::Drawing::Point(22, 28);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(96, 25);
			this->label4->TabIndex = 4;
			this->label4->Text = L"Anomaly";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 15, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(0)));
			this->label5->Location = System::Drawing::Point(22, 53);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(103, 25);
			this->label5->TabIndex = 5;
			this->label5->Text = L"Detection";
			// 
			// openFileDialog
			// 
			this->openFileDialog->FileName = L"openFileDialog";
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->BackColor = System::Drawing::SystemColors::GradientActiveCaption;
			this->ClientSize = System::Drawing::Size(408, 419);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->btnRun);
			this->Controls->Add(this->groupBox2);
			this->Controls->Add(this->groupBox1);
			this->Name = L"MainForm";
			this->Text = L"Anomaly Detection";
			this->groupBox1->ResumeLayout(false);
			this->groupBox1->PerformLayout();
			this->groupBox2->ResumeLayout(false);
			this->groupBox2->PerformLayout();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void btnBrowse_Click(System::Object^  sender, System::EventArgs^  e);
			 System::Void btnRun_Click(System::Object^  sender, System::EventArgs^  e);
			 System::Void btnViewReport_Click(System::Object^  sender, System::EventArgs^  e);
	public: void SetText(String^ string);
			void SetScore(int score);
			void ClearScreen();
			delegate void MainForm::SetTextUI(String^ string);
			delegate void MainForm::ClearScreenUI();
};
}
