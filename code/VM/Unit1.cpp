#include <vcl.h>
#include <iostream.h>
#include "interpreter/interpreter.cpp"
#pragma argsused
str * inp()
{
  char tmp[10000];
 cin>>tmp;
 return new str(tmp);
}
void out(str s)
{
 cout<<s.c_str()<<endl;
}
int main(int argc, char* argv[])
{
       if(argc==1)
           cout<<"Usage rpnvm.exe file\n";
        if(argc>2)
           cout<<"Wrong arg count\n";
        if(argc==2)
        {
            vector<str> tt= load(argv[1])  ;
            execute(tt ,0);
        }
        return 0;
}