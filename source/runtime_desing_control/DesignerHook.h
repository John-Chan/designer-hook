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
    
    //class DesignerHook : public TInterfacedObject, /*IDesignerNotify, IDesignerHook  */  _di_IDesignerHook
    class DesignerHook : public TInterfacedObject, /*IDesignerNotify,  */ IDesignerHook  
    {
    public:
    
        static DesignerHook*  BeginDesign(TCustomForm* Form);
        static DesignerHook*  BeginDesign(TCustomForm* Form, TWinControl* Root);
    public:
        /*
        virtual HResult __stdcall QueryInterface(const _GUID& guid,void**);
        virtual int __stdcall AddRef(void);
        virtual int __stdcall Release(void);
        */
        virtual ULONG __stdcall AddRef(void);
        virtual ULONG __stdcall Release(void);
        virtual HRESULT __stdcall QueryInterface(const GUID &IID, void **ppv);
        /*
    public
    { IDesignerNotify 的接口}
    procedure Modified;
    procedure Notification(AnObject: TPersistent; Operation: TOperation);
      */
    public: // IDesignerNotify 的接口
        virtual void __fastcall Modified(void) ;
        virtual void __fastcall Notification(TPersistent* AnObject, TOperation Operation) ;
    /*
  public
    { IDesignerHook 的接口}
    function GetCustomForm: TCustomForm;
    procedure SetCustomForm(Value: TCustomForm);
    function GetIsControl: Boolean;
    procedure SetIsControl(Value: Boolean);
    function IsDesignMsg(Sender: TControl; var Message: TMessage): Boolean;
    procedure PaintGrid;
    procedure PaintMenu;
    procedure ValidateRename(AComponent: TComponent;
      const CurName, NewName: string);
    function UniqueName(const BaseName: string): string;
    function GetRoot: TComponent;

    property IsControl: Boolean read GetIsControl write SetIsControl;
    property Form: TCustomForm read GetCustomForm write SetCustomForm;
    */
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
        /*    
        constructor Create();
        destructor Destroy; override;
        __property Dragging: Boolean read FDragging write SetDragging;
        __property ControlCount: Integer read GetControlCount;
        __property Controls[int Index]: TControl read GetControls;
       */
       virtual __fastcall DesignerHook();
       virtual __fastcall ~DesignerHook();
        __property bool Dragging  = {read=Dragging_, write=SetDragging};
        __property int ControlCount = {read=GetControlCount, nodefault};
        __property TControl* Controls[int Index] = {read=GetControls};
    private:
        /*
        procedure MouseLock(Sender: TControl);//锁定鼠标到某一个范围
        procedure MouseFree();//释放对鼠标的锁定

        function OnMessage(Sender: TControl; var Message: TMessage): Boolean;
        procedure Remove(AControl: TControl); overload;
        procedure Remove(Index: Integer); overload;
        procedure Clear();
        function Add(AControl: TControl): TControl;
        procedure ShowGrabHandle(const Show: boolean);
        procedure ClearGrabHandle(AControl: TControl);
        procedure SetDragging(const Value: Boolean);
        function GetControlCount: Integer;
        function GetControls(Index: Integer): TControl;
        procedure AddRectControls(Parent: TWinControl; Rect: TRect);
        function OwnerCheck(Sender: TControl; CheckOnwer: TComponent): Boolean;

        procedure MouseDown(Sender: TControl; Button: TMouseButton; Shift: TShiftState; X: Integer;
          Y: Integer); virtual;
        procedure MouseUp(Sender: TControl; Button: TMouseButton; Shift: TShiftState; X: Integer;
          Y: Integer); virtual;
        procedure MouseMove(Sender: TControl; Shift: TShiftState; X: Integer; Y: Integer); virtual;
        procedure KeyDown(Sender: TControl; var Key: Word; Shift: TShiftState); virtual;
 
        */
        //锁定鼠标到某一个范围
        void    MouseLock(TControl* Sender);
        //释放对鼠标的锁定
        void    MouseFree();

        bool    OnMessage(TControl* Sender,Messages::TMessage &Message);

        void    Remove(TControl* ctl);
        void    Remove(int Index);

        void    Clear();
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


        /*
        FGrabHandleManager: TGrabHandleManager;
        FForm: TCustomForm;
        FRoot: TWinControl;
        FControls: TList;
        FDraggingControl: TControl;
        FDragging: Boolean;
        FBeforDragPos: TPoint;

        FSelecting: Boolean;
        FPointStart, FPointEnd: TPoint;
        FOldRect: TRect;
        FNewRect: TRect;   
        FMouseRect: TRect;//鼠标被限制的范围
        */
    };

    /*
    //开始设计.设计整个窗口里,以窗口为Owner的控件
    function BeginDesign(Form: TCustomForm): IDesignerHook; overload;
    //开始设计.设计窗口里,Root控件为Owner的控件
    function BeginDesign(Form: TCustomForm; Root: TWinControl): IDesignerHook; overload;
    //终止设计
    procedure EndDesign(Form : TCustomForm);
    */
}

#endif
