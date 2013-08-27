//---------------------------------------------------------------------------


#pragma hdrstop

#include "GrabHandle.h"
#include "DesignerHook.h" //DesignerHook
#include "rdc_utility.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

namespace rdc
{
/************************************************************************
//  GrabHandleManager
//  --------------------------------------------------------------------
//
************************************************************************/
__fastcall GrabHandleManager::GrabHandleManager(DesignerHook* Hooker)
:TComponent(NULL) , Hooker_(Hooker)
{
    //
}
__fastcall GrabHandleManager::~GrabHandleManager()
{
    //
}
DesignerHook*  GrabHandleManager::GetDesigner()
{
    return Hooker_;
}

/************************************************************************
//  GrabHandle
//  --------------------------------------------------------------------
//
************************************************************************/
static const int GrabHandleSize=4;

__fastcall GrabHandle::GrabHandle(/*TComponent* GrabManager*/GrabHandleManager* GrabManager, TControl* Control,GrabHandleDirect Direct)
:TCustomControl(GrabManager),
GrabManager_(GrabManager),
Control_(Control),
Direct_(Direct)//,FDesigner()
{
    Hooker_=GrabManager_->GetDesigner();
    this->Visible=false ;
    this->Parent= Control_->Parent;
    Color=  clBlack;
    Pos();
}
__fastcall GrabHandle::~GrabHandle()
{
    //
}

void    GrabHandle::Pos()
{
    int X[3];
    int Y[3];
    X[0] = Control_->Left - GrabHandleSize / 2;
    X[1] = Control_->Left + (Control_->Width - GrabHandleSize) / 2;
    X[2] = Control_->Left + Control_->Width - GrabHandleSize / 2;
    Y[0] = Control_->Top - GrabHandleSize / 2;
    Y[1] = Control_->Top + (Control_->Height - GrabHandleSize) / 2;
    Y[2] = Control_->Top + Control_->Height - GrabHandleSize / 2;
  
    switch(Direct_)
    {
    case fdLeftUp: 
        Cursor = crSizeNWSE;
        SetBounds(X[0], Y[0], GrabHandleSize, GrabHandleSize);
        break;
    case fdUp:  
        Cursor = crSizeNS;
        SetBounds(X[1], Y[0], GrabHandleSize, GrabHandleSize);
        break;
    case fdRightUp: 
        Cursor = crSizeNESW;
        SetBounds(X[2], Y[0], GrabHandleSize, GrabHandleSize);
        break;
    case fdRight: 
        Cursor = crSizeWE;
        SetBounds(X[2], Y[1], GrabHandleSize, GrabHandleSize);
        break;
    case fdRightDown:  
        Cursor = crSizeNWSE;
        SetBounds(X[2], Y[2], GrabHandleSize, GrabHandleSize);
        break;
    case fdDown: 
        Cursor = crSizeNS;
        SetBounds(X[1], Y[2], GrabHandleSize, GrabHandleSize);
        break;
    case fdLeftDown: 
        Cursor = crSizeNESW;
        SetBounds(X[0], Y[2], GrabHandleSize, GrabHandleSize);
        break;
    case fdLeft: 
        Cursor = crSizeWE;
        SetBounds(X[0], Y[1], GrabHandleSize, GrabHandleSize);
        break;
    } 
    if (GrabManager_->GetDesigner()->ControlCount > 1){
      Cursor = crDefault;
    }
    BringToFront();
}
DesignerHook*  GrabHandle::GetDesigner()
{
    return GrabManager_->GetDesigner();
}
void    __fastcall GrabHandle::MouseDown(TMouseButton Button, Classes::TShiftState Shift, int X, int Y)
{
    //TCustomControl* pSuper= dynamic_cast<TCustomControl>(this);
    //pSuper->MouseDown(Button,Shift, X, Y);
    TCustomControl::MouseDown(Button,Shift, X, Y);
    
    if(GetDesigner()->ControlCount > 1){
        return ;
    }
    GetDesigner()->Dragging=true;
    GetDesigner()->FBeforDragPos = ClientToScreen(Point(X, Y));
    MouseCapture = true;
    /*
    inherited MouseDown(Button, Shift, X, Y);
  if GetDesigner()->ControlCount > 1 then
    Exit;
  GetDesigner()->Dragging := True;
  GetDesigner()->FBeforDragPos := ClientToScreen(Point(X, Y));
  MouseCapture := True;
  
  */
  
}
void    __fastcall GrabHandle::MouseUp(TMouseButton Button, Classes::TShiftState Shift, int X, int Y)
{
    //TCustomControl* pSuper= dynamic_cast<TCustomControl>(this);
    //pSuper->MouseUp(Button,Shift, X, Y);
    TCustomControl::MouseUp(Button,Shift, X, Y);

    GetDesigner()->Dragging=false;
    /*
    inherited MouseUp(Button, Shift, X, Y);
    MouseCapture := False;
    GetDesigner()->Dragging := False;
    */
}
void    __fastcall GrabHandle::MouseMove( Classes::TShiftState Shift, int X, int Y)
{
    int   ControlIndex=0;
    TPoint  cPos;
    int     cX,cY;

    //TCustomControl* pSuper= dynamic_cast<TCustomControl>(this);
    //pSuper->MouseMove(Shift, X, Y);
    TCustomControl::MouseMove(Shift, X, Y);

    if(!GetDesigner()->Dragging){
        return;
    }
    cPos =  ClientToScreen(Point(X, Y));

    for(;ControlIndex < GetDesigner()->ControlCount - 1;++ControlIndex){
        Hooker_->Controls[ControlIndex].Left = Hooker_->Controls[ControlIndex].Left + (X - Hooker_->FBeforDragPos.X);
        Hooker_->Controls[ControlIndex].Top = Hooker_->Controls[ControlIndex].Top + (Y - Hooker_->FBeforDragPos.Y);
    }

    cX = Hooker_->FBeforDragPos.X - CPos.X;
    cY = Hooker_->FBeforDragPos.Y - CPos.Y;
    if(Abs(cX) < 2 && Abs(cY) < 2){
        return;
    }

    switch(FDirect)
    {
    case fdLeftUp:
        if(Control_->Width + cX > 1){
          Control_->Left = Control_->Left - cX;
          Control_->Width = Control_->Width + cX;
          GetDesigner()->FBeforDragPos.X = CPos.X;
        }
        if(Control_->Height + cY > 1 ){
            Control_->Top = Control_->Top - cY;
            Control_->Height = Control_->Height + cY;
            GetDesigner()->FBeforDragPos.Y = CPos.Y;
        }

        break;
    case fdUp:
        if(Control_->Height + cY > 1){
            Control_->Top = Control_->Top - cY;
            Control_->Height = Control_->Height + cY;
            GetDesigner()->FBeforDragPos.Y = CPos.Y;
        }
        break;
    case fdRightUp:
        if(Control_->Width - cX > 1){
            
          Control_->Width = Control_->Width - cX;
          GetDesigner()->FBeforDragPos.X = CPos.X;
        } 
        if(Control_->Height + cY > 1){
            
          Control_->Top = Control_->Top - cY;
          Control_->Height = Control_->Height + cY;
          GetDesigner()->FBeforDragPos.Y = CPos.Y;
        }
        
        break;
    case fdRight:
        if(Control_->Width - cX > 1){
            
          Control_->Width = Control_->Width - cX;
          GetDesigner()->FBeforDragPos.X = CPos.X;
        }
        
        break;
    case fdRightDown:
        if(Control_->Width - cX > 1){
          Control_->Width = Control_->Width - cX;
          GetDesigner()->FBeforDragPos.X = CPos.X;
        }  
        if(Control_->Height - cY > 1){
          Control_->Height = Control_->Height - cY;
          GetDesigner()->FBeforDragPos.Y = CPos.Y;
        }
        
        break;
    case fdDown: 
        if(Control_->Height - cY > 1){
            
          Control_->Height = Control_->Height - cY;
          GetDesigner()->FBeforDragPos.Y = CPos.Y;
        }
        
        break;
    case fdLeftDown: 
        if(Control_->Width + cX > 1 ){
          Control_->Left = Control_->Left - cX;
          Control_->Width = Control_->Width + cX;
          GetDesigner()->FBeforDragPos.X = CPos.X;
        }
        if(Control_->Height - cY > 1){
          Control_->Height = Control_->Height - cY;
          GetDesigner()->FBeforDragPos.Y = CPos.Y;
        }
        break;
    case fdLeft:
        if(Control_->Width + cX > 1){
          Control_->Left = Control_->Left - cX;
          Control_->Width = Control_->Width + cX;
          GetDesigner()->FBeforDragPos.X = CPos.X;
        }
        break;
    }

}

}//namespace
