#include <vcl.h>
#include "bass.h"
#pragma hdrstop
#include "Unit1.h"
#include "Unit2.h"
#include "Unit3.h"
#include "Unit4.h"
#include <iostream>
#include <iostream.h>
#include <fstream>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <stdio.h>
#include <sysmac.h>
#include <string>
#include <clocale>
#include <locale>
#include <windows.h>
#include <algorithm>
#include <math.h>
USEFORM("Unit2.cpp",Form2);
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma comment (lib, "bass.lib")
#pragma resource "*.dfm"

TForm1 *Form1;
HSTREAM stream;
HCHANNEL chanel;
int Playing=false;
int IDPlaying=0;
float volume=5;
String Mode="";
int progress;
int selected;
String PlayState;
void BlackOut();

//=====================================================
String Sgetline(ifstream* &File) {
	char read[256];
	String result;
	File->getline(read,256);
	result=read;
	return result;
}

//=====================================================
void Swriteline(ofstream* &Target,String towrite) {
char WriteVal[256];
AnsiString sText = towrite;
strcpy( WriteVal, sText.c_str() );
Target->write(WriteVal,towrite.Length());
sText="\n";
strcpy( WriteVal, sText.c_str() );
Target->write(WriteVal,sText.Length());
}

	class Playlist {
	public:
		String PlaylistName;
		unsigned int records;
		String FilePath;
		MusicObject* SearchID(int); // ищет обьект в стеке
		void CountRecords(); //подсчитывает количество записей в файле
		void OutRecords();  // выводит все
		void DeleteID(int); // удаляет обьект из стека и пересчитывает ИД
		void Wipe();       // стриает все в этом плейлисте
		void FillFromFile(); //заполняет стек
		void New(String,String);        // новый плейлист
		void Add(MusicObject*); // новый музыкальный обьект
		void Save();       // сохраняет с окном выбора
		void Save(String);  // сохраняет в выбраном пути без окна
		Playlist() {
            head=new MusicObject;
        }
	private:
		MusicObject *head;
		ifstream *File;
		MusicObject Fill(int);  // возвращяет обьект  по нужному поиску и присваивает ему ID
		void RebuildID();   // производит пересчет ID
		String SearchParametr(String);  // ищет в файле строку, и выделяет нужное
		void WriteToFile(ofstream*&);   // записывает весь стек в файл
};
Playlist Main;

using namespace std ;



 void NameChange(String ToChange) {
String text;
    Main.PlaylistName=ToChange;
	text+="Stream `n Sound By (Fedorov A). Playlist: ";
	text+=Main.PlaylistName;
	Form1->Caption=text;

}
//=====================================================
void __fastcall TForm1::SlowTimerTimer(TObject *Sender)
{

	if(Main.records!=0) {
		N2->Enabled=true;
	}
	if(selected>0) {
		EditBtn->Enabled=true;
		N10->Enabled=true;
		N13->Enabled=true;
		DelBtn->Enabled=true;
	}
	else {
		EditBtn->Enabled=false;
		N10->Enabled=false;
		N13->Enabled=false;
		DelBtn->Enabled=false;

	}
 }
 //=====================================================
 void Playlist::Save() {
 String TargetPath;
 String EpicText;
 ofstream *targetfile=new ofstream;
	Form1->SaveDialog1->Filter="Плейлисты (.pls) |*.pls";
	Form1->SaveDialog1->DefaultExt=".pls";
	Form1->SaveDialog1->FileName=PlaylistName;

	if(Form1->SaveDialog1->Execute()) {
		TargetPath=Form1->SaveDialog1->FileName;
		targetfile->open((AnsiString(TargetPath).c_str()),ios::trunc);
		EpicText=String("Плейлист с именем: ")+PlaylistName+String(" - был успешно сохранен в файл ")+ExtractFileName(TargetPath);
		if(targetfile->is_open())
			Swriteline(targetfile,"[playlist]");
			WriteToFile(targetfile);
			MessageBox(NULL,AnsiString(EpicText).c_str() ,"Сохранение файла удалось",MB_ICONINFORMATION|MB_TASKMODAL);
			Main.FilePath=TargetPath;
			targetfile->close();
	}
	else {MessageBox(NULL, "Точка сохранения плейлиста не выбрана или недоступна","Сохранение файла провалилось",MB_ICONINFORMATION|MB_TASKMODAL);  return; }

 }
//=====================================================
  void Playlist::Save(String TargetPath) {
 String EpicText;
 ofstream *targetfile=new ofstream;
 targetfile->open((AnsiString(TargetPath).c_str()),ios::trunc);
	if(!targetfile->fail()) {

		EpicText=String("Плейлист с именем: ")+PlaylistName+String(" - был успешно сохранен в файл ")+ExtractFileName(TargetPath);
		if(targetfile->is_open())
			Swriteline(targetfile,"[playlist]");
			WriteToFile(targetfile);
			MessageBox(NULL,AnsiString(EpicText).c_str() ,"Сохранение файла удалось",MB_ICONINFORMATION|MB_TASKMODAL);
			Main.FilePath=TargetPath;
			targetfile->close();
	}
	else {MessageBox(NULL, "Точка сохранения плейлиста не выбрана или не доступна","Сохранение файла провалилось",MB_ICONINFORMATION|MB_TASKMODAL);  return; }

 }
//=====================================================
void Playlist::WriteToFile(ofstream* &filesave) {
MusicObject* current;
current=head->next;
		for (int writeCyc=1; writeCyc <= records; writeCyc++) {
			Swriteline(filesave,String("Title")+String(current->id)+String("=")+current->Name);
			Swriteline(filesave,String("File")+String(current->id)+String("=")+current->URL);
			current=current->next;


		}
}
//=====================================================
 void Playlist::Add(MusicObject *ToAdd) {
	 MusicObject *Last;
	 if(records==0) {
	 	head=new MusicObject;
		 Last=head;
		 ToAdd->id=1;
	 }
	 else{
	 Last=SearchID(records);
	 ToAdd->id=((Last->id)+1);
	 }

	 Last->next=ToAdd;
	 records++;
	 RebuildID();
	 OutRecords();
	 Form1->StopButton->Enabled=true;
	 Form1->PlayButton->Enabled=true;
	 Form1->ItemsHolder->Enabled=true;
	 Form1->N6->Enabled=true;
	 Form1->N7->Enabled=true;

 }
//=====================================================
 void Playlist::New(String PlayName,String InFilePath) {
 String FormCaption;
	BASS_StreamFree(stream);
	Playing=0;
	FormCaption+="Stream `n Sound By (Fedorov A). Playlist: ";
	 BlackOut();
	 PlaylistName=PlayName;
	 FilePath=InFilePath;
	 FormCaption+=PlayName;
	 Form1->Caption=FormCaption;

 }
//=====================================================
void Playlist::RebuildID() {
MusicObject* Reading;
int count;
	Reading=head->next;
	for(count=1;count<=records;count++)
	{
		Reading->id=count;
		Reading=Reading->next;
	}
}
//=====================================================
void Playlist::OutRecords() {
	MusicObject *q;
	Form1->ItemsHolder->Clear();
	int count;
		q=head->next;
		Form1->N10->Clear();
		Form1->ItemsHolder->Clear();
		Form1->ItemsHolder->Items->Add("Cписок музыкальных объектов:");
	for(count=1;count<=records;count++) {
		Form1->ItemsHolder->Items->Add(q->Name);
		q=q->next;
	}
}
//=====================================================
void Playlist::DeleteID(int id) {
MusicObject* prev;
MusicObject* tobedeleted;
MusicObject* temp;
if(id==IDPlaying) {
	Playing=1;
	Form1->StopButtonClick(Form1);
	Form1->PeremotkaGroup->Visible=false;
	Form1->StatusBar1->Panels->Items[0]->Text=String("Остановленно");
	Form1->Taskbar1->ProgressState=0;
	BASS_StreamFree(stream);
	Form1->Height=340;
}
	if(id<=records) {
		if(id==1) {
			prev=head;
		}
		else {
			prev=SearchID((id-1));
			}
	tobedeleted=SearchID(id);
	prev->next=tobedeleted->next;
	records--;
	RebuildID();
	OutRecords();
		if(records==0) {
	Form1->N6->Enabled=false;
	Form1->N7->Enabled=false;
}
}

}
//=====================================================
MusicObject* Playlist::SearchID(int IDReq) {
unsigned int cycle=1;
int IDobject;
MusicObject* Checking=new MusicObject;
	Checking=head;
	do{
		Checking=Checking->next;
		IDobject=Checking->id;
		if(cycle>records) {
			ShowMessage("Что то пошло не так. Количество попыток больше чем самая большая запись");
			break;
		}
	cycle++;
		}  while(IDobject!=IDReq);

return Checking;
}
//=====================================================
void Playlist::FillFromFile() {
unsigned int count=1;
head=new MusicObject;
MusicObject* q=new MusicObject;
MusicObject t;
	head->next=q;
	for(count=1;count<=records;count++) {
		t=Fill(count);
		if(!t.bad) {
			q->id=t.id;
			q->Name=t.Name;
			q->URL=t.URL;
			q->bad=t.bad;
			q->next=new MusicObject;
			}
		q=q->next;
	}
	OutRecords();
	Form1->ItemsHolder->Enabled=True;
	Form1->N6->Enabled=true;
	Form1->N7->Enabled=true;

	Form1->SlowTimerTimer(Form1);
}
//=====================================================
void Playlist::CountRecords() {
int max=0;
String SearchLabel;
String SearchResult;
	do{
		max++;
		SearchLabel="File"; SearchLabel+=max;
		SearchResult= SearchParametr(SearchLabel);
		} while(SearchResult!="===NOTHING===");
	max--;
	records=max;
}
//=====================================================
MusicObject Playlist::Fill (int id) {
MusicObject Returning;
String SearReq;
String func;
int Clen;
	Returning.id=id;
	SearReq="File"; SearReq+=id;
	func=SearchParametr(SearReq);
	if(func=="===NOTHING===") {
		Returning.bad=true;
		return Returning;
	}
	Returning.URL=func;
	SearReq="Title"; SearReq+=id;
	func=SearchParametr(SearReq);
	if(func=="===NOTHING===") {
		Clen=Returning.URL.Length();
		Returning.Name=Returning.URL.SubString(Clen-17,Clen);
	}
	else{Returning.Name=func;}
	Returning.bad=false;
	Returning.id=id;
	return Returning;
}
//=====================================================
String Playlist::SearchParametr(String Par) {
delete File;
File=new ifstream;
	File->open(AnsiString(FilePath).c_str());
int dlina;
	Par=Par+"=";
	dlina=Par.Length();
String line;
String Cutted;
String result;
int LineLen;
int moretest;
bool Cycle;
bool SEOF;
int tries=0;
		do {
			tries++;
			SEOF=File->eof();
			if(SEOF) {File->close(); return "===NOTHING===";}
			line=Sgetline(File);
			LineLen=line.Length();
			Cutted= line.SubString(0,Par.Length());
			Cycle=(Cutted!=Par);
		}while (Cycle);
		if (LineLen>Par.Length()) {
			result= line.SubString((dlina+1),256);
			return result;
		}
		else{File->close(); return "===NOTHING===";}
        File->close();
		return "===NOTHING===";

}
//=====================================================
void Playlist::Wipe() {
	for (int count=records;count>0; count--) {
		DeleteID(records);
	}
	head->next=NULL;
	PlaylistName="";
	FilePath="";
	Form1->N6->Enabled=false;
	Form1->N7->Enabled=false;
	}
//=====================================================
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}
 void PlayButtonSet (String state) {
	if(Form1->Edit1->Text=="") {
		Form1->PlayButton->ShowHint=True;
		Form1->PlayButton->Picture->LoadFromFile("recourses\\img\\false_play.png");
		}
	else{
		Form1->PlayButton->Picture->LoadFromFile("recourses\\img\\"+state+"_play.png");
		Form1->PlayButton->ShowHint=False;

}}
//=====================================================
  void StopButtonSet (String state) {
	if(Playing==0)
	{
		if(BASS_ChannelBytes2Seconds(stream,Form1->Peremotka->Max)==-1)  {
			Form1->StopButton->Picture->LoadFromFile("recourses\\img\\false_stop.png");
		}
	}
	else{
	Form1->StopButton->Picture->LoadFromFile("recourses\\img\\"+state+"_stop.png");
	}
}
//=====================================================
String TimeTransf(int seconds) {
int minutes=0;
String Result;
	if(seconds>0) {
		while(seconds>59) {
			minutes++;
			seconds-=60;
		}
	}
	Result+=minutes/10;
	Result+=minutes%10;
	Result+=":";
	Result+=seconds/10;
	Result+=seconds%10;
	return Result;
}
//=====================================================
void __fastcall TForm1::Timer1Timer(TObject *Sender) {
int len=BASS_StreamGetFilePosition(stream, BASS_FILEPOS_END);
int buf=BASS_StreamGetFilePosition(stream, BASS_FILEPOS_BUFFER);
	BASS_ChannelSetAttribute(stream,BASS_ATTRIB_VOL,volume);
	volume= ((VolumeBar->Max)/10)-((VolumeBar->Position)/(VolumeBar->Max*1.0));
	progress=buf*100.0/len;
	Buffer->Position=progress;
	if(Playing==1) {
		Peremotka->Position=BASS_ChannelGetPosition(stream,0);
  }
  if(BASS_ChannelGetLength(stream,0)-1 <10000000000) {
		Taskbar1->ProgressValue=BASS_ChannelGetPosition(stream,0);
  }
		Label1->Caption=TimeTransf(BASS_ChannelBytes2Seconds(stream,Peremotka->Position));

}
//=====================================================
void __fastcall TForm1::Edit1Change(TObject *Sender)
{
	if(Edit1->Text=="") {
		PlayButton->ShowHint=True;
		PlayButton->Picture->LoadFromFile("recourses\\img\\false_play.png");
	}
	else{
		PlayButton->Picture->LoadFromFile("recourses\\img\\on_play.png");
		PlayButton->ShowHint=False;
}
}
//=====================================================
void __fastcall TForm1::PlayButtonMouseDown(TObject *Sender, TMouseButton Button,
		  TShiftState Shift, int X, int Y)
{
	PlayButtonSet("off");
}
//---------------------------------------------------------------------------

void __fastcall TForm1::PlayButtonMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
		  int X, int Y)
{
	PlayButtonSet("on");
}

//---------------------------------------------------------------------------
void ConfirmForm2 () {

int Clen;
Clen=Form2->URLText->Text.Length();
	if(Clen>0) {
		if (Mode.Compare("edit")==0) {
			if(Form2->TitleEdit->Text.Length()==0) {
				Form2->TitleEdit->Text=Form2->URLText->Text.SubString(Clen-17,Clen);
			}
		Send->Name= Form2->TitleEdit->Text;
		Send->URL= Form2->URLText->Text;
		}
		else if(Mode.Compare("add")==0) {
			if(Form2->TitleEdit->Text.Length()==0) {
				Form2->TitleEdit->Text=Form2->URLText->Text.SubString(Clen-17,Clen);
			}
		Send->Name= Form2->TitleEdit->Text;
		Send->URL= Form2->URLText->Text;
		Main.Add(Send);

        }
		}
	else{MessageBox(NULL, "Поле - Адрес (URL) обязательно для заполнения","Поле не заполнено",MB_ICONWARNING|MB_TASKMODAL);return;}
	Form1->ItemsHolder->Clear();
	Main.OutRecords();
	Form2->Hide();
	selected=0;
	Form1->SlowTimerTimer(Form1);
}

//---------------------------------------------------------------------------
void __fastcall TForm1::PlayButtonClick(TObject *Sender)
{
int ErrCode;
String Text;
	Text=Edit1->Text;
	if(selected>0) {
	if(Playing!=3|selected!=IDPlaying) {
		BASS_Start();
		BASS_ChannelStop(chanel);
		BASS_StreamFree(stream);
		char *url=AnsiString(Text).c_str();
		stream=BASS_StreamCreateURL(url, 0, BASS_STREAM_STATUS, NULL, 0);
		ErrCode=BASS_ErrorGetCode();
		if(ErrCode!=0) {

			StatusBar1->Panels->Items[0]->Text=String(Main.SearchID(selected)->Name+" не может быть воспроизведена. проверьте правильность.") ;
			Taskbar1->ProgressValue=Taskbar1->ProgressMaxValue;
			Taskbar1->ProgressState=3;
			StopButtonSet("false");
			StopButton->Enabled=false;
			return;

		}
		chanel=BASS_ChannelPlay(stream,TRUE);  // только в новом потоке (после очистки)
		Playing=1;
		TrayIcon1->BalloonTitle="Воспроизведение:";
		TrayIcon1->BalloonHint= Main.SearchID(selected)->Name;
		TrayIcon1->ShowBalloonHint();
		StatusBar1->Panels->Items[0]->Text=String("Воспроизведение: ("+Main.SearchID(selected)->Name+")") ;
		Taskbar1->ProgressValue=Taskbar1->ProgressMaxValue;
		Taskbar1->ProgressState=1;
		StopButton->Enabled=true;
	}
	if(Playing==3) {
		BASS_Start();
		Playing=1;
		StopButtonSet("on");
		StopButton->Enabled=true;
		IDPlaying= ItemsHolder->ItemIndex;
		Taskbar1->ProgressState=2;
		StatusBar1->Panels->Items[0]->Text=String("Воспроизведение: ("+Main.SearchID(selected)->Name+")") ;
		return;
	}
	SlowTimer->OnTimer(Form1);
	IDPlaying= ItemsHolder->ItemIndex;
	if(Playing) {

		if(BASS_ChannelGetLength(stream,0)-1 <10000000000) {
			Peremotka->Max=BASS_ChannelGetLength(stream,0)-1 ;
			Taskbar1->ProgressMaxValue=BASS_ChannelGetLength(stream,0)-1 ;
			PeremotkaGroup->Visible=True;
			Form1->Height=430;
			Taskbar1->ProgressState=2;
		}
		else{PeremotkaGroup->Visible=false;Form1->Height=340;	Taskbar1->ProgressState=1;}
	}
	Label2->Caption=TimeTransf(BASS_ChannelBytes2Seconds(stream,Peremotka->Max));
	Playing=1;
	StopButtonSet("on");
 }}
//=====================================================
void __fastcall TForm1::StopButtonMouseDown(TObject *Sender, TMouseButton Button,
		  TShiftState Shift, int X, int Y)
{
	StopButtonSet("off");
}
//=====================================================
void __fastcall TForm1::SoundPosBarChange(TObject *Sender)
{
float soundPos;
	soundPos= ((SoundPosBar->Position))/10.0;
	BASS_ChannelSetAttribute(stream,BASS_ATTRIB_PAN,soundPos);
}
//=====================================================
void __fastcall TForm1::PeremotkaScroll(TObject *Sender, TScrollCode ScrollCode, int &ScrollPos)
{
	BASS_Pause();
	BASS_ChannelSetAttribute(stream,BASS_ATTRIB_VOL,0);
	BASS_ChannelSetPosition(stream, Peremotka->Position, 0);
	if(Playing==1) {
		BASS_Start();
	}
	else{BASS_Pause();}
}
//=====================================================
String Converter(string Normal) {
	String result;
	result=Normal.c_str();
	return result;
}
//=====================================================

void __fastcall TForm1::ItemsHolderClick(TObject *Sender)
{
	if(selected== ItemsHolder->ItemIndex) { return; }
	selected=ItemsHolder->ItemIndex;
	if(selected>0) {
		Edit1->Text=Main.SearchID(ItemsHolder->ItemIndex)->URL;
		EditBtn->Enabled=true;
		DelBtn->Enabled=true;
		N10->Enabled=true;

	}

}
//=====================================================
 void BlackOut() {
	 Form1->Caption="Stream `n Sound. (Fedorov A.)";

	 Form1->N2->Enabled=false;
	 if(Playing>0) {
	 Form1->StopButtonClick(Form1);
	 Playing=0;
	 }
	 StopButtonSet("false");
	 Form1->StopButton->Enabled=false;
	 Form1->PlayButton->Enabled=false;
	 Form1->PeremotkaGroup->Visible=false;
	 Form1->Height=340;
	 Form1->N10->Clear();
	 Form1->ItemsHolder->Clear();
	 Form1->ItemsHolder->Items->Add("Список музыкальных объектов:");
	 Form1->ItemsHolder->Enabled=false;
	 Form1->EditBtn->Enabled=false;
	 Form1->DelBtn->Enabled=false;
	 Form1->N10->Enabled=false;
	 Form1->Edit1->Text="";
	 IDPlaying=0;
	 selected=0;
	 Form1->Taskbar1->ProgressState=0;
	 if(Main.records>0) {
	 Main.Wipe();
	 }
	 Send=NULL;
 }
//=====================================================
void __fastcall TForm1::FormCreate(TObject *Sender)
{
	Form1->N6->Enabled=false;
	Form1->N7->Enabled=false;
	setlocale(LC_ALL,"Russian");
	VolumeBar->Position=volume;
	if(!BASS_Init (-1, 44100, BASS_DEVICE_3D , 0, NULL))
		{MessageBox(NULL,"Инициализия библиотеки BASS не удалось. Дальнейшое продолжение роботы невозможно","BASS провалился",MB_ICONERROR);
		Application->Terminate();
		}
	else{StatusBar1->Panels->Items[0]->Text="Ждем ввод";}
	if(Edit1->Text=="") {
			PlayButton->ShowHint=True;
			PlayButton->Picture->LoadFromFile("recourses\\img\\false_play.png");
	}
	else{
		PlayButton->Picture->LoadFromFile("recourses\\img\\on_play.png");
		PlayButton->ShowHint=False;

		}
	BlackOut();
}
//=====================================================
void __fastcall TForm1::N5Click(TObject *Sender)
{
	OpenDialog1->Filter="Плейлисты (.pls) |*.pls";
	if (OpenDialog1->Execute()) {
	BlackOut();
		Main.FilePath=OpenDialog1->FileName;
ifstream *test=new ifstream;
		test->open(AnsiString(Main.FilePath).c_str());
		if(Sgetline(test)!="[playlist]"|test->fail()) {
			test->close();
			MessageBox(NULL, "Файл поврежден","Выбран неверный файл",MB_ICONWARNING|MB_TASKMODAL);  Form2->Hide();
			return;
			}
	Main.New(ExtractFileName(Main.FilePath),Main.FilePath);
	StopButton->Enabled=true;
	PlayButton->Enabled=true;
	Main.CountRecords();
	Main.FillFromFile();

	TrayIcon1->BalloonTitle="Stream `n Sound";
	TrayIcon1->BalloonHint=String("Плейлист "+ExtractFileName(Main.FilePath)+" подключен. Кол-во записей: "+Main.records);
	TrayIcon1->ShowBalloonHint();
	}
	else {MessageBox(NULL, "Файл не выбран","Выбор файла был прерван",MB_ICONINFORMATION|MB_TASKMODAL);  Form2->Hide(); }
}
//=====================================================
void __fastcall TForm1::EditBtnClick(TObject *Sender)
{
	String PlusPlus="Редактирование: ";
	if(selected>0) {
		Mode="edit";
		Send=Main.SearchID(selected);
		PlusPlus+=Send->Name;
		Form2->GroupBox1->Caption=PlusPlus;
		Form2->Caption=PlusPlus;
		Form2->TitleEdit->Text=Send->Name;
		Form2->URLText->Text=Send->URL;
		Form2->Show();
	}
	else {Mode="";}
}
//=====================================================
void __fastcall TForm1::StopButtonClick(TObject *Sender)
{
	if(BASS_ChannelGetLength(stream,0)-1 <10000000000) {
		BASS_Pause();
		Playing=3;
		Taskbar1->ProgressState=4;
		StatusBar1->Panels->Items[0]->Text=String("("+Main.SearchID(IDPlaying)->Name+") на Паузе");
	}
	else{
		BASS_StreamFree(stream);
		Playing=0;
		Taskbar1->ProgressState=0;
		StatusBar1->Panels->Items[0]->Text="Остановлено";
		}
	StopButtonSet("false");
	StopButton->Enabled=false;
}
//=====================================================
void __fastcall TForm1::Button2Click(TObject *Sender)
{
	Main.Wipe();
}
//=====================================================
void __fastcall TForm1::Button1Click(TObject *Sender)
{
	Main.OutRecords();
}
//=====================================================
void __fastcall TForm1::StopButtonMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
		  int X, int Y)
{
	if(Playing==1) {
		StopButtonSet("on");
	}
	else if(Playing==0) {StopButtonSet("false");}
}
//=====================================================
void __fastcall TForm1::DelBtnClick(TObject *Sender)
{
if (selected>0) {
	Main.DeleteID(selected);
	selected=0;
}
SlowTimerTimer(Form1);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::AddBtnClick(TObject *Sender)
{
	int dialog;
	String PlusPlus="Добавление нового элемента";
	if(Main.PlaylistName.Length()>0) {
		Mode="add";
		Send=new MusicObject;
		Form2->GroupBox1->Caption=PlusPlus;
		Form2->Caption=PlusPlus;
		Form2->TitleEdit->Text="";
		Form2->URLText->Text="";
		Form2->Show();
	}
	else {
	dialog = MessageBox(NULL,"Необходимо сперва создать/загрузить плейлист. Создать новый плейлист?", "Процедура добавления", MB_YESNO|MB_ICONQUESTION|MB_TASKMODAL);
		if(dialog==6) {
			Main.New("NewPlaylist",String());
			AddBtnClick(Form1);
		}
		}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::N13Click(TObject *Sender)
{
EditBtnClick(Form1);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::N4Click(TObject *Sender)
{
Main.New("New Playlist",String());
}
//---------------------------------------------------------------------------
void __fastcall TForm1::N9Click(TObject *Sender)
{
Form1->AddBtnClick(Form1);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::N11Click(TObject *Sender)
{
BlackOut();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::N7Click(TObject *Sender)
{
Main.Save();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::N6Click(TObject *Sender)
{
if(!Main.FilePath.IsEmpty()) {
	Main.Save(Main.FilePath);
}
else{ 	Main.Save();}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::N15Click(TObject *Sender)
{
Form3->Show();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormResize(TObject *Sender)
{
if (Form1->WindowState == wsMinimized) {
	Form1->Hide();
	Form2->Hide();
	TrayIcon1->BalloonTitle="Stream `n Sound (Свернут)";
	TrayIcon1->BalloonHint="для открытия нажмите по иконке";
	TrayIcon1->ShowBalloonHint();
}
}
//---------------------------------------------------------------------------
void __fastcall TForm1::TrayIcon1Click(TObject *Sender)
{
Form1->Show();
Application->Restore();
	if(Playing==1|Playing==3) {

		if(BASS_ChannelGetLength(stream,0)-1 <10000000000) {
			Taskbar1->ProgressMaxValue=BASS_ChannelGetLength(stream,0)-1 ;
			Taskbar1->ProgressState=2;
		}
		else {
		Taskbar1->ProgressMaxValue=10;
		Taskbar1->ProgressValue=10;
		Taskbar1->ProgressState=1;}
		}


}
//---------------------------------------------------------------------------
void __fastcall TForm1::TrayIcon1BalloonClick(TObject *Sender)
{
  TrayIcon1Click(Form1);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
TrayIcon1->Visible=false;
BlackOut();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::N3Click(TObject *Sender)
{

Form4->Show();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::N10Click(TObject *Sender)
{
if (selected>0) {
	Main.DeleteID(selected);
	selected=0;
}
Edit1->Text="";
SlowTimerTimer(Form1);
}
