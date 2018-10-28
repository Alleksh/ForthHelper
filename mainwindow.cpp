#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "editwordlistdialog.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->Files->clear();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete project;
}
void MainWindow::configure()
{   qDebug() << "configure called";
    if(project==nullptr) return;
    ui->Status->showMessage("Выводим...");
    ui->ProjectName->setText(project->ProjectName);
    QStringList filters;
    auto list = project->GFPaN();
    for(size_t i=0;i<list.size();i++) filters << list[i].second;
    qDebug() << project->ProjectFolder;
    QDirModel *model = new QDirModel;
    for(size_t i=0;i<Tabs.size();i++) Tabs[i]->save();
    Tabs.clear();
    ui->Files->clear();
    model->setFilter(QDir::AllDirs | QDir::Files | QDir::NoDotAndDotDot);
    model->setNameFilters(filters);
    ui->ProjectTree->setModel(model);
    ui->ProjectTree->setRootIndex(model->index(project->ProjectFolder));
    ui->ProjectTree->hideColumn(1);
    ui->ProjectTree->hideColumn(2);
    ui->ProjectTree->hideColumn(3);
    ui->ProjectTree->header()->hide();
    ui->Status->showMessage("Готово!",5000);
}
void MainWindow::on_OpenProject_triggered()
{
    qDebug() << "OpenProject triggered";
    ui->Status->showMessage("Открываем проект...");
    QString Path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if(!QDir(Path).exists())
        QDir().mkdir(Path);
    Path += "/projects";
    QString Directory = QFileDialog::getOpenFileName(this,"Open Project", Path, "Project.frthp");
    if(Directory=="") return;
    std::string abcd  = Directory.toStdString();
    abcd.erase(abcd.end()-1);
    while(abcd[abcd.size()-1]!='/')
    {
        abcd.erase(abcd.end()-1);
    }
    this->project = new Project(QString::fromStdString(abcd));
    configure();
}
void MainWindow::on_CreateProject_triggered()
{
    qDebug() << "CreateProject triggered";
    ui->Status->showMessage("Создаем проект...");
    QString Path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    if(!QDir(Path).exists())
        QDir().mkdir(Path);
    Path += "/projects";
    QString Directory = QFileDialog::getExistingDirectory(this,"Create Project", Path, QFileDialog::ShowDirsOnly);
    if(Directory=="") return;
    if(QDir(Directory).entryList().size()>2)
    {
        ui->Status->showMessage("Ошибка! Папка не пуста!",10000);
        return;
    }
    this->project = new Project(Directory + "/");
    project->Init();
    configure();
}
void MainWindow::on_SaveAndExit_triggered()
{
    for(size_t i=0;i<Tabs.size();i++)
        Tabs[i]->save();
    exit(EXIT_SUCCESS);
}
void MainWindow::on_ProjectTree_doubleClicked(const QModelIndex &index)
{
    qDebug() << "on_ProjectTree_doubleClicked";
    ui->Status->showMessage("Открываем " + index.data().toString());
    bool _Val = 0;
    for(size_t i=0;i<Tabs.size();i++)
    {
        if(Tabs[i]->filename==index.data().toString())
        {
            _Val = 1;
            break;
        }
    }
    if(_Val)
    {
        ui->Status->showMessage("Произошла ошибка. Данный файл уже открыт.",7500);
        return;
    }
    QString x = index.data().toString();
    auto list = project->GFPaN();
    size_t id = 0;
    for(;id<list.size();id++)
    {
        if(list[id].second==x) break;
    }
    if(id==list.size())
    {
        ui->Status->showMessage("Произошла ошибка. Выбран(а) неверный файл(папка).",7500);
        return;
    }
    CodeEditor* abc = new CodeEditor(ui->TextAnalysisOutput,project,list[id].first,list[id].second,this);
    Tabs.push_back(abc);
    ui->Files->addTab(abc,list[id].second);
    ui->Status->showMessage("Готово!",5000);
}
void MainWindow::on_Files_tabCloseRequested(int index)
{
    qDebug("on_Files_tabCloseRequested");
    QString tabText = ui->Files->tabText(index);
    bool _Val = 0;
    for(size_t i=0;i<Tabs.size();i++)
        if(Tabs[i]->filename==tabText)
        {
            delete Tabs[i];
            Tabs.erase(Tabs.begin() + int(i));
            _Val = 1;
            break;
        }
    if(!_Val)
        return;
    ui->Files->removeTab(index);
    ui->Status->showMessage("Готово!",5000);
}
void MainWindow::on_CompilerOutput_tabCloseRequested(int index)
{
    if(index==0)
    {
        ui->Status->showMessage("Простите! Данную вкладку закрыть нельзя.",2000);
        return;
    }
    ui->CompilerOutput->removeTab(index);
    ui->Status->showMessage("Готово!",5000);
}
#include <QMessageBox>
void MainWindow::on_EditCurrentWordList_triggered()
{
    EditWordListDialog EWLD (this);
    EWLD.open();
    EWLD.exec();
    if(project!=nullptr)
    if(QMessageBox::question(nullptr,"", "Вы изменили словарь. \nЧтобы изменения вступили в силу,\n необходимо перезагрузить проект.\n Перезагрузить?")!=65536)
    configure();
}
