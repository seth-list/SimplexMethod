//---------------------------------------------------------------------------

#ifndef UnitH
#define UnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include "CSPIN.h"
#include <ExtCtrls.hpp>
#include "Word_2K_SRVR.h"
#include <Dialogs.hpp>
#include <OleServer.hpp>
#include <Grids.hpp>
#include "fraction.h"
#include "simplex.h"
#include <Menus.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
TEdit *Mass_edit[11][11];
TLabel *Mass_label[11][10];
TComboBox *Mass_cb[11];
TCustomMemo* Memo;
TCheckBox *Mass_ch[10];
//буфер для записи пр задачи
AnsiString decs1="";
//буфер для записи дв задачи
AnsiString decs2="";

int m1=0,n1=0;
fraction get_fr(AnsiString text);
AnsiString add_table(simplex s, int m, int n);
AnsiString add_title(simplex s);
AnsiString add_answer(simplex s);

class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TPageControl *PageControl1;
        TTabSheet *TabSheet1;
        TGroupBox *GroupBox2;
        TCSpinEdit *CSpinEdit1;
        TCSpinEdit *CSpinEdit2;
        TLabel *Label1;
        TLabel *Label2;
        TButton *Button1;
        TGroupBox *GroupBox1;
        TLabel *Label3;
        TLabel *Label4;
        TButton *Button2;
        TPanel *Panel1;
        TTabSheet *TabSheet2;
        TSaveDialog *SaveDialog1;
        TStringGrid *StringGrid1;
        TStringGrid *StringGrid2;
        TPanel *Panel3;
        TPanel *Panel4;
        TButton *Button4;
        TButton *Button5;
        TLabel *Label5;
        TLabel *Label6;
        TCheckBox *CheckBox1;
        TSaveDialog *SaveDialog2;
        TMainMenu *MainMenu1;
        TMenuItem *MFile;
        TMenuItem *MSave;
        TMenuItem *N3;
        TMenuItem *Exit;
        TMenuItem *MHelp;
        TMenuItem *N5;
        TMenuItem *N6;
        TMenuItem *N7;
        TMenuItem *N1;
        TMenuItem *N2;
        TMenuItem *N4;
        TMenuItem *N8;
        TMenuItem *N9;
        TMenuItem *N10;
        TMenuItem *N11;
        TCheckBox *CheckBox2;
        TButton *Button3;
        TButton *Button6;
        TButton *Button7;
        TSaveDialog *SaveDialog3;
        TOpenDialog *OpenDialog1;
        TSpeedButton *SpeedButton1;
        TSpeedButton *SpeedButton2;
        TSpeedButton *SpeedButton3;
        TSpeedButton *SpeedButton4;
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall Button2Click(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall Button4Click(TObject *Sender);
        void __fastcall Button5Click(TObject *Sender);
        void __fastcall ExitClick(TObject *Sender);
        void __fastcall N8Click(TObject *Sender);
        void __fastcall N9Click(TObject *Sender);
        void __fastcall N7Click(TObject *Sender);
        void __fastcall N5Click(TObject *Sender);
        void __fastcall N10Click(TObject *Sender);
        void __fastcall N11Click(TObject *Sender);
        void __fastcall Button3Click(TObject *Sender);
        void __fastcall Button6Click(TObject *Sender);
        void __fastcall Button7Click(TObject *Sender);

private:	// User declarations
public:		// User declarations
        __fastcall TForm1(TComponent* Owner);
       
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
