#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDirModel>
#include <QStandardPaths>
#include <QFileDialog>
#include <QDebug>
#include <QFileSystemModel>
#include <QIcon>
#include <QStringListModel>
#include <QTextEdit>
#include "tab.h"
#include <QDesktopServices>
#include <QMessageBox>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    Project* project = nullptr;
    std::vector<CodeEditor*> Tabs;
    void configure();
    QPoint now_pos;
    void LoadProjectTree();
private slots:
    void AddFile();

    void AddFolder();

    void changeName();

    void eraseItem();

    void ShowContextMenu(QPoint);

    void on_CreateProject_triggered();

    void on_OpenProject_triggered();

    void on_SaveAndExit_triggered();

    void on_ProjectTree_doubleClicked(const QModelIndex &index);

    void on_Files_tabCloseRequested(int index);

    void on_CompilerOutput_tabCloseRequested(int index);

    void on_EditCurrentWordList_triggered();

    void on_VKDirectButton_clicked();

    void on_Emulate_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
