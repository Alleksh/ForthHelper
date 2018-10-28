#ifndef EDITWORDLISTDIALOG_H
#define EDITWORDLISTDIALOG_H

#include <QDialog>
#include <compiler.h>
namespace Ui {
class EditWordListDialog;
}
#include <QListWidgetItem>
class EditWordListDialog : public QDialog
{
    Q_OBJECT

public:
    std::vector<Word> WL;
    explicit EditWordListDialog(QWidget *parent = nullptr);
    ~EditWordListDialog();
    void Load(size_t id);
    QPoint now_point;
    size_t NOW_ID = -1;
private slots:
    void addItem();
    void eraseItem();
    void on_Save_clicked();

    void ShowContextMenu(const QPoint &p);

    void on_list_itemClicked(QListWidgetItem *item);

    void on_Reload_clicked();

    void on_desciption_textChanged();

    void on_isVariable_clicked();

    void on_isFunction_clicked();

    void on_StartWord_editingFinished();

    void on_EndWords_editingFinished();

    void on_NeedIntegers_editingFinished();

    void on_GiveIntegers_editingFinished();

    void on_one_word_stateChanged(int arg1);

    void on_SaEWiOL_stateChanged(int arg1);

    void on_CreateNewWord_stateChanged(int arg1);

private:
    Ui::EditWordListDialog *ui;
};

#endif // EDITWORDLISTDIALOG_H
