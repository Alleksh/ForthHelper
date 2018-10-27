#ifndef EDITWORDLISTDIALOG_H
#define EDITWORDLISTDIALOG_H

#include <QDialog>

namespace Ui {
class EditWordListDialog;
}

class EditWordListDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EditWordListDialog(QWidget *parent = nullptr);
    ~EditWordListDialog();

private:
    Ui::EditWordListDialog *ui;
};

#endif // EDITWORDLISTDIALOG_H
