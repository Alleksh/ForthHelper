#include "compiler.h"

std::vector<QStringList> Compiler::getWords()
{
    access.lock();
    std::vector<QStringList> returnValue = WList;
    access.unlock();
    return returnValue;
}
QString Compiler::GetErrorQString()
{
    return ErrList;
}
bool Compiler::UpdateCompleter(QString pathToFile)
{
    QString _Value;
    for(size_t i=0;i<WL.size();i++)
    {
        _Value+= WL[i].StartWord + '\n';
        for(int y=0;y<WL[i].EndWords.size();y++)
            if(WL[i].EndWords[y]!=' ') _Value+=WL[i].EndWords[y];
            else _Value+='\n';
    }
    access.lock();
    for(size_t i=0;i<WList->size();i++)
        for(int j=0;j<(*WList)[i].size();j++)
            _Value += (*WList)[i][j] + '\n';
    access.unlock();
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
void Compiler::UpdateCompiler(QString text)
{
    access.lock();
    this->nowText = text;
    access.unlock();
}
void CompilerThread(Compiler* compiler)
{
    qDebug() << "Thread started";
    QString previousText;
    while(compiler!=nullptr)
    {
       std::this_thread::sleep_for(std::chrono::milliseconds(50));
       if(compiler->close) break;
       compiler->access.lock();
       //error list

       compiler->access.unlock();
       std::this_thread::sleep_for(std::chrono::milliseconds(25));
       if(compiler->close) break;
       compiler->access.lock();
       //WList

       compiler->access.unlock();
    }
    qDebug() << "Thread closed";
}
