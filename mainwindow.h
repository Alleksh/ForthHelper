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
private slots:
    void on_CreateProject_triggered();

    void on_OpenProject_triggered();

    void on_SaveAndExit_triggered();

    void on_ProjectTree_doubleClicked(const QModelIndex &index);
    void on_Files_tabCloseRequested(int index);

    void on_CompilerOutput_tabCloseRequested(int index);

    void on_EditCurrentWordList_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
