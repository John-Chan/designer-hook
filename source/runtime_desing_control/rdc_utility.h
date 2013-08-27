//---------------------------------------------------------------------------

#ifndef rdc_utilityH
#define rdc_utilityH

#include "DesignerHook.h" //DesignerHook

#include <Types.hpp>
#include <Classes.hpp>
#include <Controls.hpp>

//---------------------------------------------------------------------------
namespace rdc
{

    // 点到区的转化
    TRect   PointToRect(const TPoint& pt1,const TPoint& pt2);

    bool    InRect(TRect R1,TRect R2);

    AnsiString CreateGuid(void);
    bool        IsMouseMsg(unsigned Msg);
    bool        IsKeyMsg(unsigned Msg);
    /************************************************************************
    //  CrackComponent
    //  --------------------------------------------------------------------
    //
    ************************************************************************/
    class   CrackComponent : public TComponent
    {
    public:
        friend class DesignerHook;
        // 非派生类通过CallSetDesigning 来调用 SetDesigning
        void   CallSetDesigning(bool Value, bool SetChildren);
    private:
    };
          
    /************************************************************************
    //  CrackControl
    //  --------------------------------------------------------------------
    //
    ************************************************************************/
    class   CrackControl : public TControl
    {
    public:
        friend class DesignerHook; 
        // 非派生类通过CallSetDesigning 来调用 SetDesigning
        void   CallSetDesigning(bool Value, bool SetChildren);
    private:
    };
    

}
#endif
