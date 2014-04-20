//---------------------------------------------------------------------------
  #include <fstream.h>


#include <vcl.h>
#pragma hdrstop
 #include <map.h>
#include <vector.h>
#include <math.h>

#define str AnsiString
  //Команды виртуальной машины
  enum {PUSH,HALT,CALL,GOTO,
  RET,DUP,IF,ADD,NEG,DIV,MUL,DROP,LT,EQ,
  OR,XOR,MAKEARR,SETARRELEM,GETARRELEM,POSH1,POSH2,PUP1,PUP2,
  DELARR,LSTR,SETCHAR,GETCHAR,DELSTR,NEXTSTR,
  NEXTNUM,PRINTNUM,PRINTSTR,EQUOTE,SQUOTE,STRLEN,ARRSIZE,LOAD,
  STORE,VAR,AND,NOT,STACKSIZE,SIN,POW};


 vector< map<str,unsigned long long int> >  wordscope;
       map<str,unsigned long long int> words;
       vector<   map<str,unsigned long long int> > varscope;
       map<str,unsigned long long int> vars;
       unsigned long long int varsnow=0;
       vector<str>  bytecode;
       vector<unsigned long long int> brackets;
       
       long double  tmp[5];
      vector<int>retstack;
      map<unsigned long long int,long double> varsi;

       
       unsigned long long int nowptr=1,kvob=0;
       int inword=0;
  struct  com
  {
    int opcode;
   
    char *text;
  };
 const  com commands[] = {

       {DUP,"DUP"},
       {IF,"IF"},
       {ADD,"+"},
       {NEG,"NEG"},
       {DIV,"/"},
       {MUL,"*"},
       {DROP,"DROP"},
       {LT,"<"},
       {EQ,"="},
       {OR,"OR"},
       {XOR,"XOR"},
       {MAKEARR,"MAKEARR"},
       {SETARRELEM,"SETELEM"},
       {GETARRELEM,"GETELEM"},
       {POSH1,"POSH1"},
       {POSH2,"POSH2"},
       {PUP1,"PUP1"},
       {PUP2,"PUP2"},
       {DELARR,"DELARR"},
       {SETCHAR,"SETCHAR"},
       {GETCHAR,"GETCHAR"},
       {DELSTR,"DELSTR"},
       {NEXTSTR,"NEXTSTR"},
       {NEXTNUM,"NEXTNUM"},
       {PRINTNUM,"PRINTNUM"},
       {PRINTSTR,"PRINTSTR"},
       {STRLEN,"LEN"},
       {ARRSIZE,"SIZE"},
       {CALL,"CALL"}  ,
       {LOAD,"!"},
       {NOT,"NOT"},
       {AND,"AND"},
       {STACKSIZE,"STSIZE"},
       {SIN,"SIN"},
       {POW,"POW"},
       {STORE,"@"}



   };





 class myarray
 {//реализация массива для виртуальной машины
     vector<long double> arr;       //массив
     unsigned long long int size;   //размер
     public:
     myarray()
     {

         size=0;
     }
     void setElem(unsigned long long int  n,
                        long double value)
     {
         //изменение элемента массива, в случае превышения
         //размера массива он автоматически увеличивается
         if(n>=size)
                arr.resize(n+1);
         arr[n]=value;

         size=arr.size();


     }
     long double getElem(unsigned long long int  n)
     {
         //получение элемента массива, если не существует, возвращаем 0
        if(n>=size)
                return 0;
        
        return arr[n];
     }
     unsigned long long int _size()
     {
        return size;
     }

 }  ;
  class mystr
 {//реализация строк для виртуальной машины
 public:
     str *s;       //строка
     unsigned long long int size;   //размер
     public:
     mystr(str *st)
     {
         s=st;
         size=st->Length();
     }
     ~mystr()
     {
         delete s;
     }
     void setCh(unsigned long long int  n,
                        char value)
     {

         if(n>size)
                *s=*s+*new str(" ",n-size);

         (*s)[n]=value;
         size=s->Length();
     }
     long double getElem(unsigned long long int  n)
     {

        if(n>size)
                return 0;
        return (*s)[n];
     }
     unsigned long long int _size()
     {
        return size;
     }

 }  ;



vector<long double> stack;
vector<long double> hand1;
vector<long double> hand2;

  void out(str s);
    str* inp();

  str foldstr(vector<str> tok)
  {
     str res="";
     for(unsigned long long int i=0;i<tok.size();i++)
        res=res+" "+tok[i]+" ";

     return res;

  }

  str delDead(vector<str> tokens)
  {

       map<str,unsigned long long int> words;
       //map<str,TPoint> point;

        for(unsigned long long int i=0;i<tokens.size();i++)
       {
           words[tokens[i].UpperCase()]++;
       }


       for(unsigned long long int i=0;i<tokens.size();i++)
       {
           //  ShowMessage(tokens[i].UpperCase());
           if(tokens[i].UpperCase()=="DEF" && words[tokens[i+1].UpperCase()]==1)
           {
               unsigned long long int kvo=0;
               tokens[i]=" ";
               i++;
               while(i<tokens.size())
               {
                        if(kvo==0 && tokens[i]==".")
                        {
                                tokens[i]=" ";
                                break;
                        }
                        if(tokens[i].UpperCase()=="DEF")
                                kvo++;
                        if(tokens[i]==".")
                                kvo--;
                        tokens[i]=" ";
                        i++;

               }
           }
       }
       return (foldstr(tokens));


  }






 str escape(str t)
 {
     //обработка escape последовательностей в строке
    str  res="";
    t=t+" ";
         for(int i=1;i<=t.Length()-1;i++)
                  if(t[i]=='\\' && t[i+1]=='\\')
                          res=res+t[i++] ;
                  else
                  if(t[i]=='\\' && t[i+1]=='n')
                  {
                          res=res+'\n';
                          i++;
                  }
                  else
                   if(t[i]=='\\' && t[i+1]=='s')
                  {
                          res=res+' ';
                          i++;
                  }
                  else
                  if(t[i]=='\\')
                          res=res+t[++i];
                  else
                          res=res+t[i];

        return res;
 }



 str preprocess(str code)
 {
     //препроцессор: вставляет другие файлы в текст
   str res="";
   unsigned long long int ptr=1,lastptr=1;

    ptr = code.Pos("#");
    if(ptr==0)
            return code;

     while(ptr!=0)
     {

            code=code.SubString(lastptr,code.Length());
            lastptr=1;
             ptr = code.Pos("#");
             if (ptr==0)
             {
                    res=res+code.SubString(lastptr,code.Length());
                    break;
             }
             else
                  res=res+code.SubString(lastptr,ptr-1);

            if(code.SubString(ptr+1,7).UpperCase()=="INCLUDE")
            {

                ptr+=8;
                while(ptr<=code.Length()  && code[ptr]==' ')
                    ptr++;
                str filename ="";
                unsigned long long int tmp=ptr;
                while(tmp<=code.Length()  &&  (code[tmp]!='\r' && code[tmp]!=' ' && code[tmp]!='\n'))
                    filename+=code[tmp++];
                char cstr=0;
                 ifstream f;
                f.open(filename.c_str());

                while(!f.eof())
                   {
                      f.get(cstr);
                      if(!f.eof())
                        res=res+cstr;
                       //memset(strtmp, 0, sizeof(strtmp));

                   }
                   res=res+" ";
                lastptr=tmp;
                f.close();


            }

     }
     //ShowMessage(res);
    return res;

 }








vector<str> parseCode(str code)
{
    //парсер: разбивает код по пробельным символам и ищет строки
vector<str>  res;
str now;
code=code.Trim();
for(int i=1;i<=code.Length();i++)
{
      if(code[i]!=' ' && code[i]!='\r' && code[i]!='\t' && code[i]!='\n')
      {
        if(code[i]=='\"')
        {
                i++;
                while(i<=code.Length())
                {
                        if(code[i]=='\"' && code[i-1]!='\\')
                                break;
                        now=now+code[i];
                        i++;
                }
                str tmp="";
              //  ShowMessage("0"+now+"0");
                for(int i =1; i<=now.Length();i++)
                {
                   if(now[i]==' ')
                        tmp=tmp+"\\s";

                   else
                        tmp=tmp+now[i];
                }
                now=tmp;
              //  ShowMessage("0"+now+"0");
                res.push_back("STR");


        }
        else
                now=now+code[i];

      }
      else {
           while(i<=code.Length()  && (code[i]==' ' || code[i]=='\t' || code[i]=='\n' || code[i]=='\r'))
                i++;
           i--;
           res.push_back(now.Trim());
           now="";
      }



}
     if(now!="")
        res.push_back(now.Trim());


   return   res;
}






 vector<str>   compile(vector<str> tokens)
 {
     //компилятор
       
       #define toCode(X) bytecode.push_back(X);nowptr++; 
       vector<com> coms(commands,commands+sizeof(commands)/sizeof(commands[0]));
       for(unsigned long long int i=0;i<tokens.size();i++)
    {
        if(tokens[i].UpperCase()=="DEF"){
               
                 inword++;
                 words[tokens[i+1].UpperCase()]= nowptr+1;
               wordscope.push_back(words);

               i++;
               continue;
         }
          if(tokens[i].UpperCase()=="."){
               if(inword==0)
                        throw(str("Ненужная точка"));
               inword--;
                words=wordscope[wordscope.size()-1];
                wordscope.pop_back();
               toCode(DROP);
              continue;
         }
         if(tokens[i].UpperCase()=="["){
                kvob++;
                toCode(SQUOTE);
                toCode(0);
                brackets.push_back(nowptr-2);
                varscope.push_back(vars);
                continue;
         }
         if(tokens[i].UpperCase()=="]"){
                if(kvob<=0)
                        throw(str("Лишняя скобка"));
                bytecode[brackets[brackets.size()-1]]=nowptr;
                toCode(RET);
                toCode(EQUOTE);

                brackets.pop_back();
                vars=varscope[varscope.size()-1];
                varscope.pop_back();
               continue;
         }
          if(words[tokens[i].UpperCase()])
         {
                toCode(PUSH);
                toCode(words[tokens[i].UpperCase()]);
                toCode(CALL); 
                continue;                
         }
          if(vars[tokens[i].UpperCase()])
         {
                toCode(PUSH);
                toCode(vars[tokens[i].UpperCase()]);
                continue;                
         }

         if(tokens[i].UpperCase()=="VAR"){
                toCode(VAR);
                vars[tokens[++i].UpperCase()]=++varsnow;
                toCode(varsnow);



                continue;
         }
         if(tokens[i].UpperCase()=="DEVAR"){

                vars[tokens[++i].UpperCase()]=0;
                



                continue;
         }

         if(tokens[i].UpperCase()=="TAIL"){
               //   ShowMessage( bytecode[nowptr-2].c_str());
                bytecode[nowptr-2]=GOTO;





                continue;
         }


         for(unsigned int j=0;j<coms.size();j++)
                if (tokens[i].UpperCase()==coms[j].text)
                {
                    //ShowMessage(coms[j].text);
                    toCode(coms[j].opcode);
                    goto exitloop;
                }
          goto next;
         exitloop:
                continue;
         next:;
         if(tokens[i].UpperCase()=="STR"){
                toCode(LSTR);
                i++;
                toCode(escape(tokens[i]));
                toCode("\"");
                continue;

         }
         if(tokens[i].UpperCase()=="'"){
                toCode(PUSH);
                toCode((int)(escape(tokens[++i])[1]));

                continue;

         }
         try
         {
                tokens[i].ToDouble();
                toCode(PUSH);
                toCode(tokens[i].ToDouble());
         }
         catch(...)
         {
                if(tokens[i]=="(")
                        while(tokens[i]!=")")
                                i++;
                else
                {
                /*ShowMessage(tokens[i-3]);
                     ShowMessage(tokens[i-2]);
                     ShowMessage(tokens[i-1]);
               ShowMessage("0"+tokens[i]+"0");
                     ShowMessage(tokens[i+1]);
                     ShowMessage(tokens[i+2]);
                     ShowMessage(tokens[i+3]);   */
                    // if(tokens[i]!="")   //костыль
                        throw("Неизвестное слово: "+tokens[i]);

                }
         }




    }
    toCode(HALT);
    return bytecode;
 #undef toCode
 }



   void save(vector<str> bytecode,str filename)
   {
       //сохранение байткода в файл
       ofstream f;
       f.open(filename.c_str());

       for(unsigned long long int i=0;i<bytecode.size();i++)
       {
           f<<bytecode[i].c_str()<<" ";
       }
       f.close();

   }
   vector<str> load(str filename)
   {
       //загрузка байткода из файла
       ifstream f;
       vector<str> res;
       f.open(filename.c_str());
       char  tmp[50];
        char forStr,last='0';
       while(!f.eof())
       {

          f>> tmp;
           str t(tmp);
            if (t==LSTR)
            {

                res.push_back(t);
                t="";
                f.get();
               while(!f.eof())
               {
                       f.get(forStr);
                       if(forStr=='\"' && last!='\\')
                        {

                            break;
                        }
                       t=t+forStr;

                       last=forStr ;
               }

               t=escape(t);
                res.push_back(t);
                res.push_back("\"");
           }
           else
                res.push_back(t);
          if(f.fail())
                break;
       }




       f.close();

       return res;
   }


    #define DEBUG_VM
void execute(unsigned long long int start)
{
    //виртуальная машина
      
      #ifdef DEBUG_VM
          ofstream log;
          log.open("log.txt");
          ofstream logstack;
          logstack.open("logstack.txt");
      #endif
  /*  for(int i = 0 ;i<bytecode.size();i++)
        ShowMessage(bytecode[i]);*/
    for(unsigned long long int i=start;i<bytecode.size();)
    {
         /*if(bytecode[i]==ENTRY){
                 i=bytecode[i+1].ToInt();
         }   */
         
           #ifdef DEBUG_VM
                logstack<<"from"<<endl;
               for(int i=stack.size()-1;i>=0;i--)
                logstack<<"\t\t"<<stack[i]<<endl;
                logstack<<"to"<<endl;
           #endif

         if(bytecode[i]==PUSH){
                 stack.push_back(bytecode[i+1].ToDouble());
                  #ifdef DEBUG_VM
                     log<<"pushed:"<< bytecode[i+1].c_str()<<" I:"<<i<<endl;
                #endif
                 i+=2;
                 continue;
         }
         if(bytecode[i]==HALT){
                 #ifdef DEBUG_VM
                     log<<"halted"<<" I:"<<i<<endl;
                #endif
                 return;
        }
        if(bytecode[i]==CALL){
        #ifdef DEBUG_VM
                     log<<"called:"<< bytecode[i+1].c_str()<<" I:"<<i<<endl;
                #endif
                /*   retstack.push_back(i+2);
                 i=bytecode[i+1].ToInt();  */
                retstack.push_back(i+1);
                 i=stack[stack.size()-1];
                 stack.pop_back();
                 continue;
        }
         if(bytecode[i]==RET){
        #ifdef DEBUG_VM
                     log<<"returned:"<<retstack[retstack.size()-1]<<" I:"<<i<<endl;
                #endif
                 i=retstack[retstack.size()-1];
                 retstack.pop_back();
                 continue;

        }
        /*if (i>=bytecode.size())
        ShowMessage(i); */
        if(bytecode[i]==SQUOTE){
        #ifdef DEBUG_VM
                     log<<"squoted:"<<bytecode[i].c_str()<<" I:"<<i<<endl;
         #endif
                 stack.push_back(i+2);
                  i=bytecode[i+1].ToInt()+1;

                 continue;

        }
        if(bytecode[i]==PRINTNUM){
        #ifdef DEBUG_VM
                     log<<"outed:"<<stack[stack.size()-1]<<" I:"<<i<<endl;
                #endif
                 out(FloatToStr(stack[stack.size()-1]));
                 stack.pop_back();
                 i++;
                 continue;
        }
        if(bytecode[i]==PRINTSTR){
        #ifdef DEBUG_VM
                     log<<"outed:"<<stack[stack.size()-1]<<" I:"<<i<<endl;
                #endif
                 out(*(*(mystr*)(stack[stack.size()-1])).s);
                 
                 i++;
                 continue;
        }
        if(bytecode[i]==DUP){
        #ifdef DEBUG_VM
                     log<<"duped:"<<stack[stack.size()-1]<<" I:"<<i<<endl;
                #endif
                 stack.push_back(stack[stack.size()-1]);
                 i++;
                 continue;
        }
        if(bytecode[i]==IF){
        #ifdef DEBUG_VM
                     log<<"ifed,cond:"<<stack[stack.size()-3]<<"then:"<<stack[stack.size()-2]<<"else:"<< stack[stack.size()-1]<<" I:"<<i<<endl;
                #endif
                 if(  stack[stack.size()-3]!=0)
                 {
                        retstack.push_back(i+1);
                        i=  stack[stack.size()-2];
                 }
                 else
                 {
                     retstack.push_back(i+1);
                     i=  stack[stack.size()-1];
                 }
                 stack.erase(stack.end()-3,stack.end())   ;
                 continue;
        }
        if(bytecode[i]==ADD){
         #ifdef DEBUG_VM
                     log<<"added:"<<stack[stack.size()-2]<<" "<<stack[stack.size()-1]<<" I:"<<i<<endl;
         #endif
                  stack[stack.size()-2]+= stack[stack.size()-1];
                  stack.pop_back();
                  i++;
                  continue;
        }
        if(bytecode[i]==MUL){
         #ifdef DEBUG_VM
                     log<<"muled:"<<stack[stack.size()-2]<<" "<<stack[stack.size()-1]<<" I:"<<i<<endl;
         #endif
                  stack[stack.size()-2]*= stack[stack.size()-1];
                  stack.pop_back();
                  i++;
                  continue;
        }
        if(bytecode[i]==DIV){
         #ifdef DEBUG_VM
                     log<<"dived:"<<stack[stack.size()-2]<<" "<<stack[stack.size()-1]<<" I:"<<i<<endl;
         #endif
                  stack[stack.size()-2]/= stack[stack.size()-1];
                  stack.pop_back();
                  i++;
                  continue;
        }
        if(bytecode[i]==NEG){
         #ifdef DEBUG_VM
                     log<<"negated:"<<stack[stack.size()-1]<<" I:"<<i<<endl;
         #endif
                  stack[stack.size()-1]=-stack[stack.size()-1];

                  i++;
                  continue;
        }
        if(bytecode[i]==DROP){
         #ifdef DEBUG_VM
                     log<<"droped:"<<stack[stack.size()-1]<<" I:"<<i<<endl;
         #endif
                stack.pop_back();


                  i++;
                  continue;
        }
         if(bytecode[i]==LT){
        #ifdef DEBUG_VM
                    log<<"littled:"<<stack[stack.size()-2]<<" "<<stack[stack.size()-1]<<" I:"<<i<<endl;
        #endif
               tmp[0]= stack[stack.size()-2];
           tmp[1]= stack[stack.size()-1];
           stack.pop_back();
           stack.pop_back();

               stack.push_back(tmp[0]<tmp[1]);


                 i++;
                 continue;
       }


        if(bytecode[i]==EQ){
         #ifdef DEBUG_VM
                     log<<"equaled:"<<stack[stack.size()-2]<<" "<<stack[stack.size()-1]<<" I:"<<i<<endl;
         #endif
         tmp[0]= stack[stack.size()-2];
         tmp[1]= stack[stack.size()-1];
         stack.pop_back();
         stack.pop_back();
                stack.push_back(tmp[0]==tmp[1]);


                  i++;
                  continue;
        }

        if(bytecode[i]==OR){
         #ifdef DEBUG_VM
                     log<<"ored:"<<stack[stack.size()-2]<<" "<<stack[stack.size()-1]<<" I:"<<i<<endl;
         #endif
          tmp[0]= stack[stack.size()-1];
           tmp[1]= stack[stack.size()-2];
           stack.pop_back();
           stack.pop_back();
                stack.push_back(tmp[0] || tmp[1]);


                  i++;
                  continue;
        }
        if(bytecode[i]==AND){
         #ifdef DEBUG_VM
                     log<<"anded:"<<stack[stack.size()-2]<<" "<<stack[stack.size()-1]<<" I:"<<i<<endl;
         #endif
          tmp[0]= stack[stack.size()-1];
           tmp[1]= stack[stack.size()-2];
           stack.pop_back();
           stack.pop_back();
                stack.push_back(tmp[0] && tmp[1]);


                  i++;
                  continue;
        }
        if(bytecode[i]==NOT){
         #ifdef DEBUG_VM
                     log<<"anded:"<<stack[stack.size()-2]<<" "<<stack[stack.size()-1]<<" I:"<<i<<endl;
         #endif
          stack[stack.size()-1]=!stack[stack.size()-1];



                  i++;
                  continue;
        }
        if(bytecode[i]==XOR){
         #ifdef DEBUG_VM
                     log<<"xored:"<<stack[stack.size()-1]<<" I:"<<i<<endl;
         #endif
         tmp[0]= stack[stack.size()-1];
         tmp[1]= stack[stack.size()-2];
         stack.pop_back();
         stack.pop_back();
         stack.push_back((int)tmp[0] ^ (int)tmp[1]);
                  i++;
                  continue;
        }
         if(bytecode[i]==MAKEARR){
         #ifdef DEBUG_VM
                     log<<"array created"<<" I:"<<i<<endl;
         #endif
                stack.push_back((int)(new myarray()));
                  i++;
                  continue;
        }

        if(bytecode[i]==DELARR){
         #ifdef DEBUG_VM
                     log<<"array deleted"<<" I:"<<i<<endl;
         #endif
                 delete ((myarray*)stack[stack.size()-1]);
                stack.pop_back();
                  i++;
                  continue;
        }

         if(bytecode[i]==SETARRELEM){
         #ifdef DEBUG_VM
                     log<<"arr element seted:"<< stack[stack.size()-1] <<" I:"<<i<<endl;
         #endif
                ((myarray*)stack[stack.size()-3])->setElem(stack[stack.size()-1],stack[stack.size()-2]);
                stack.pop_back();
                stack.pop_back();                
                  i++;
                  continue;
        }
        if(bytecode[i]==GETARRELEM){
         #ifdef DEBUG_VM
                     log<<"arr element geted:"<< stack[stack.size()-1] <<" I:"<<i<<endl;
         #endif

         tmp[0]=stack[stack.size()-1];
         stack.pop_back();
                stack.push_back(((myarray*)stack[stack.size()-1])->getElem(tmp[0]));
                  i++;
                  continue;
        }
        if(bytecode[i]==POSH1){
         #ifdef DEBUG_VM
                     log<<"poshed1:"<< stack[stack.size()-1] <<" I:"<<i<<endl;
         #endif

         hand1.push_back( stack[stack.size()-1]);
         stack.pop_back();
                  i++;
                  continue;
        }
        if(bytecode[i]==POSH2){
         #ifdef DEBUG_VM
                     log<<"poshed2:"<< stack[stack.size()-1] <<" I:"<<i<<endl;
         #endif

         hand2.push_back( stack[stack.size()-1]);
         stack.pop_back();
                  i++;
                  continue;
        }
        if(bytecode[i]==PUP1){
         #ifdef DEBUG_VM
                     log<<"puped1:"<< stack[stack.size()-1] <<" I:"<<i<<endl;
         #endif

         stack.push_back( hand1[hand1.size()-1]);
         hand1.pop_back();
                  i++;
                  continue;
        }
        if(bytecode[i]==PUP2){
         #ifdef DEBUG_VM
                     log<<"puped2:"<< stack[stack.size()-1] <<" I:"<<i<<endl;
         #endif
         stack.push_back( hand2[hand2.size()-1]);
         hand2.pop_back();
                  i++;
                  continue;
        }
         if(bytecode[i]==LSTR){
         #ifdef DEBUG_VM
                     log<<"str loaded:"<<bytecode[i+1].c_str() <<"I:"<<i<<endl;
         #endif

         stack.push_back((int)new mystr(&bytecode[++i]))   ;
                  i+=2;
                  continue;
        }
        if(bytecode[i]==NEXTSTR){
         #ifdef DEBUG_VM
                     log<<"inped"<<" I:"<<i<<endl;
         #endif

         stack.push_back((int)new mystr(inp()));
                  i++;
                  continue;
        }
        if(bytecode[i]==NEXTNUM){
         #ifdef DEBUG_VM
                     log<<"n inped"<<" I:"<<i<<endl;
         #endif
         tmp[0]=(int)inp();
         stack.push_back((*(str*)tmp[0]).ToDouble());
         delete (str*)tmp[0];
                  i++;
                  continue;
        }
        if(bytecode[i]==SETCHAR){
         #ifdef DEBUG_VM
                     log<<"char seted:"<< stack[stack.size()-2] <<" I:"<<i<<endl;
         #endif

         (*(mystr *)stack[stack.size()-3]).setCh(stack[stack.size()-1],(char)stack[stack.size()-2])   ;

         //(*((str*)stack[stack.size()-3]))[stack[stack.size()-1]]=(char)stack[stack.size()-2];
            stack.pop_back();
            stack.pop_back();
                  i++;
                  continue;
        }
        if(bytecode[i]==GETCHAR){
         #ifdef DEBUG_VM
                     log<<"char geted:"<< stack[stack.size()-1] <<" I:"<<i<<endl;
         #endif
          tmp[0]= stack[stack.size()-1];
          stack.pop_back();
          stack.push_back(    ( *(mystr *)stack[stack.size()-1]).getElem(tmp[0])  );
     //    stack.push_back((*((str*)stack[stack.size()-1]))[tmp[0]]);
                  i++;
                  continue;
        }
        if(bytecode[i]==DELSTR){
         #ifdef DEBUG_VM
                     log<<"str deleted:"<< stack[stack.size()-1] <<" I:"<<i<<endl;
         #endif
          delete (mystr*)stack[stack.size()-1];
          stack.pop_back();
                  i++;
                  continue;
        }
        if(bytecode[i]==STRLEN){
         #ifdef DEBUG_VM
                     log<<"str lengthed:"<< stack[stack.size()-1] <<" I:"<<i<<endl;
         #endif
          stack.push_back((*(mystr*)stack[stack.size()-1])._size());
                  i++;
                  continue;
        }
        if(bytecode[i]==ARRSIZE){
         #ifdef DEBUG_VM
                     log<<"arr sized:"<< stack[stack.size()-1] <<" I:"<<i<<endl;
         #endif
          stack.push_back((*(myarray*)stack[stack.size()-1])._size());
                  i++;
                  continue;
        }
        if(bytecode[i]==GOTO){
         #ifdef DEBUG_VM
                     log<<"gotoed"<<" I:"<<i<<endl;
         #endif
          i=stack[stack.size()-1];
          stack.pop_back();

                  continue;
        }
        if(bytecode[i]==VAR){
         #ifdef DEBUG_VM
                     log<<"vared"<<" I:"<<i<<endl;
         #endif
          varsi[bytecode[++i].ToDouble()]=0;
          i++;
                  continue;
        }
        if(bytecode[i]==LOAD){
         #ifdef DEBUG_VM
                     log<<"loaded"<<" I:"<<i<<endl;
         #endif
          tmp[0]=varsi[stack[stack.size()-1]];
          stack.pop_back();
          stack.push_back(tmp[0]);
          i++;

                  continue;
        }
        if(bytecode[i]==STORE){
         #ifdef DEBUG_VM
                     log<<"stored"<<" I:"<<i<<endl;
         #endif
          varsi[stack[stack.size()-2]]=stack[stack.size()-1];
          stack.pop_back();
          stack.pop_back();
          i++;


                  continue;
        }
        if(bytecode[i]==STACKSIZE){
         #ifdef DEBUG_VM
                     log<<"sized"<<" I:"<<i<<endl;
         #endif
          stack.push_back(stack.size());
          i++;


                  continue;
        }
        if(bytecode[i]==SIN){
         #ifdef DEBUG_VM
                     log<<"sined"<<" I:"<<i<<endl;
         #endif
          stack[stack.size()-1]=sin(stack[stack.size()-1]);
          i++;


                  continue;
        }
        if(bytecode[i]==POW){
         #ifdef DEBUG_VM
                     log<<"powed"<<" I:"<<i<<endl;
         #endif
          stack[stack.size()-2]=pow(stack[stack.size()-2],stack[stack.size()-1]);
          stack.pop_back();
          i++;


                  continue;
        }

              //STACKSIZE,SIN,POW};
         #ifdef DEBUG_VM
                     log<<"undefined:"<< bytecode[i].c_str() <<" I:"<<i<<endl;
         #endif
         i++;


    }
    #ifdef DEBUG_VM

      log.close();

      logstack.close();
    #endif
}
