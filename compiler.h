#ifndef COMPILER_H
#define COMPILER_H
#include "project.h"
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
class Compiler
{
public:
    std::vector<Word> WL;
    Project* project;
    Compiler(Project* project)
    {
        this->project = project;
    }
    ~Compiler()
    {
        delete project;
    }
    QString GetStyle(QString);
    QString DrawText(QString);
    QString GetErrorQString();
    bool UpdateCompleter(QString);
};

#endif // COMPILER_H
