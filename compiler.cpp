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
bool Compiler::UpdateCompleter(QString pathToFile, QString Text)
{
    QString _Value;
    for(size_t i=0;i<WL.size();i++)
    {
        _Value+= WL[i].StartWord + '\n';
        for(int y=0;y<WL[i].EndWords.size();y++)
            if(WL[i].EndWords[y]!=' ') _Value+=WL[i].EndWords[y];
            else _Value+='\n';
    }
    QFile file2(pathToFile);file2.open(QIODevice::ReadOnly);
    QTextStream* QTS = new QTextStream(&file2);
    if(QTS->readAll()==_Value)
    {
        delete QTS;
        return 0;
    }
    delete QTS;
    QFile file(pathToFile);file.open(QIODevice::WriteOnly);
    QTS = new QTextStream(&file);
    (*QTS) << _Value;
    file.close();
    delete QTS;
    return 1;
}
