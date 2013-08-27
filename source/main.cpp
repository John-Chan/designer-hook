//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "main.h"
#include "Unit2.h"
#include "runtime_desing_control\DesignerHook.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;

TForm2* fm2=NULL;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
    : TForm(Owner)
{

}
//---------------------------------------------------------------------------
void __fastcall TForm1::btn1Click(TObject *Sender)
{
    fm2=new TForm2(NULL);
    //rdc::BeginDesign(fm2);
    fm2->Show();  
}
//---------------------------------------------------------------------------

void __fastcall TForm1::btn2Click(TObject *Sender)
{
    if(fm2)
        rdc::DesignerHook::BeginDesign(fm2);
}
//---------------------------------------------------------------------------

