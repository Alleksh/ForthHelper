#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "editwordlistdialog.h"
#include "changenamewidget.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->Files->clear();
    ui->ProjectTree->setRootIsDecorated(false);
    ui->ProjectTree->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->ProjectTree, SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(ShowContextMenu(QPoint)));
}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::LoadProjectTree()
{
    QStringList filters;
    auto list = project->GFPaN();
    for(size_t i=0;i<list.size();i++) filters << list[i].second;
    if(filters.isEmpty()) filters << ":.:";
    QDirModel *model = new QDirModel;
    model->setFilter(QDir::AllDirs | QDir::Files | QDir::NoDotAndDotDot);
    model->setNameFilters(filters);
    ui->ProjectTree->setModel(model);
    ui->ProjectTree->setRootIndex(model->index(project->ProjectFolder));
    ui->ProjectTree->hideColumn(1);
    ui->ProjectTree->hideColumn(2);
    ui->ProjectTree->hideColumn(3);
    ui->ProjectTree->header()->hide();
}
void MainWindow::configure()
{
    qDebug() << "configure called";
    if(project==nullptr) return;
    ui->Status->showMessage("Выводим...");
    ui->ProjectName->setText(project->ProjectName);
    qDebug() << project->ProjectFolder;
    for(size_t i=0;i<Tabs.size();i++) Tabs[i]->save();
    LoadProjectTree();
    Tabs.clear();
    ui->Files->clear();
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
    for(size_t i=0;i<Tabs.size();i++)
    {
        if(Tabs[i]->filename==index.data().toString())
        {
            for(int i=0;i<ui->Files->count();i++)
                if(ui->Files->tabText(i)==index.data().toString())
                {
                    ui->Files->setCurrentIndex(i);
                    break;
                }
            ui->Status->showMessage("Произошла ошибка. Данный файл уже открыт.",7500);
            return;
            break;
        }
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
void MainWindow::on_EditCurrentWordList_triggered()
{
    EditWordListDialog EWLD (this);
    EWLD.open();
    EWLD.exec();
    if(project!=nullptr)
    if(QMessageBox::question(nullptr,"", "Вы изменили словарь. \nЧтобы изменения вступили в силу,\n необходимо перезагрузить проект.\n Перезагрузить?")!=65536)
    configure();
}
void MainWindow::on_VKDirectButton_clicked()
{
    QUrl vk("https://vk.com/im?sel=391846880");
    QDesktopServices::openUrl(vk);
}
void MainWindow::ShowContextMenu(QPoint position)
{
    if(project==nullptr) return;
    now_pos = position;
    QPoint globalPos = ui->ProjectTree->mapToGlobal(position);
       QMenu myMenu;
       myMenu.setStyleSheet(
R"(QMenu {
       background-color: #414243;
       border: 1px solid #232425;
   }

   QMenu::item {
       background-color: transparent;
       color:#c1c1c1;
       padding: 4px 8px;
   }

   QMenu::item:selected {
       background-color: #6b6c6d;
   })");
       QMenu subMenu(&myMenu);
       subMenu.setTitle("Добавить");
       subMenu.setStyleSheet(
R"(QMenu {
       background-color: #414243;
       border: 1px solid #232425;
   }

   QMenu::item {
       background-color: transparent;
       color:#c1c1c1;
       padding: 4px 8px;
   }

   QMenu::item:selected {
       background-color: #6b6c6d;
   })");
       subMenu.addAction("Создать папку", this, SLOT(AddFolder()));
       subMenu.addAction("Создать файл FORTH", this, SLOT(AddFile()));
       myMenu.addMenu(&subMenu);
       myMenu.addSeparator();
       myMenu.addAction("Изменить имя файла", this, SLOT(changeName()));
       myMenu.addAction("Удалить файл",  this, SLOT(eraseItem()));
       myMenu.exec(globalPos);
}
void MainWindow::AddFile()
{
    QString _Val = ui->ProjectTree->indexAt(now_pos).data().toString(), _Val2;
    qDebug() << _Val;
    if(_Val=="" || _Val.contains('.'))
    {
        ui->Status->showMessage("Вы должны выбрать папку,\n в которой будете создавать файл!",10000);
        return;
    }
    QString filename;
    ChangeNameWidget CNW(&filename);
    CNW.show();
    CNW.exec();
    filename+=".forth";
    auto Val = project->GFPaN();
    for(size_t i=0;i<Val.size();i++)
    {
        if(Val[i].second==filename)
        {
            ui->Status->showMessage("Произошла ошибка. Данный файл существует!",10000);
            return;
        }
    }
    QFile file(project->ProjectFolder + _Val + "/" + filename);
    file.open(QIODevice::WriteOnly);
    file.close();
    project->AddFile(project->ProjectFolder + _Val + "/" + filename);
    LoadProjectTree();
    ui->Status->showMessage("Готово!",5000);
}
void MainWindow::AddFolder()
{
    QString folder;
    ChangeNameWidget CNW(&folder);
    CNW.show();
    CNW.exec();
    if(folder=="")
    {
        ui->Status->showMessage("Произошла ошибка. Строка пуста!",10000);
        return;
    }
    if(QDir(project->ProjectFolder+folder).exists())
    {
        ui->Status->showMessage("Произошла ошибка. Данная папка существует!",10000);
        return;
    }
    QDir().mkdir(project->ProjectFolder+folder);
    LoadProjectTree();
    ui->Status->showMessage("Готово!",5000);
}
void MainWindow::changeName()
{

}
void MainWindow::eraseItem()
{
    QString _Val = ui->ProjectTree->indexAt(now_pos).data().toString(), _Val2;
    qDebug() << _Val;
    if(_Val=="" || !_Val.contains('.'))
    {
        ui->Status->showMessage("Удалить можно только лишь файл!",10000);
        return;
    }
    auto Val = project->GFPaN();
    for(size_t i=0;i<Val.size();i++)
    {
        if(Val[i].second==_Val)
        {
            if(QMessageBox::question(nullptr,"Вы уверены?", "Вы уверены, что хотите удалить файл " + Val[i].second)==65536)
                return;
            for(int y=0;y<ui->Files->count();y++)
                if(ui->Files->tabText(y)==Val[i].second)
                {
                    ui->Files->removeTab(y);
                    break;
                }
            project->RemoveFile(Val[i].first);
            ui->Status->showMessage("Готово!",5000);
            LoadProjectTree();
            return;
        }
    }
    ui->Status->showMessage("Произошла какая-то ошибка. Файл удалить не удалось.",10000);
}
