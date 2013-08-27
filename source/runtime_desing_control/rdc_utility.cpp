//---------------------------------------------------------------------------


#pragma hdrstop

#include "rdc_utility.h"

#include <ComObj.hpp>   //for GUID
#include <Messages.hpp> // MSG
//---------------------------------------------------------------------------

#pragma package(smart_init)

namespace rdc
{

/************************************************************************
//  helper funcs
//  --------------------------------------------------------------------
//
************************************************************************/
TRect   PointToRect(const TPoint& pt1,const TPoint& pt2)
{
    TRect rect;
    if(pt1.x < pt2.x){
        rect.Left = pt1.x;
        rect.Right= pt2.x;
    }else{
        rect.Left = pt2.x;
        rect.Right= pt1.x;
    }

    if(pt1.y < pt2.y){
        rect.Top = pt1.y;
        rect.Bottom = pt2.y;
    }else{
        rect.Top = pt2.y;
        rect.Bottom = pt1.y;
    }
    return rect;
}


bool        InRect(TRect R1,TRect R2)
{
    bool ret=false;
    if( !IntersectRect(R1, R1, R2 )){
        return ret;
    }
    ret= !IsRectEmpty(R1);
    return ret;
}

AnsiString CreateGuid(void) 
{ 
    System::TGUID g;

    OleCheck (CoCreateGuid (&g)); 
    return (Sysutils::GUIDToString (g)); 
} 

bool        IsMouseMsg(unsigned Msg)
{
    return (Msg> WM_MOUSEFIRST && Msg< WM_MOUSELAST);
}
bool        IsKeyMsg(unsigned Msg)
{
    return (Msg> WM_KEYFIRST && Msg< WM_KEYLAST);
}
/************************************************************************
//  CrackComponent
//  --------------------------------------------------------------------
//
************************************************************************/
void   CrackComponent::CallSetDesigning(bool Value, bool SetChildren)
{
    SetDesigning( Value,  SetChildren);
}

/*
CrackComponent::CrackComponent(TComponent* Owner)
:TComponent(Owner)
{

}
CrackComponent::~CrackComponent()
{
    //
}
*/

/************************************************************************
//  CrackControl
//  --------------------------------------------------------------------
//
************************************************************************/
void   CrackControl::CallSetDesigning(bool Value, bool SetChildren)
{
    SetDesigning( Value,  SetChildren);
}

/*
CrackControl::CrackControl(Classes::TComponent* Owner)
:TControl(Owner)
{
    //
}
CrackControl::~CrackControl()
{
    //
}
*/
}
