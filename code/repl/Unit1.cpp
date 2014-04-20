#include <vcl.h>
#pragma hdrstop
#include "interpreter/interpreter.cpp"
#include "Unit1.h"
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
enum {loop,input};
unsigned long long int bytecodeptr=0;
int state= loop;
void out(str* s){
    Form1->Memo1->Lines->Add(*s);}
str* inp(){
    str* s= new str;
    state=input;
    Form1->Edit1->Text="";
    Form1->StatusBar1->SimpleText="Need user input";
    while(state!=loop)
        Application->ProcessMessages();
    *s=Form1->Edit1->Text;
    Form1->Edit1->Text="";
    Form1->StatusBar1->SimpleText="";
    return s;}
__fastcall TForm1::TForm1(TComponent* Owner)
        : TForm(Owner){}
void __fastcall TForm1::SpeedButton1Click(TObject *Sender){
    ShowMessage(*inp());}
void __fastcall TForm1::Edit1KeyPress(TObject *Sender,
char &Key){
    if(Key==13){
        Memo1->Lines->Add(">>> "+Edit1->Text);
    switch(state){
        case loop:
            try{
                str last="",now="";
                now =preprocess(Edit1->Text);
                while(now!=last){
                    last=now;
                    now= preprocess(last);}
                compile(parseCode(last));}
            catch(str msg){
                Memo1->Lines->Add(msg);
                while(bytecode.size()!=bytecodeptr)
                    bytecode.pop_back();}
            execute( bytecodeptr);
            bytecodeptr=bytecode.size();
            Edit1->Text="";
        break;
        case input:
            state=loop;
        break;}
    ListBox1->Clear();
    ListBox2->Clear();
    ListBox3->Clear();
    for(unsigned long long int i=0;i<stack.size();i++){
        ListBox1->Items->Insert(0,FloatToStr(stack[i]));}
    for(unsigned long long int i=0;i<hand1.size();i++){
        ListBox2->Items->Insert(0,FloatToStr(hand1[i]));}
    for(unsigned long long int i=0;i<hand2.size();i++){
        ListBox3->Items->Insert(0,FloatToStr(hand2[i]));}}}

