#ifndef CHANGENAMEWIDGET_H
#define CHANGENAMEWIDGET_H

#include <QDialog>

namespace Ui {
class ChangeNameWidget;
}

class ChangeNameWidget : public QDialog
{
    Q_OBJECT

public:
    explicit ChangeNameWidget(QString* ptr, QWidget *parent = nullptr);
    ~ChangeNameWidget();

private slots:
    void on_buttonBox_accepted();

private:
    QString* ptr;
    Ui::ChangeNameWidget *ui;
};

#endif // CHANGENAMEWIDGET_H
