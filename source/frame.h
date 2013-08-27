//---------------------------------------------------------------------------


#ifndef frameH
#define frameH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFrame1 : public TFrame
{
__published:	// IDE-managed Components
    TPanel *pnl1;
    TEdit *edt1;
    TButton *btn1;
    TButton *btn2;
    TMemo *mmo1;
    TLabeledEdit *lbledt1;
    TShape *shp1;
    TRadioGroup *rg1;
    TButton *btn3;
    TButton *btn4;
    TButton *btn5;
private:	// User declarations
public:		// User declarations
    __fastcall TFrame1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFrame1 *Frame1;
//---------------------------------------------------------------------------
#endif
