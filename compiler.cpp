#include "compiler.h"

QString Compiler::GetLineStyle(QString word)
{
    if(word=="&nbsp;") return "<p>"  + word + "</p>";
    return "<p>"  + word + "</p>";
}
QString Compiler::DrawText(QString _Val)
{
QString returnValue, buffer;
    for(int i=0;i<_Val.size();i++)
        if(_Val[i]=='\n')
        {
            if(i>1)
                if((_Val[i-2]=='\n' && _Val[i-1]==' ') || (_Val[i-1]=='\n'))
                    buffer+="&nbsp;";
        returnValue+=GetLineStyle(buffer);
        buffer = "";
        }
        else  buffer+=_Val[i];
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
