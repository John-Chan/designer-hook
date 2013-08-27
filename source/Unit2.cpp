//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit2.h"   
#include "runtime_desing_control\DesignerHook.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm2 *Form2;

rdc::DesignerHook* g_hooker2=NULL;
//---------------------------------------------------------------------------
__fastcall TForm2::TForm2(TComponent* Owner)
    : TForm(Owner)
{
    //edt1->DoubleBuffered=true ;
    //this->DoubleBuffered=true ;
}
//---------------------------------------------------------------------------

void __fastcall TForm2::btn1Click(TObject *Sender)
{

    if(g_hooker2==NULL){
        g_hooker2=rdc::DesignerHook::BeginDesign(this,pnl1);
        //g_hooker2=rdc::DesignerHook::BeginDesign(this);
    }
}
//---------------------------------------------------------------------------
