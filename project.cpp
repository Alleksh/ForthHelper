#include "project.h"
#include <QStandardPaths>
Project::Project(QString ProjectFolder)
{
    qDebug() << "Project constructor called";
    this->ProjectFolder = ProjectFolder;
    std::string abcd  = ProjectFolder.toStdString(),name;
    abcd.erase(abcd.end()-1);
    while(abcd[abcd.size()-1]!='/')
    {
        name+=abcd[abcd.size()-1];
        abcd.erase(abcd.end()-1);
    }
    std::reverse(name.begin(),name.end());
    ProjectName = QString::fromStdString(name);
    BackupFolder = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    BackupFolder+="/backup/";
    if(!QDir(BackupFolder).exists())
        QDir().mkdir(BackupFolder);
    BackupFolder+=ProjectName+"/";
    if(!QDir(BackupFolder).exists())
        QDir().mkdir(BackupFolder);
    qDebug() << "BackupPath: " << BackupFolder;
    qDebug() << "ProjectFolder: " << ProjectFolder;
    qDebug() << "ProjectName: " << ProjectName;
}
//Create project
void Project::Init()
{
    qDebug() << "Init called";
    QFile file(ProjectFolder + "Project.frthp");
    file.open(QIODevice::WriteOnly);
    file.close();
    QDir().mkdir(ProjectFolder + "Source Files");
    QFile file1(ProjectFolder + "Source Files/Main.forth");
    file1.open(QIODevice::WriteOnly);
    file1.close();
    AddFile(ProjectFolder + "Source Files/Main.forth");
}
void Project::AddFile(QString PathToFile)
{
    qDebug() << "AddFile called";
    QFile* file = new QFile(ProjectFolder + "Project.frthp");
    file->open(QIODevice::ReadOnly);
    QTextStream * QTS = new QTextStream(file);
    QString File = QTS->readAll();
    file->close();
    delete file;
    delete QTS;
    file = new QFile(ProjectFolder + "Project.frthp");
    file->open(QIODevice::WriteOnly);
    QTS = new QTextStream(file);
    (*QTS) << File << PathToFile << "\n";
    file->close();
    delete QTS;
    delete file;
}
void Project::ChangeName(QString PathToFile, QString NewName)
{
    qDebug() << "ChangeName called";
}
void Project::RemoveFile(QString PathToFile)
{
    qDebug() << "RemoveFile called";

}
QStringList Project::GetFilePaths()
{
    qDebug() << "GetFilePaths called";
    QFile project(ProjectFolder + "Project.frthp");
    QStringList returnValue;
    project.open(QIODevice::ReadOnly);
    QTextStream qts(&project);
    while (!qts.atEnd())
    {
        returnValue += qts.readLine();
    }
    project.close();
    return returnValue;
}
std::vector<std::pair<QString, QString>> Project::GFPaN()
{
    qDebug() << "GFPaN called";
    std::vector<std::pair<QString,QString>> returnValue;
    auto QSL(GetFilePaths());
    for(int i=0;i<QSL.size();i++)
    {
        std::pair<QString,QString> pair;
        std::string name, buffer = (pair.first = QSL[i]).toStdString();
        for(size_t j=buffer.size()-1;j < buffer.size() && buffer[j]!='/';j--)
        {
            name+=buffer[j];
            buffer.erase(buffer.end()-1);
        }
        std::reverse(name.begin(),name.end());
        pair.second = QString::fromStdString(name);
        returnValue.push_back(pair);
    }
    return returnValue;
}
