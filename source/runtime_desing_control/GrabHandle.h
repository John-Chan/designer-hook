//---------------------------------------------------------------------------

#ifndef GrabHandleH
#define GrabHandleH

#include <Classes.hpp>   //   TShiftState
#include <Controls.hpp>  //   TMouseButton


//---------------------------------------------------------------------------
namespace rdc
{
    // 前置声明
    class DesignerHook;

    
    enum GrabHandleDirect{
        fdLeftUp,
        fdUp,
        fdRightUp,
        fdRight,
        fdRightDown,
        fdDown,
        fdLeftDown,
        fdLeft
    };
    
    /************************************************************************
    //  GrabHandleManager
    //  --------------------------------------------------------------------
    //
    ************************************************************************/
    //管理FrameSize的.免得要遍历释放每个FrameSize
    class  GrabHandleManager : public  TComponent
    {
    public:
    
	    //__fastcall virtual TComponent(TComponent* AOwner);
	    //__fastcall virtual ~TComponent(void);
    
        __fastcall virtual GrabHandleManager(DesignerHook* Hooker);
        __fastcall virtual ~GrabHandleManager();
        DesignerHook*  GetDesigner();
    private:
        DesignerHook*  Hooker_;
    };

    /************************************************************************
    //  GrabHandle
    //  --------------------------------------------------------------------
    //
    ************************************************************************/

    //选中时控件边上的八个小黑点
    class GrabHandle : public  TCustomControl
    {
        friend class DesignerHook;
    private:
        GrabHandleManager* GrabManager_;
        TControl*  Control_;
        GrabHandleDirect    Direct_;
        DesignerHook*       Hooker_;
    private:
        void    Pos();
        DesignerHook*  GetDesigner();
    protected:
        //DYNAMIC void __fastcall MouseDown(TMouseButton Button, Classes::TShiftState Shift, int X, int Y);
        DYNAMIC void __fastcall MouseDown(TMouseButton Button, Classes::TShiftState Shift, int X, int Y);
        DYNAMIC void __fastcall MouseUp(TMouseButton Button, Classes::TShiftState Shift, int X, int Y);
        DYNAMIC void __fastcall MouseMove(Classes::TShiftState Shift, int X, int Y);

        //virtual void    MouseDown(TMouseButton* Button,TShiftState Shift,int X,int Y);
        //virtual void    MouseUp(TMouseButton* Button,TShiftState Shift,int X,int Y);
        //virtual void    MouseMove(TShiftState Shift,int X,int Y);
        //procedure MouseDown(Button: TMouseButton; Shift: TShiftState; X: Integer;Y: Integer); override;
        //procedure MouseUp(Button: TMouseButton; Shift: TShiftState; X: Integer;Y: Integer); override;
        //procedure MouseMove(Shift: TShiftState; X: Integer; Y: Integer); override;
    public:
        __fastcall virtual GrabHandle(/*TComponent* GrabManager*/GrabHandleManager* GrabManager, TControl* Control,GrabHandleDirect Direct);
        __fastcall virtual  ~GrabHandle();
        __property DesignerHook* Designer = {read=GetDesigner, nodefault};
    //constructor Create(AManager: TComponent; AControl: TControl; ADirect: TGrabHandleDirect);
    //destructor Destroy; override;

    //property Designer: TDesignerHook read GetDesigner;
    };
}//namespace
#endif
