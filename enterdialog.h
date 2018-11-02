#ifndef ENTERDIALOG_H
#define ENTERDIALOG_H

#include <QDialog>
#include <QListWidgetItem>
#include "project.h"
namespace Ui {
class EnterDialog;
}

class EnterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EnterDialog(Project* proj, QWidget *parent = nullptr);
    ~EnterDialog();

private slots:
    void on_leftList_itemClicked(QListWidgetItem *item);

    void on_rightList_itemClicked(QListWidgetItem *item);

    void on_start_clicked();

    void on_cancel_clicked();

private:
    Ui::EnterDialog *ui;
    Project* proj;
    std::vector<std::pair<QString,QString>> LeftList, RightList;
};

#endif // ENTERDIALOG_H
