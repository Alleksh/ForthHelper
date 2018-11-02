#ifndef COMPILER_H
#define COMPILER_H
#include "project.h"
#include <thread>
#include <mutex>
struct Word
{

    QString StartWord,
            EndWords,
            description;
    int     NeedNumbers,
            GiveNumbers;
    //0 var
    //1 func
    bool    VF,
            one_word,
            StartAndEndWordInOneLine,
            CreatesNewWord;
    bool operator==(Word& _Val)
    {
           return   (StartWord==_Val.StartWord && EndWords==_Val.EndWords && description==_Val.description &&
                    NeedNumbers==_Val.NeedNumbers && GiveNumbers==_Val.GiveNumbers && VF==_Val.VF && one_word == _Val.one_word &&
                    StartAndEndWordInOneLine==_Val.StartAndEndWordInOneLine && CreatesNewWord==_Val.CreatesNewWord);
    }
    bool operator==(QString& _Val)
    {
       return (StartWord==_Val);
    }
    bool operator==(const char*& _Val)
    {
        return StartWord==_Val;
    }

};
class Compiler;
void CompilerThread(Compiler* compiler);
class Compiler
{
public:
    std::vector<Word> WL;
    Project* project;
    QString nowText,
            ErrList;
    std::vector<QStringList> WList;
    std::mutex access;
    bool close = 0;
    Compiler(Project* project)
    {
        WList = new std::vector<QStringList>;
        std::thread x(CompilerThread,this); x.detach();
        this->project = project;
        QFile file("words.txt");
        file.open(QIODevice::ReadOnly);
        QTextStream qts(&file);
        while(!qts.atEnd())
        {
            Word word;
            if(qts.atEnd()) break;
            QString str = qts.readLine();
            word.StartWord = str;
            if(qts.atEnd()) break;
            str = qts.readLine();
            word.EndWords = str;
            if(qts.atEnd()) break;
            str = qts.readLine();
            word.NeedNumbers = str.toInt();
            if(qts.atEnd()) break;
            str = qts.readLine();
            word.GiveNumbers = str.toInt();
            if(qts.atEnd()) break;
            str = qts.readLine();
            word.VF = str.toInt();
            if(qts.atEnd()) break;
            str = qts.readLine();
            word.one_word = str.toInt();
            if(qts.atEnd()) break;
            str = qts.readLine();
            word.StartAndEndWordInOneLine = str.toInt();
            if(qts.atEnd()) break;
            str = qts.readLine();
            word.CreatesNewWord = str.toInt();
            if(qts.atEnd()) break;
            std::string Str = qts.readLine().toStdString();
            for(size_t i=0;i<Str.size();i++)
            if(i!=0)
            if(Str[i-1]=='%')
            {
                if(i!=1)
                {
                    if(Str[i-2]=='%')
                    {
                        Str.erase((Str.begin() + int(i)) -1);
                        continue;
                    }
                }
                if(Str[i]=='#')
                {
                    Str[i]='\n';
                    Str.erase((Str.begin() + int(i)) -1);
                }
                else if(Str[i]=='%')
                    Str.erase((Str.begin() + int(i)) -1);
            }
            word.description = QString::fromStdString(Str);
            WL.push_back(word);
        }
        file.close();
    }
    ~Compiler()
    {
        close = 1;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    std::vector<QStringList> getWords();
    QString GetErrorQString();
    bool UpdateCompleter(QString);
    void UpdateCompiler(QString text);
};
#endif // COMPILER_H
