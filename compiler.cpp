#include "compiler.h"

QString Compiler::GetStyle(QString word)
{
return "<p>" + word + "</p>";
}
QString Compiler::DrawText(QString _Val)
{
QString returnValue, buffer;
    for(int i=0;i<_Val.size();i++)
        if(_Val[i]=="\n")
        {
            if(i!=0)
                if(_Val[i-1]=="\n")
                    buffer+=" ";
        returnValue+=GetStyle(buffer);
        buffer = "";
        }
        else  buffer+=_Val[i];
    returnValue+=GetStyle(buffer);
return returnValue;
}
QString Compiler::GetErrorQString()
{
    return "";
}
bool Compiler::UpdateCompleter(QString pathToFile)
{
return 1;
}
