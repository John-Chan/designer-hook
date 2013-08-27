//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "main.h"
#include "Unit2.h"
#include "DesignerHook.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;

TForm2* fm2=NULL;
rdc::DesignerHook* hooker=NULL;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
    : TForm(Owner)
{

}
//---------------------------------------------------------------------------
void __fastcall TForm1::btn1Click(TObject *Sender)
{
    if(fm2 == NULL){
        fm2=new TForm2(Application);
        fm2->Show();
    }
}
//---------------------------------------------------------------------------

void __fastcall TForm1::btn2Click(TObject *Sender)
{
    if(fm2 && hooker==NULL)
        hooker=rdc::DesignerHook::BeginDesign(fm2);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::btn3Click(TObject *Sender)
{
    if(hooker)
        rdc::DesignerHook::EndDesign(hooker);
}
//---------------------------------------------------------------------------

