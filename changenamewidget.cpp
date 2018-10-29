#include "changenamewidget.h"
#include "ui_changenamewidget.h"

ChangeNameWidget::ChangeNameWidget(QString* ptr, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChangeNameWidget)
{
    this->ptr = ptr;
            ui->setupUi(this);
}

ChangeNameWidget::~ChangeNameWidget()
{
    delete ui;
}

void ChangeNameWidget::on_buttonBox_accepted()
{
    (*ptr) = ui->Name->text();
}
