#include "editwordlistdialog.h"
#include "ui_editwordlistdialog.h"

EditWordListDialog::EditWordListDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditWordListDialog)
{
    ui->setupUi(this);
    ui->GiveIntegers->setValidator( new QIntValidator(0, 100, this) );
    ui->NeedIntegers->setValidator( new QIntValidator(0, 100, this) );
    ui->StartWord->setValidator(new QRegExpValidator(QRegExp("^\\S+$")));
    setWindowTitle("Изменить словарь");
    on_Reload_clicked();
    ui->list->setFocusPolicy(Qt::NoFocus);
    ui->list->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui->list, SIGNAL(customContextMenuRequested(QPoint)),this,SLOT(ShowContextMenu(QPoint)));
}
void EditWordListDialog::on_Reload_clicked()
{
    ui->StartWord->setText("");
    ui->EndWords->setText("");
    ui->desciption->setText("");
    ui->NeedIntegers->setText("");
    ui->GiveIntegers->setText("");
    ui->SaEWiOL->setCheckState(Qt::Unchecked);
    ui->CreateNewWord->setCheckState(Qt::Unchecked);
    ui->one_word->setCheckState(Qt::Unchecked);
    ui->isFunction->setChecked(0);
    ui->isVariable->setChecked(0);
    WL.clear();
    ui->list->clear();
    QFile file("words.txt");
    file.open(QIODevice::ReadOnly);
    QTextStream qts(&file);
    while(!qts.atEnd())
    {
        Word word;
        if(qts.atEnd()) break;
        QString str = qts.readLine();
        word.StartWord = str;
        ui->list->addItem(str);
        if(qts.atEnd()) break;
        str = qts.readLine();
        word.EndWords = str;
        if(qts.atEnd()) break;
        str = qts.readLine();
        word.NeedNumbers = str.toInt();
        if(qts.atEnd()) break;
        str = qts.readLine();
        word.GiveNumbers = str.toInt();
        if(qts.atEnd()) break;
        str = qts.readLine();
        word.VF = str.toInt();
        if(qts.atEnd()) break;
        str = qts.readLine();
        word.one_word = str.toInt();
        if(qts.atEnd()) break;
        str = qts.readLine();
        word.StartAndEndWordInOneLine = str.toInt();
        if(qts.atEnd()) break;
        str = qts.readLine();
        word.CreatesNewWord = str.toInt();
        if(qts.atEnd()) break;
        std::string Str = qts.readLine().toStdString();
        for(size_t i=0;i<Str.size();i++)
        if(i!=0)
        if(Str[i-1]=='%')
        {
            if(i!=1)
            {
                if(Str[i-2]=='%')
                {
                    Str.erase((Str.begin() + int(i)) -1);
                    continue;
                }
            }
            if(Str[i]=='#')
            {
                Str[i]='\n';
                Str.erase((Str.begin() + int(i)) -1);
            }
            else if(Str[i]=='%')
                Str.erase((Str.begin() + int(i)) -1);
        }
        word.description = QString::fromStdString(Str);
        WL.push_back(word);
    }
    file.close();
    ui->list->sortItems(Qt::SortOrder::AscendingOrder);
    NOW_ID = 4294967295;//-1 in uint
}
void EditWordListDialog::addItem()
{
    for(size_t i=0;i<WL.size();i++)
        if(WL[i].StartWord=="New_word")
            return;
Word NewWord;
    NewWord.GiveNumbers = NewWord.NeedNumbers = NewWord.StartAndEndWordInOneLine =
    NewWord.CreatesNewWord = NewWord.VF = NewWord.one_word = 0;
    NewWord.EndWords = NewWord.description = "";
    NewWord.StartWord = "New_word";
    ui->list->addItem("New_word");
    WL.push_back(NewWord);
    ui->list->setCurrentItem(ui->list->item(int(WL.size())));
    Load(WL.size()-1);
}
void EditWordListDialog::eraseItem()
{
    if(ui->list->itemAt(now_point)==nullptr) return;
    QString text = ui->list->itemAt(now_point)->text();
    for(size_t i=0;i<WL.size();i++)
        if(WL[i].StartWord==text)
        {
            WL.erase(WL.begin() + int(i));
            break;
        }
    now_point.setX(0);
    now_point.setX(0);
    on_Reload_clicked();
}
void EditWordListDialog::ShowContextMenu(const QPoint& position)
{
    this->now_point = position;
    QPoint globalPos = ui->list->mapToGlobal(position);
       QMenu myMenu;
       myMenu.setStyleSheet(
R"(QMenu {
       background-color: #414243; /* sets background of the menu */
       border: 1px solid #232425;
   }

   QMenu::item {
       /* sets background of menu item. set this to something non-transparent
           if you want menu color and menu item color to be different */
       background-color: transparent;
       color:#c1c1c1;
       padding: 4px 8px;
   }

   QMenu::item:selected { /* when user selects item using mouse or keyboard */
       background-color: #6b6c6d;
   })");
       myMenu.addAction("Добавить новый элемент", this, SLOT(addItem()));
       myMenu.addAction("Удалить элемент",  this, SLOT(eraseItem()));
       myMenu.exec(globalPos);
}
EditWordListDialog::~EditWordListDialog()
{
    if(QMessageBox::question(nullptr,"", "Возможно Вы не сохранили изменения. Сохранить?")!=65536)
    on_Save_clicked();
    delete ui;
}
void EditWordListDialog::Load(size_t id)
{
    ui->EndWords->setReadOnly(0);
    ui->NeedIntegers->setReadOnly(0);
    ui->GiveIntegers->setReadOnly(0);
    ui->SaEWiOL->setEnabled(1);
    ui->one_word->setEnabled(1);
    ui->CreateNewWord->setEnabled(1);
    ui->isVariable->setEnabled(1);
    if(WL[id].StartAndEndWordInOneLine)
    {
        ui->SaEWiOL->setCheckState(Qt::Checked);
        on_SaEWiOL_stateChanged(2);
    }
    else
    {
        ui->SaEWiOL->setCheckState(Qt::Unchecked);
        on_SaEWiOL_stateChanged(0);
    }
    if(WL[id].CreatesNewWord)
    {
        ui->CreateNewWord->setCheckState(Qt::Checked);
        on_CreateNewWord_stateChanged(2);
    }
    else
    {
        ui->CreateNewWord->setCheckState(Qt::Unchecked);
        on_CreateNewWord_stateChanged(0);
    }
    if(WL[id].one_word)
    {
        ui->one_word->setCheckState(Qt::Checked);
        on_one_word_stateChanged(2);
    }
    else
    {
        ui->one_word->setCheckState(Qt::Unchecked);
        on_one_word_stateChanged(0);
    }
    if(WL[id].VF)
    {
        ui->isFunction->setChecked(1);
        on_isFunction_clicked();
    }
    else
    {
        ui->isVariable->setChecked(1);
        on_isVariable_clicked();
    }
    ui->StartWord->setText(WL[id].StartWord);
    ui->EndWords->setText(WL[id].EndWords);
    ui->desciption->setPlainText(WL[id].description);
    ui->NeedIntegers->setText(QString::number(WL[id].NeedNumbers));
    ui->GiveIntegers->setText(QString::number(WL[id].GiveNumbers));
}
void EditWordListDialog::on_list_itemClicked(QListWidgetItem *item)
{
    QString _Val = item->text();
    for(size_t i=0;i<WL.size();i++)
    {
        if(WL[i]==_Val)
        {
            NOW_ID = i;
            Load(i);
            return;
        }
    }
    return;
}
void EditWordListDialog::on_Save_clicked()
{
    QFile file("words.txt");
    file.open(QIODevice::WriteOnly);
    QTextStream qts(&file);
    for(size_t i=0;i<WL.size();i++)
    {
        qts << WL[i].StartWord + '\n';
        qts << WL[i].EndWords + "\n";
        qts << QString::number(WL[i].NeedNumbers) + "\n";
        qts << QString::number(WL[i].GiveNumbers)+"\n";
        qts << QString::number(WL[i].VF)+"\n";
        qts << QString::number(WL[i].one_word)+"\n";
        qts << QString::number(WL[i].StartAndEndWordInOneLine)+"\n";
        qts << QString::number(WL[i].CreatesNewWord)+"\n";
        QString Value = WL[i].description , Value2;
        for(int i=0;i<Value.size();i++)
        {
            if(Value[i]=='%')
                Value2+="%%";
            else if(Value[i]=='\n')
                Value2+="%#";
            else Value2+=Value[i];
        }
        qts << Value2 + "\n";
    }
    file.close();
}
void EditWordListDialog::on_isFunction_clicked()
{
    if(NOW_ID>WL.size()) return;
    if(!WL[NOW_ID].VF)
    {
        WL[NOW_ID].VF = 1;
        ui->EndWords->setReadOnly(0);
        ui->NeedIntegers->setReadOnly(0);
        ui->GiveIntegers->setReadOnly(0);
        ui->SaEWiOL->setEnabled(1);
        ui->one_word->setEnabled(1);
        ui->CreateNewWord->setEnabled(1);
    }
}
void EditWordListDialog::on_isVariable_clicked()
{
    if(NOW_ID>WL.size()) return;

    if(WL[NOW_ID].VF)
    {
        ui->EndWords->setText("");
        ui->EndWords->setReadOnly(1);
        ui->NeedIntegers->setText("0");
        ui->NeedIntegers->setReadOnly(1);
        ui->GiveIntegers->setText("1");
        ui->GiveIntegers->setReadOnly(1);
        ui->SaEWiOL->setCheckState(Qt::CheckState::Unchecked);
        ui->SaEWiOL->setEnabled(0);
        ui->one_word->setCheckState(Qt::CheckState::Unchecked);
        ui->one_word->setEnabled(0);
        ui->CreateNewWord->setCheckState(Qt::CheckState::Unchecked);
        ui->CreateNewWord->setEnabled(0);
        WL[NOW_ID].VF = 0;
    }
}
void EditWordListDialog::on_StartWord_editingFinished()
{
    if(NOW_ID>WL.size()) return;
    QString text = ui->StartWord->text();
    bool _Val = 0;
    for(size_t i=0;i<WL.size();i++)
    {
        if(WL[i]==text && i!=NOW_ID)
        {
            _Val = 1;
            break;
        }
    }
    if(!_Val) if(text=="")
    {
        QMessageBox::warning(nullptr,"Предупреждение!", "Данное название не подходит. Переименуйте.");
        ui->StartWord->setText(WL[NOW_ID].StartWord);
        return;
    }
    if(_Val)
    {
        QMessageBox::warning(nullptr,"Предупреждение!", "Данное слово уже существует. Переименуйте.");
        ui->StartWord->setText(WL[NOW_ID].StartWord);
        return;
    }
    WL[NOW_ID].StartWord = text;
    ui->list->currentItem()->setText(text);
}
void EditWordListDialog::on_EndWords_editingFinished()
{
    if(NOW_ID>WL.size()) return;
    WL[NOW_ID].EndWords = ui->EndWords->text();
}
void EditWordListDialog::on_NeedIntegers_editingFinished()
{
    if(NOW_ID>WL.size()) return;
    WL[NOW_ID].NeedNumbers = ui->NeedIntegers->text().toInt();
}
void EditWordListDialog::on_GiveIntegers_editingFinished()
{
    if(NOW_ID>WL.size()) return;
    WL[NOW_ID].GiveNumbers = ui->GiveIntegers->text().toInt();
}
void EditWordListDialog::on_desciption_textChanged()
{
    if(NOW_ID>WL.size()) return;
    WL[NOW_ID].description = ui->desciption->toPlainText();
}
void EditWordListDialog::on_one_word_stateChanged(int arg1)
{
    qDebug() << "on_one_word_stateChanged";
    if(NOW_ID>WL.size()) return;
    if(arg1==2) //true
    {
        ui->EndWords->setReadOnly(1);
        ui->EndWords->setText("");
        WL[NOW_ID].one_word = WL[NOW_ID].VF = 1;
        ui->isFunction->setChecked(1);
        ui->isVariable->setEnabled(0);
    }
    else
    {
        ui->isVariable->setEnabled(1);
        ui->EndWords->setReadOnly(0);
        WL[NOW_ID].one_word = 0;
    }
}
void EditWordListDialog::on_SaEWiOL_stateChanged(int arg1)
{
    qDebug() << "on_SaEWiOL_stateChanged";
    if(NOW_ID>WL.size()) return;
    if(arg1==2)//true
    {
        ui->one_word->setCheckState(Qt::CheckState::Unchecked);
        WL[NOW_ID].VF = WL[NOW_ID].StartAndEndWordInOneLine = 1;
        ui->isFunction->setChecked(1);
        ui->isVariable->setEnabled(0);
        ui->one_word->setEnabled(0);
    }
    else
    {
        WL[NOW_ID].StartAndEndWordInOneLine = 0;
        ui->isVariable->setEnabled(1);
        ui->one_word->setEnabled(1);
    }
}
void EditWordListDialog::on_CreateNewWord_stateChanged(int arg1)
{
    qDebug() << "on_CreateNewWord_stateChanged";
    if(NOW_ID>WL.size()) return;
    if(arg1==2)//true
    {
        ui->SaEWiOL->setCheckState(Qt::CheckState::Checked);
        ui->EndWords->setReadOnly(1);
        ui->EndWords->setText("*");
        WL[NOW_ID].CreatesNewWord = 1;
        ui->SaEWiOL->setEnabled(0);
        ui->isVariable->setEnabled(0);
    }
    else
    {
        ui->EndWords->setReadOnly(0);
        ui->EndWords->setText("");
        WL[NOW_ID].CreatesNewWord = 0;
        ui->SaEWiOL->setEnabled(1);
    }
}
