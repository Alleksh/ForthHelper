#include "editwordlistdialog.h"
#include "ui_editwordlistdialog.h"

EditWordListDialog::EditWordListDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditWordListDialog)
{
    ui->setupUi(this);
}

EditWordListDialog::~EditWordListDialog()
{
    delete ui;
}
