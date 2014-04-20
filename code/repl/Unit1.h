//---------------------------------------------------------------------------

#ifndef Unit1H
#define Unit1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TForm1 : public TForm
{
__published:	// IDE-managed Components
        TMemo *Memo1;
        TEdit *Edit1;
        TListBox *ListBox1;
        TLabel *Label1;
        TListBox *ListBox2;
        TLabel *Label2;
        TListBox *ListBox3;
        TLabel *Label3;
        TStatusBar *StatusBar1;
        void __fastcall SpeedButton1Click(TObject *Sender);
        void __fastcall Edit1KeyPress(TObject *Sender, char &Key);
private:	// User declarations
public:		// User declarations
        __fastcall TForm1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TForm1 *Form1;
//---------------------------------------------------------------------------
#endif
