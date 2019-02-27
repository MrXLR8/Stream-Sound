//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include "Unit1.h"
#include "Unit1.cpp"
#include "Unit2.h"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TForm2 *Form2;
extern String mode;
//---------------------------------------------------------------------------
__fastcall TForm2::TForm2(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TForm2::DeleteButtonClick(TObject *Sender)
{
Mode="";
Form2->Hide();
}
//---------------------------------------------------------------------------




void __fastcall TForm2::ConfirmButtonClick(TObject *Sender)
{ConfirmForm2();}

//---------------------------------------------------------------------------

