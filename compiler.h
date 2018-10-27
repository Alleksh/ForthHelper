#ifndef COMPILER_H
#define COMPILER_H
#include "project.h"
struct Word
{

    QString StartWord,
            EndWord;
    int NeedNumbers, GiveNumbers;
    //0 var
    //1 func
    bool    VF,
            one_word,
            StartAndEndWordInOneLine,
            CreateNewWord;
};

class WordList
{
public:
    std::vector<Word> list;
    WordList()
    {

    }
};
class Compiler
{
public:
    WordList WL;
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
