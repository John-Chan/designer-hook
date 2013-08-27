//---------------------------------------------------------------------------


#pragma hdrstop

#include "DesignerHook.h"
#include "GrabHandle.h"
#include "rdc_utility.h"
#include "TypeCast.hpp"
#include "Setting.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)


namespace rdc
{
ULONG __stdcall DesignerHook::AddRef(void)
{
    return ++this->FRefCount;
}
ULONG __stdcall DesignerHook::Release(void)
{
    return --this->FRefCount;
}
HRESULT __stdcall DesignerHook::QueryInterface(const GUID &IID, void **ppv)
{
    //
    return DISP_E_UNKNOWNINTERFACE;//S_OK; E_NoInterface;
}

/*
int __stdcall DesignerHook::Release(void)
{
    //
}
int __stdcall DesignerHook::AddRef(void)
{
    //
}

HResult __stdcall QueryInterface(const _GUID& guid,void**)
{
    //
}
*/

/************************************************************************
//  IDesignerNotify 的接口
//  --------------------------------------------------------------------
//
************************************************************************/

void __fastcall DesignerHook::Modified(void) 
{
    // nothing
}

void __fastcall DesignerHook::Notification(TPersistent* AnObject, TOperation Operation)
{
    int index=0;
    switch(Operation){
    case opRemove:
        index = Controls_->IndexOf(AnObject);
        if(index != -1){
            Remove(index);
        }
        if(AnObject == Form_){
            DownCast<CrackComponent*,TCustomForm*>(Form_)-> SetDesigning(false, true);
            Form_=NULL;
        }
        break;
    case opInsert:
        // nothing
        break;
    }
}

/************************************************************************
//  IDesignerHook 的接口
//  --------------------------------------------------------------------
//
************************************************************************/

TCustomForm* __fastcall DesignerHook::GetCustomForm(void) 
{
    return Form_;
}
void __fastcall DesignerHook::SetCustomForm(TCustomForm* Form)
{
    Form_ = Form;
    if(Form != NULL){
        Form->Designer = this;
    }
}
bool __fastcall DesignerHook::GetIsControl(void) 
{
    return  (ForceCast<CrackControl*,void*>(Form_))->IsControl ;//TCrackControl(FForm).IsControl
}
void __fastcall DesignerHook::SetIsControl(bool b)
{
    if( CheckPtrType<TControl*>(Form_)){
        (ForceCast<CrackControl*,void*>(Form_))->IsControl=b;
    }
}
bool __fastcall DesignerHook::IsDesignMsg(Controls::TControl* Sender, Messages::TMessage &Message) 
{
    if(CheckPtrType<GrabHandle*>(Sender) ){
        return false;
    }
    if( (Message.Msg>= WM_MOUSEFIRST && Message.Msg<= WM_MOUSELAST) || (Message.Msg>= WM_KEYFIRST && Message.Msg<= WM_KEYLAST) ){
        return OnMessage(Sender, Message);
    }
    return false ;
}
void __fastcall DesignerHook::PaintGrid(void)
{
    // nothing
}
void __fastcall DesignerHook::ValidateRename(Classes::TComponent* AComponent, const AnsiString CurName, const AnsiString) 
{
    // nothing
}
AnsiString __fastcall DesignerHook::UniqueName(const AnsiString BaseName) 
{
    return BaseName + "-"+CreateGuid();
}
Classes::TComponent* __fastcall DesignerHook::GetRoot(void) 
{
    return  Form_;
}


////////////////////////////////////////////////////////////////////////
/************************************************************************
//  GrabHandle
//  --------------------------------------------------------------------
//
************************************************************************/

__fastcall DesignerHook::DesignerHook()
:GrabHandleManager_(NULL),
Form_(NULL),
Root_(NULL),
Controls_(new TList()),
DraggingControl_(NULL),
Dragging_(false),
BeforDragPos_(),
Selecting_(false),
PointStart_(),
PointEnd_(),
OldRect_(),
NewRect_(),
MouseRect_()
{
    GrabHandleManager_=new GrabHandleManager(this);
}
__fastcall DesignerHook::~DesignerHook()
{
    if(Form_) ((CrackComponent*)Form_)->SetDesigning(false, true);

    delete GrabHandleManager_;
    delete Controls_;

}

/////////
//锁定鼠标到某一个范围
void    DesignerHook::MouseLock(TControl* Sender)
{
    TRect   Rect;   
    SetCaptureControl(Sender);
    GetClipCursor(&MouseRect_);
    if(NULL == Sender->Parent){
        return;
    }
    Rect=  Sender->Parent->ClientRect;
    //Rect = Sender->Parent->ClientToScreen(Rect);
    TPoint  PtTopLeft,PtBottomRight;
    PtTopLeft.x = Rect.Left;
    PtTopLeft.y = Rect.Top;
    PtTopLeft =  Sender->Parent->ClientToScreen(PtTopLeft);
    Rect.Top= PtTopLeft.y;
    Rect.Left= PtTopLeft.x;

    PtBottomRight.x = Rect.right;
    PtBottomRight.y = Rect.bottom;
    PtBottomRight =  Sender->Parent->ClientToScreen(PtBottomRight);
    Rect.bottom= PtBottomRight.y;
    Rect.right= PtBottomRight.x;
    
    //Rect.TopLeft = Sender->Parent->ClientToScreen(Rect.TopLeft);
    //Rect.BottomRight := Sender->Parent->ClientToScreen(Rect.BottomRight);
    ClipCursor(&Rect); //把鼠标锁定在固定区?
}
//释放对鼠标的锁定
void    DesignerHook::MouseFree()
{
    SetCaptureControl(NULL);
    ClipCursor(&MouseRect_);
}

bool    DesignerHook::OnMessage(TControl* Sender,Messages::TMessage &Message)
{
    //int  CtrlIndex=0;i=0;   
    // ( (Message.Msg >= WM_MOUSEFIRST) && (Message.Msg <= WM_MOUSELAST)) || ((Message.Msg >= WM_KEYFIRST) && (Message.Msg <= WM_KEYLAST));
    bool msg_handled=(IsMouseMsg(Message.Msg) || IsKeyMsg(Message.Msg));
    TWMMouse& WMMouse=(TWMMouse&)Message;
    TWMKey&   WMKey=(TWMKey&)Message;
    switch(Message.Msg){
    case WM_LBUTTONDOWN:
        MouseDown(
        Sender,
        mbLeft,
        KeysToShiftState(WMMouse.Keys),
        WMMouse.XPos,
        WMMouse.YPos
        );
        break;
    case WM_MOUSEMOVE:
        MouseMove(
          Sender,
          KeysToShiftState(WMMouse.Keys),
          WMMouse.XPos,
          WMMouse.YPos);
        break;
    case WM_LBUTTONUP:
        MouseUp(
          Sender,
          mbLeft,
          KeysToShiftState(WMMouse.Keys),
          WMMouse.XPos,
          WMMouse.YPos);
        break;
    case WM_KEYDOWN:
        KeyDown(
          Sender,
          WMKey.CharCode,
          KeyDataToShiftState(WMKey.KeyData)
          );
        break;
    }
 
    if(Sender == Form_){
        msg_handled=false ;
    }
    return msg_handled;
}

void    DesignerHook::Remove(TControl* ctl)
{
    Remove(Controls_->IndexOf(ctl));
}
void    DesignerHook::Remove(int Index)
{
    if(Index == -1){
        return;
    }
    TControl* ctl= ForceCast<TControl*,void*>(Controls_->Items[Index]);//(TControl*)(Controls_[Index]);
    Controls_->Delete(Index);
    ClearGrabHandle(ctl);
}

void    DesignerHook::Clear()
{
    for(int i=0;i<Controls_->Count - 1;++i){
        Remove(i);
    }
}
TControl*   DesignerHook::Add(TControl* Ctrol)
{

    //GrabHandleDirect    GrabDirect;
    GrabHandle*         GrabFrame;
    Controls_->Add(Ctrol);

    GrabFrame= new  GrabHandle(GrabHandleManager_,Ctrol,fdLeftUp);
    GrabFrame= new  GrabHandle(GrabHandleManager_,Ctrol,fdUp);
    GrabFrame= new  GrabHandle(GrabHandleManager_,Ctrol,fdRightUp);
    GrabFrame= new  GrabHandle(GrabHandleManager_,Ctrol,fdRight);
    GrabFrame= new  GrabHandle(GrabHandleManager_,Ctrol,fdRightDown);
    GrabFrame= new  GrabHandle(GrabHandleManager_,Ctrol,fdDown);
    GrabFrame= new  GrabHandle(GrabHandleManager_,Ctrol,fdLeftDown);
    GrabFrame= new  GrabHandle(GrabHandleManager_,Ctrol,fdLeft);
    (void)GrabFrame;
    return Ctrol;
}

void        DesignerHook::AddRectControls(TWinControl* Parent,TRect Rect)
{
    Clear();
    for(int i=0; i<Parent->ControlCount - 1 ;++i ){
        if( InRect(Rect, Parent->Controls[i]->BoundsRect) && OwnerCheck(Parent->Controls[i], Root_)){
            Add(Parent->Controls[i]);
        }
    }

}
void    DesignerHook::ShowGrabHandle(const bool b)
{
    int ComponentIndex=0;
    for(;ComponentIndex < GrabHandleManager_->ComponentCount - 1;++ComponentIndex){
        if( CheckPtrType<GrabHandle*>(GrabHandleManager_->Components[ComponentIndex])){
            GrabHandle* GrabHandlePtr=DownCast<GrabHandle*,TComponent*>(GrabHandleManager_->Components[ComponentIndex]);
            if(ControlCount > 1){
                GrabHandlePtr->Color=kGrabHandleColorMutiSelect;//kGrabHandleColorActive;
            }else{
                GrabHandlePtr->Color=kGrabHandleColorNormal;
            }
            GrabHandlePtr->Pos();
            bool ctl_count_showable= (ControlCount >= 1 ) ;
            bool direct_showable= (GrabHandlePtr->Direct_ == fdLeftUp|| GrabHandlePtr->Direct_ ==fdLeftDown ||GrabHandlePtr->Direct_ == fdRightUp ||GrabHandlePtr->Direct_ == fdRightDown );
            //GrabHandlePtr->Color= Selecting_?kGrabHandleColorActive:kGrabHandleColorNormal;
            GrabHandlePtr->Visible=b && ctl_count_showable && direct_showable ;
        }
    }
}
void    DesignerHook::ClearGrabHandle(TControl* Ctrol)
{
    for(int i=GrabHandleManager_->ComponentCount - 1;i>=0;--i){
        GrabHandle* p=DownCast<GrabHandle*,TComponent*>(GrabHandleManager_->Components[i]);// dynamic_cast<GrabHandle>(GrabHandleManager_->Components[i]);
        if( p && p->Control_ == Ctrol)
            delete  p;
    }
}
void    DesignerHook::SetDragging(const bool b)
{
    Dragging_= b;
    ShowGrabHandle(!b);
}

int         DesignerHook::GetControlCount()
{
    return Controls_->Count;
}
TControl*   DesignerHook::GetControls(int Index)
{
    return ForceCast<TControl*,void*>(Controls_->Items[Index]);//(TControl*)(Controls_[Index]);
}

bool        DesignerHook::OwnerCheck(TControl* Sender, TComponent* CheckOnwer)
{
    TComponent* ptr=Sender->Owner;
    while(ptr != NULL){
        if(CheckOnwer == ptr){
            return true ;
        }
        ptr=ptr->Owner;
    }
    return false;
}

void __fastcall DesignerHook::MouseDown(TControl* Sender,TMouseButton Button, Classes::TShiftState Shift, int X, int Y)
{
    int  CtrlIndex=0;
    if(Dragging_) return ;
    CtrlIndex = Controls_->IndexOf(Sender);

    if(Shift.Contains(ssShift)){
        //按Shift多选
        if (Sender == Root_ || Sender == Form_ ) {
            //多选不能添加Root或者Form
            return;
        }
        if(CtrlIndex == -1){
            Add(Sender);
            Dragging_=false;
        }else{
        Remove(Sender);
        ShowGrabHandle(true);
      }
    }else {
        // if (ssCtrl in Shift) or (Sender = FRoot) then
        if( Shift.Contains(ssCtrl) || Sender == Root_){
            //按住Shift或者点击的是Root就框选
            Clear();
            if(CheckPtrType<TWinControl*>(Sender)){
                TWinControl* win_ctrl=DownCast<TWinControl*,TControl*>(Sender);
                if(win_ctrl->ControlCount == 0){
                    if(CtrlIndex == -1){
                        Add(Sender);
                        ShowGrabHandle(true);
                    }
                }else{
                    PointStart_= Sender->ClientToScreen(Point(X, Y));
                    OldRect_= Rect(X, Y, X + 1, Y + 1);
                    Selecting_ = true;
                    SetCaptureControl(Sender);
                }
                return ;
            }
        }else{
            //没按Shift也没按Ctrl点击.那就添加自己到选择的控件组中 .Root和Form不能和别的控件同时在组中
            if( Sender == Root_ || Sender == Form_ ) {
                return;
            }
            if(CtrlIndex == -1){
                Clear();
                Add(Sender);
            }

            Dragging_ = true;
            DraggingControl_ = Sender;
            MouseLock(Sender);
            BeforDragPos_ = Sender->ClientToScreen(Point(X, Y));
        }
    }
}
void __fastcall DesignerHook::MouseUp(TControl* Sender,TMouseButton Button, Classes::TShiftState Shift, int X, int Y)
{
    HDC dc;
    if(Dragging_){
        MouseFree();
        Dragging_ = false;
    }
    if(Selecting_){
        dc= GetDC(0);
        DrawFocusRect(dc, &OldRect_);
        ReleaseDC(0, dc);
        Selecting_= false;
        SetCaptureControl(NULL);
        if( CheckPtrType<TWinControl*>(Sender) ){
            TPoint  PtTopLeft,PtBottomRight;
            PtTopLeft.x = OldRect_.Left;
            PtTopLeft.y = OldRect_.Top;
            PtTopLeft =  Sender->ScreenToClient(PtTopLeft);
            OldRect_.Top= PtTopLeft.y;
            OldRect_.Left= PtTopLeft.x;

            PtBottomRight.x = OldRect_.right;
            PtBottomRight.y = OldRect_.bottom;
            PtBottomRight =  Sender->ScreenToClient(PtBottomRight);
            OldRect_.bottom= PtBottomRight.y;
            OldRect_.right= PtBottomRight.x;
    
            //OldRect_.TopLeft = Sender->ScreenToClient(OldRect_.TopLeft);
            //OldRect_.BottomRight = Sender->ScreenToClient(OldRect_.BottomRight);

            OldRect_ = PointToRect(PtTopLeft, PtBottomRight);
            AddRectControls(DownCast<TWinControl*,TControl*>(Sender), OldRect_);
            ShowGrabHandle(true);
        }
    }

}
void __fastcall DesignerHook::MouseMove(TControl* Sender,Classes::TShiftState Shift, int X, int Y)
{
    int i=0;
    TPoint pos;
    HDC    dc;
    if(Dragging_){
        pos= Mouse->CursorPos;
        for(i=Controls_->Count - 1;i>=0;--i){
            if(Controls[i]->Parent == Sender->Parent){
                //如果都是同一个Paren的话  
                Controls[i]->Left = Controls[i]->Left - (BeforDragPos_.x - pos.x);
                Controls[i]->Top = Controls[i]->Top - (BeforDragPos_.y - pos.y);
            }else{
                Remove(i);
            }
            BeforDragPos_ = pos;
        }
        ShowGrabHandle(true);
    }else{
        if(Selecting_){
            PointEnd_ = Sender->ClientToScreen(Point(X, Y));
            NewRect_ = PointToRect(PointStart_, PointEnd_);  
            dc = GetDC(0);
            DrawFocusRect(dc, &OldRect_);
            DrawFocusRect(dc, &NewRect_);
            ReleaseDC(0, dc);
            OldRect_ = NewRect_;
        }
    }
}

void __fastcall DesignerHook::KeyDown(TControl* Sender,Word &Key,Classes::TShiftState Shift)
{
    int i=0;
    if(0== ControlCount ||(!Shift.Contains(ssShift) && !Shift.Contains(ssCtrl)) ||(VK_CONTROL==Key || VK_SHIFT == Key) ) {
            return;
    }
    if(GetControlCount() == 0 && Controls[0] == Form_){
        return;
    }
    if(Shift.Contains(ssCtrl)){
        switch(Key){
        case VK_UP:
            ShowGrabHandle(false);
            __try{
                for(int index=0;i<GetControlCount() -1 ;++i){
                    Controls[index]->Top= Controls[index]->Top -1;
                }
            }__finally{
                ShowGrabHandle(true);
            }
            break;
        case VK_DOWN:
            ShowGrabHandle(false);
            __try{
                for(int index=0;i<GetControlCount() -1 ;++i){
                    Controls[index]->Top= Controls[index]->Top +1;
                }
            }__finally{
                ShowGrabHandle(true);
            }
            break;
        case VK_LEFT:
            ShowGrabHandle(false);
            __try{
                for(int index=0;i<GetControlCount() -1 ;++i){
                    Controls[index]->Left= Controls[index]->Left -1;
                }
            }__finally{
                ShowGrabHandle(true);
            }
            break;
        case VK_RIGHT:
            ShowGrabHandle(false);
            __try{
                for(int index=0;i<GetControlCount() -1 ;++i){
                    Controls[index]->Left= Controls[index]->Left +1;
                }
            }__finally{
                ShowGrabHandle(true);
            }
            break;
        }
    }else{
        switch(Key){
        case VK_UP:
            ShowGrabHandle(false);
            __try{
                for(int index=0;i<GetControlCount() -1 ;++i){
                    if(Controls[index]->Height - 1 > 1)
                        Controls[index]->Height= Controls[index]->Height -1;
                }
            }__finally{
                ShowGrabHandle(true);
            }
            break;  
        case VK_DOWN:
            ShowGrabHandle(false);
            __try{
                for(int index=0;i<GetControlCount() -1 ;++i){
                    if(Controls[index]->Height + 1 > 1)
                        Controls[index]->Height= Controls[index]->Height + 1;
                }
            }__finally{
                ShowGrabHandle(true);
            }
            break;
        case VK_LEFT:
            ShowGrabHandle(false);
            __try{
                for(int index=0;i<GetControlCount() -1 ;++i){
                    if(Controls[index]->Width - 1 > 1)
                        Controls[index]->Width= Controls[index]->Width -1;
                }
            }__finally{
                ShowGrabHandle(true);
            }
            break;
        case VK_RIGHT:
            ShowGrabHandle(false);
            __try{
                for(int index=0;i<GetControlCount() -1 ;++i){
                    if(Controls[index]->Width + 1 > 1)
                        Controls[index]->Width= Controls[index]->Width +1;
                }
            }__finally{
                ShowGrabHandle(true);
            }
            break;
        }
    }
}


DesignerHook*  DesignerHook::BeginDesign(TCustomForm* Form)
{
    return BeginDesign(Form,Form);
}
DesignerHook*  DesignerHook::BeginDesign(TCustomForm* Form, TWinControl* Root)
{
    DesignerHook* p_designer=new DesignerHook();
    p_designer->Form_= Form;
    p_designer->Root_= Root;
    _di_IDesignerHook di_hook((IDesignerHook*)p_designer);

    Form->Designer=di_hook;
    (ForceCast<CrackComponent*,void*>(Form))->CallSetDesigning(true, false);
    (ForceCast<CrackComponent*,void*>(Root))->CallSetDesigning(true, true);
    return p_designer;
}

}//namespace
