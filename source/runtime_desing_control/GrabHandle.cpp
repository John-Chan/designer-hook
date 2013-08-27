//---------------------------------------------------------------------------


#pragma hdrstop

#include "GrabHandle.h"
#include "DesignerHook.h" //DesignerHook
#include "rdc_utility.h" 
#include "Setting.h"
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


__fastcall GrabHandle::GrabHandle(/*TComponent* GrabManager*/GrabHandleManager* GrabManager, TControl* Control,GrabHandleDirect Direct)
:TCustomControl(GrabManager),
GrabManager_(GrabManager),
Control_(Control),
Direct_(Direct)//,FDesigner()
{
    Hooker_=GrabManager_->GetDesigner();
    this->Visible=false ;
    this->Parent= Control_->Parent;
    Color= kGrabHandleColorNormal;
    Pos();
}
__fastcall GrabHandle::~GrabHandle()
{
    //
}

void    GrabHandle::Pos()
{
    /*
    TPoint  angle_0,angle_45,angle_90,angle_135,angle_180,angle_225,angle_270,angle_315;
    int x0= Control_->Left - kGrabHandleSize / 2;
    int x1= Control_->Left + (Control_->Width - kGrabHandleSize) / 2;
    int x2= Control_->Left + Control_->Width - kGrabHandleSize / 2;

    int y0=Control_->Top - kGrabHandleSize / 2;;
    int y1=Control_->Top + (Control_->Height - kGrabHandleSize) / 2;;
    int y2=Control_->Top + Control_->Height - kGrabHandleSize / 2;
    
    angle_0.x= x1;
    angle_0.y= y0;

    angle_45.x= x2;
    angle_45.y= y0;

    angle_90.x= x2;
    angle_90.y= y1;

    angle_135.x= x2;
    angle_135.y= y2;

    angle_180.x= x1;
    angle_180.y= y2;
    
    angle_225.x= x0;
    angle_225.y= y2;
    
    angle_270.x= x0;
    angle_270.y= y1;

    angle_315.x= x0;
    angle_315.y= y0;
    */
    ///*
    
    int X[3];
    int Y[3];
    X[0] = Control_->Left - kGrabHandleSize / 2;
    X[1] = Control_->Left + (Control_->Width - kGrabHandleSize) / 2;
    X[2] = Control_->Left + Control_->Width - kGrabHandleSize / 2;
    Y[0] = Control_->Top - kGrabHandleSize / 2;
    Y[1] = Control_->Top + (Control_->Height - kGrabHandleSize) / 2;
    Y[2] = Control_->Top + Control_->Height - kGrabHandleSize / 2;
    //*/
    switch(Direct_)
    {
    case fdLeftUp: 
        Cursor = crSizeNWSE;
        //SetBounds(angle_315.x, angle_315.y, kGrabHandleSize, kGrabHandleSize);
        SetBounds(X[0], Y[0], kGrabHandleSize, kGrabHandleSize);
        break;
    case fdUp:  
        Cursor = crSizeNS;  
        //SetBounds(angle_0.x, angle_0.y, kGrabHandleSize, kGrabHandleSize);
        SetBounds(X[1], Y[0], kGrabHandleSize, kGrabHandleSize);
        break;
    case fdRightUp: 
        Cursor = crSizeNESW;
        //SetBounds(angle_45.x, angle_45.y, kGrabHandleSize, kGrabHandleSize);
        SetBounds(X[2], Y[0], kGrabHandleSize, kGrabHandleSize);
        break;
    case fdRight: 
        Cursor = crSizeWE;     
        //SetBounds(angle_90.x, angle_90.y, kGrabHandleSize, kGrabHandleSize);
        SetBounds(X[2], Y[1], kGrabHandleSize, kGrabHandleSize);
        break;
    case fdRightDown:  
        Cursor = crSizeNWSE;
        //SetBounds(angle_135.x, angle_135.y, kGrabHandleSize, kGrabHandleSize);
        SetBounds(X[2], Y[2], kGrabHandleSize, kGrabHandleSize);
        break;
    case fdDown: 
        Cursor = crSizeNS;
        //SetBounds(angle_180.x, angle_180.y, kGrabHandleSize, kGrabHandleSize);
        SetBounds(X[1], Y[2], kGrabHandleSize, kGrabHandleSize);
        break;
    case fdLeftDown: 
        Cursor = crSizeNESW;   
        //SetBounds(angle_225.x, angle_225.y, kGrabHandleSize, kGrabHandleSize);
        SetBounds(X[0], Y[2], kGrabHandleSize, kGrabHandleSize);
        break;
    case fdLeft:
        Cursor = crSizeWE;
        //SetBounds(angle_270.x, angle_270.y, kGrabHandleSize, kGrabHandleSize);
        SetBounds(X[0], Y[1], kGrabHandleSize, kGrabHandleSize);
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
    GetDesigner()->GetBeforDragPos() = ClientToScreen(Point(X, Y));
    MouseCapture = true;
  
}
void    __fastcall GrabHandle::MouseUp(TMouseButton Button, Classes::TShiftState Shift, int X, int Y)
{
    TCustomControl::MouseUp(Button,Shift, X, Y);

    GetDesigner()->Dragging=false;

}
void    __fastcall GrabHandle::MouseMove( Classes::TShiftState Shift, int X, int Y)
{
    int   ControlIndex=0;
    TPoint  cPos;
    int     cX,cY;

    TCustomControl::MouseMove(Shift, X, Y);

    if(!GetDesigner()->Dragging){
        return;
    }
    cPos =  ClientToScreen(Point(X, Y));

    for(;ControlIndex < GetDesigner()->ControlCount - 1;++ControlIndex){
        Hooker_->Controls[ControlIndex]->Left = Hooker_->Controls[ControlIndex]->Left + (X - Hooker_->GetBeforDragPos().x);
        Hooker_->Controls[ControlIndex]->Top = Hooker_->Controls[ControlIndex]->Top + (Y - Hooker_->GetBeforDragPos().y);
    }

    cX = Hooker_->GetBeforDragPos().x - cPos.x;
    cY = Hooker_->GetBeforDragPos().y - cPos.y;
    if(::abs(cX) < 2 && ::abs(cY) < 2){
        return;
    }

    switch(Direct_)
    {
    case fdLeftUp:
        if(Control_->Width + cX > 1){
          Control_->Left = Control_->Left - cX;
          Control_->Width = Control_->Width + cX;
          GetDesigner()->GetBeforDragPos().x = cPos.x;
        }
        if(Control_->Height + cY > 1 ){
            Control_->Top = Control_->Top - cY;
            Control_->Height = Control_->Height + cY;
            GetDesigner()->GetBeforDragPos().y = cPos.y;
        }

        break;
    case fdUp:
        if(Control_->Height + cY > 1){
            Control_->Top = Control_->Top - cY;
            Control_->Height = Control_->Height + cY;
            GetDesigner()->GetBeforDragPos().y = cPos.y;
        }
        break;
    case fdRightUp:
        if(Control_->Width - cX > 1){
            
          Control_->Width = Control_->Width - cX;
          GetDesigner()->GetBeforDragPos().x = cPos.x;
        } 
        if(Control_->Height + cY > 1){
            
          Control_->Top = Control_->Top - cY;
          Control_->Height = Control_->Height + cY;
          GetDesigner()->GetBeforDragPos().y = cPos.y;
        }
        
        break;
    case fdRight:
        if(Control_->Width - cX > 1){
            
          Control_->Width = Control_->Width - cX;
          GetDesigner()->GetBeforDragPos().x = cPos.x;
        }
        
        break;
    case fdRightDown:
        if(Control_->Width - cX > 1){
          Control_->Width = Control_->Width - cX;
          GetDesigner()->GetBeforDragPos().x = cPos.x;
        }  
        if(Control_->Height - cY > 1){
          Control_->Height = Control_->Height - cY;
          GetDesigner()->GetBeforDragPos().y = cPos.y;
        }
        
        break;
    case fdDown: 
        if(Control_->Height - cY > 1){
            
          Control_->Height = Control_->Height - cY;
          GetDesigner()->GetBeforDragPos().y = cPos.y;
        }
        
        break;
    case fdLeftDown: 
        if(Control_->Width + cX > 1 ){
          Control_->Left = Control_->Left - cX;
          Control_->Width = Control_->Width + cX;
          GetDesigner()->GetBeforDragPos().x = cPos.x;
        }
        if(Control_->Height - cY > 1){
          Control_->Height = Control_->Height - cY;
          GetDesigner()->GetBeforDragPos().y = cPos.y;
        }
        break;
    case fdLeft:
        if(Control_->Width + cX > 1){
          Control_->Left = Control_->Left - cX;
          Control_->Width = Control_->Width + cX;
          GetDesigner()->GetBeforDragPos().x = cPos.x;
        }
        break;
    }

}

}//namespace
