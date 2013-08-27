//---------------------------------------------------------------------------

#ifndef DesignerHookH
#define DesignerHookH
//---------------------------------------------------------------------------
#include <system.hpp>  // TInterfacedObject
#include <Classes.hpp> // IDesignerNotify
#include <Forms.hpp>   // IDesignerHook
#include <Messages.hpp>

#include "GrabHandle.h"

namespace rdc
{
    class DesignerHook : public TInterfacedObject, /*IDesignerNotify,  */ IDesignerHook
    {
    public: // 辅助函数
        static DesignerHook*  BeginDesign(TCustomForm* Form);
        static DesignerHook*  BeginDesign(TCustomForm* Form, TWinControl* Root); 
        //static void           EndDesign(TCustomForm* Form);
        static void           EndDesign(DesignerHook* hooker);
    public: // IUnknown的接口
        virtual ULONG __stdcall AddRef(void);
        virtual ULONG __stdcall Release(void);
        virtual HRESULT __stdcall QueryInterface(const GUID &IID, void **ppv);

    public: // IDesignerNotify 的接口
        virtual void __fastcall Modified(void) ;
        virtual void __fastcall Notification(TPersistent* AnObject, TOperation Operation) ;

    public: //IDesignerHook 的接口
        virtual TCustomForm* __fastcall GetCustomForm(void) ;
        virtual void __fastcall SetCustomForm(TCustomForm* Form) ;
        virtual bool __fastcall GetIsControl(void) ;
        virtual void __fastcall SetIsControl(bool b) ;
        virtual bool __fastcall IsDesignMsg(Controls::TControl* Sender, Messages::TMessage &Message) ;
        virtual void __fastcall PaintGrid(void);
        virtual void __fastcall ValidateRename(Classes::TComponent* AComponent, const AnsiString CurName, const AnsiString) ;        virtual AnsiString __fastcall UniqueName(const AnsiString BaseName) ;
        virtual Classes::TComponent* __fastcall GetRoot(void) ;
        __property bool IsControl = {read=GetIsControl, write=SetIsControl};
        __property TCustomForm* Form = {read=GetCustomForm, write=SetCustomForm};

    public:
       virtual __fastcall DesignerHook();
       virtual __fastcall ~DesignerHook();
        __property bool Dragging  = {read=Dragging_, write=SetDragging};
        __property int ControlCount = {read=GetControlCount, nodefault};
        __property TControl* Controls[int Index] = {read=GetControls};
        //鼠标移动时,是不是显示边框
        void        ShowGrabWhenMove(bool b);  
        void        Clear();
    private:
        //锁定鼠标到某一个范围
        void    MouseLock(TControl* Sender);
        //释放对鼠标的锁定
        void    MouseFree();

        bool    OnMessage(TControl* Sender,Messages::TMessage &Message);

        void    Remove(TControl* ctl);
        void    Remove(int Index);

        TControl*   Add(TControl* Ctrol);
        void    ShowGrabHandle(const bool b);
        void    ClearGrabHandle(TControl* Ctrol);
        void    SetDragging(const bool b);

        int         GetControlCount();
        TControl*   GetControls(int Index);
        void        AddRectControls(TWinControl* Parent,TRect Rect);
        bool        OwnerCheck(TControl* Sender, TComponent* CheckOnwer);
        bool        DirectShowable(GrabHandleDirect drect);

        DYNAMIC void __fastcall MouseDown(TControl* Sender,TMouseButton Button, Classes::TShiftState Shift, int X, int Y);
        DYNAMIC void __fastcall MouseUp(TControl* Sender,TMouseButton Button, Classes::TShiftState Shift, int X, int Y);
        DYNAMIC void __fastcall MouseMove(TControl* Sender,Classes::TShiftState Shift, int X, int Y);
        
        DYNAMIC void __fastcall KeyDown(TControl* Sender,Word &Key,Classes::TShiftState Shift);


    public: //TODO: private
        GrabHandleManager*  GrabHandleManager_;
        TCustomForm*        Form_;
        TWinControl*        Root_;
        TList*              Controls_;
        TControl*           DraggingControl_;
        bool   Dragging_;
        TPoint BeforDragPos_;
        bool   Selecting_;
        TPoint PointStart_;
        TPoint PointEnd_;
        TRect  OldRect_;
        TRect  NewRect_;
        TRect  MouseRect_;//鼠标被限制的范围
        bool   ShowGrabWhenMove_;

    };
}

#endif
