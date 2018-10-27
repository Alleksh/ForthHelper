#ifndef PROJECT_H
#define PROJECT_H
#include <QDir>
#include <QDebug>
#include <list>

class Project
{
public:
    QString ProjectFolder;
    QString ProjectName;
    QString BackupFolder;
    Project(QString);
    void Init();
    void AddFile(QString);
    void ChangeName(QString, QString);
    void RemoveFile(QString);
    QStringList GetFilePaths();
    //get file paths and names
    std::vector<std::pair<QString, QString>>  GFPaN();
};

#endif // PROJECT_H
