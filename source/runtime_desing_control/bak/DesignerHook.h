//---------------------------------------------------------------------------

#ifndef DesignerHookH
#define DesignerHookH
//---------------------------------------------------------------------------
#include <system.hpp>  // TInterfacedObject
#include <Classes.hpp> // IDesignerNotify
#include <Forms.hpp>   // IDesignerHook

namespace rdc
{
    class DesignerHook : public TInterfacedObject, /*IDesignerNotify,*/ IDesignerHook
    {
        /*
    public
    { IDesignerNotify 的接口}
    procedure Modified;
    procedure Notification(AnObject: TPersistent; Operation: TOperation);
      */
public: // IDesignerNotify 的接口
	virtual void __fastcall Modified(void) = 0 ;
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
        virtual void __fastcall SetCustomForm(TCustomForm* Value) ;
        virtual bool __fastcall GetIsControl(void) ;
        virtual void __fastcall SetIsControl(bool Value) ;
        virtual bool __fastcall IsDesignMsg(Controls::TControl* Sender, Messages::TMessage &Message) ;
        virtual void __fastcall PaintGrid(void);
        virtual void __fastcall ValidateRename(Classes::TComponent* AComponent, const AnsiString CurName, const AnsiString) ;        virtual AnsiString __fastcall UniqueName(const AnsiString BaseName) = 0 ;
        virtual Classes::TComponent* __fastcall GetRoot(void) = 0 ;
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
        __property bool Dragging  = {read=FDragging, write=SetDragging};
        __property int ControlCount = {read=GetControlCount, nodefault};
        __property TControl* Controls[int Index] = {read=GetControls};
    private:
        /*
    procedure SetDragging(const Value: Boolean);
    function GetControlCount: Integer;
    function GetControls(Index: Integer): TControl;
    */
        void    SetDragging(const bool b);
        int     GetControlCount();
        TControl*   GetControls(int Index);
    public: //TODO: private
        GrabHandleManager*  FGrabHandleManager;
        TCustomForm*        FForm;
        TWinControl*        FRoot;
        TList*              FControls;
        TControl*           FDraggingControl; 
        bool   FDragging;
        TPoint FBeforDragPos;
        bool   FSelecting;
        TPoint FPointStart;  
        TPoint FPointEnd;
        TRect  FOldRect;
        TRect  FNewRect;

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
    */
    };
}

#endif
