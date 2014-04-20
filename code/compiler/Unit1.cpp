#include <vcl.h>
#include <iostream.h>
#include "interpreter/interpreter.cpp"
#pragma argsused
int main(int argc, char* argv[])
{
        if(argc==1)
           cout<<"Usage rpnc.exe file\n";
        if(argc>2)
           cout<<"Wrong arg count\n";

        if(argc==2)
        {
           str last="",now="";
           char tstr[300]="";
           strcat(tstr, "#include ");
           strcat(tstr,    argv[1]  );

           now =preprocess(tstr);
           while(now!=last)
           {
                last=now;
                now= preprocess(last);
           }
           try
           {
                char tstr[300]="";

                strcat(tstr, "c");
                strcat(tstr,    argv[1]  );
               save( compile(parseCode(last)),tstr);
           } catch(str msg)
           {
                cout<<msg.c_str();
           }
        }
        return 0;
}