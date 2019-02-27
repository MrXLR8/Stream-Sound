//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <VCLTee.TeeFilters.hpp>
#include <Vcl.Imaging.pngimage.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ToolWin.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.Imaging.jpeg.hpp>
#include <Vcl.ExtDlgs.hpp>
#include <vcl.h>
#include "bass.h"
#include <Vcl.JumpList.hpp>
#include <System.Win.TaskbarCore.hpp>
#include <Vcl.Taskbar.hpp>
#pragma hdrstop
#pragma link "Shlwapi.lib"
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
	TEdit *Edit1;
	TTimer *Timer1;
	TImage *PlayButton;
	TImage *StopButton;
	TListBox *ItemsHolder;
	TTimer *SlowTimer;
	TMainMenu *MainMenu1;
	TMenuItem *N1;
	TMenuItem *N2;
	TMenuItem *N3;
	TMenuItem *N4;
	TMenuItem *N5;
	TMenuItem *N6;
	TMenuItem *N7;
	TMenuItem *N8;
	TMenuItem *N9;
	TMenuItem *N10;
	TMenuItem *N11;
	TMenuItem *N12;
	TMenuItem *N13;
	TGroupBox *GroupBox1;
	TTrackBar *VolumeBar;
	TGroupBox *GroupBox3;
	TTrackBar *SoundPosBar;
	TOpenDialog *OpenDialog1;
	TStatusBar *StatusBar1;
	TBitBtn *EditBtn;
	TBitBtn *DelBtn;
	TGroupBox *PeremotkaGroup;
	TLabel *Label1;
	TLabel *Label2;
	TProgressBar *Buffer;
	TScrollBar *Peremotka;
	TBitBtn *AddBtn;
	TSaveDialog *SaveDialog1;
	TMenuItem *N14;
	TMenuItem *N15;
	TTrayIcon *TrayIcon1;
	TTaskbar *Taskbar1;
	void __fastcall Timer1Timer(TObject *Sender);
	void __fastcall Edit1Change(TObject *Sender);
	void __fastcall PlayButtonMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall PlayButtonMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall PlayButtonClick(TObject *Sender);
	void __fastcall StopButtonMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall SoundPosBarChange(TObject *Sender);
	void __fastcall PeremotkaScroll(TObject *Sender, TScrollCode ScrollCode, int &ScrollPos);
	void __fastcall SlowTimerTimer(TObject *Sender);
	void __fastcall ItemsHolderClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall N5Click(TObject *Sender);
	void __fastcall EditBtnClick(TObject *Sender);
	void __fastcall StopButtonClick(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall StopButtonMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
		  int X, int Y);
	void __fastcall DelBtnClick(TObject *Sender);
	void __fastcall AddBtnClick(TObject *Sender);
	void __fastcall N13Click(TObject *Sender);
	void __fastcall N4Click(TObject *Sender);
	void __fastcall N9Click(TObject *Sender);
	void __fastcall N11Click(TObject *Sender);
	void __fastcall N7Click(TObject *Sender);
	void __fastcall N6Click(TObject *Sender);
	void __fastcall N15Click(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall TrayIcon1Click(TObject *Sender);
	void __fastcall TrayIcon1BalloonClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall N3Click(TObject *Sender);
	void __fastcall N10Click(TObject *Sender);

private:	// User declarations
public:		// User declarations
	__fastcall TForm1(TComponent* Owner);
};
struct  MusicObject {
	unsigned int id;
	UnicodeString Name;
	UnicodeString URL;
	MusicObject *next;
	bool bad;
};


MusicObject *Send;
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
