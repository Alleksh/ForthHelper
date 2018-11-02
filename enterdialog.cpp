#include "enterdialog.h"
#include "ui_enterdialog.h"

EnterDialog::EnterDialog(Project *proj,QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EnterDialog)
{
    ui->setupUi(this);
    this->proj = proj;
    auto Val = proj->GFPaN();
    for(size_t i=0;i<Val.size();i++)
    {
        ui->leftList->addItem(Val[i].second);
    }
    LeftList = Val;
    ui->leftList->setFocusPolicy(Qt::NoFocus);
    ui->rightList->setFocusPolicy(Qt::NoFocus);
}
EnterDialog::~EnterDialog()
{
    delete ui;
}
void EnterDialog::on_leftList_itemClicked(QListWidgetItem *item)
{
    auto Data = item->text();
    std::pair<QString,QString>* pair = nullptr;
    for(size_t i=0;i<LeftList.size();i++)
        if(LeftList[i].second==Data)
        {
            pair = &LeftList[i];
        }
    if(pair==nullptr) return;
    for(size_t i=0;i<RightList.size();i++)
        if(RightList[i].second==Data) return;
    RightList.push_back((*pair));
    ui->rightList->addItem((*pair).second);
}
void EnterDialog::on_rightList_itemClicked(QListWidgetItem *item)
{
    auto Data = item->text();
    for(size_t i=0;i<RightList.size();i++)
        if(RightList[i].second==Data)
        {
            RightList.erase(RightList.begin()+int(i));
            break;
        }
    ui->rightList->clear();
    for(size_t i=0;i<RightList.size();i++)
        ui->rightList->addItem(RightList[i].second);
}
void EnterDialog::on_start_clicked()
{

}
void EnterDialog::on_cancel_clicked()
{
    this->close();
}
