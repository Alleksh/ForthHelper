#include "tab.h"
#include <QApplication>
QAbstractItemModel *modelFromFile(const QString& fileName, QCompleter *completer)
{
    QFile file(fileName);
       if (!file.open(QFile::ReadOnly))
           return new QStringListModel(completer);

   #ifndef QT_NO_CURSOR
       QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
   #endif
       QStringList words;

       while (!file.atEnd())
       {
           QByteArray line = file.readLine();
           if (!line.isEmpty())
               words << line.trimmed();
       }

   #ifndef QT_NO_CURSOR
       QApplication::restoreOverrideCursor();
   #endif
       return new QStringListModel(words, completer);
}
CodeEditor::CodeEditor(QTextBrowser* ProblemsBar, Project* project, QString pathToFile,QString filename, QWidget *parent) : QPlainTextEdit(parent)
{
    qDebug() << "Creating tab. pathToFile: " + pathToFile << "Filename: "  + filename;
    lineNumberArea = new LineNumberArea(this);
    compiler = new Compiler(project);
    connect(this, SIGNAL(textChanged()), this, SLOT(UpdateText()));
    connect(this, SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this, SIGNAL(updateRequest(QRect,int)), this, SLOT(updateLineNumberArea(QRect,int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));
    this->setStyleSheet(R"(
                        QScrollBar:horizontal {
                            color:#AFAFAF;
                        background-color:#2C2E2F;
                        }

                        QScrollBar::handle:horizontal {

                         border: 2px groove #6b6c6d;
                        border-radius:4px;
                        background-color:#2C2E2F;
                            color:#AFAFAF;
                            min-width: 40px;
                        }

                        QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal {
                            color:#c1c1c1;
                        background-color:#2C2E2F;
                        }

                        QScrollBar:vertical {
                            color:#AFAFAF;
                        background-color:#2C2E2F;
                         }
                         QScrollBar::handle:vertical {
                         border: 2px groove #6b6c6d;
                        border-radius:4px;
                        background-color:#2C2E2F;
                            color:#AFAFAF;
                             min-height: 80px;
                         }
                         QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {
                            background: #2C2E2F;
                         }

                        QPlainTextEdit{
                        border-style: solid;
                        border-width: 2px;
                        border-color: #27292a;
                        color:#FAFAFA;
                        }
                        )");
    this->project = project;
    this->pathToFile = pathToFile;
    this->filename = filename;
    this->QTbrowser = ProblemsBar;

    QFile File(filename + ".cwl");
    File.open(QIODevice::WriteOnly);
    File.close();

    QFile file(pathToFile);
    file.open(QIODevice::ReadOnly);
        QTextStream qts(&file);
        QString Text = qts.readAll();
        this->setPlainText(Text);
    file.close();
    compiler->UpdateCompiler(this->toPlainText());

    this->setLineWrapMode(LineWrapMode::NoWrap);
    QFont font("Consolas",16);
    this->setFont(font);

    updateLineNumberAreaWidth(0);
    highlightCurrentLine();

    this->QTbrowser->setText(compiler->GetErrorQString());

    QCompleter* completer = new QCompleter();
    completer->setModel(modelFromFile("words.cwl", completer));
    completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setWrapAround(false);
    setCompleter(completer);
    now_highlighter = new Highlighter(this->document(),compiler->getWords());
}
CodeEditor::~CodeEditor()
{
    QFile file(pathToFile);
    file.open(QIODevice::ReadOnly);
    QTextStream qts(&file);
    if(this->toPlainText()!=qts.readAll())
    if(QMessageBox::question(nullptr,"", "Файл " + this->filename + " не сохранён. Сохранить?")!=65536)
    save();
    file.close();

    delete c;
    delete lineNumberArea;
    delete now_highlighter;
    delete compiler;
}
//Syntax highlight functions
Highlighter::Highlighter(QTextDocument *parent, std::vector<QStringList> vector)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;
    errors.setFontUnderline(1);
    errors.setForeground(QBrush(QColor("#d3d3d3")));
    errors.setUnderlineColor(Qt::red);
    rule.pattern = QRegularExpression("\\w+");
    rule.format = errors;
    highlightingRules.append(rule);

    integers.setForeground(QBrush(QColor("#ADD8E6")));
    rule.pattern = QRegularExpression("\\d+");
    rule.format = integers;
    highlightingRules.append(rule);

    //Common words
        keywords.setForeground(QBrush(QColor("#fafafa")));
        keywords.setFontWeight(QFont::Bold);
        QStringList keywordPatterns;
        keywordPatterns << vector[0];
        foreach (const QString &pattern, keywordPatterns)
        {
            QString x;
            for(int i=0;i<pattern.size();i++)
            {
                if(     pattern[i]=='.'||   pattern[i]=='*' ||
                        pattern[i]=='^' ||  pattern[i]=='-' ||
                        pattern[i]=='\\' ||
                        pattern[i]=='$' ||  pattern[i]=='?')
                        x+= "\\";
             x+= pattern[i];
            }
            x="\\b((\\s?)" + x + "(\\s?))\\b";
            rule.pattern = QRegularExpression(x);
            rule.format = keywords;
            highlightingRules.append(rule);
        }
    //Word-creators( VARIABLE, CONSTANT, : )
    keywordPatterns.clear();
    keywordPatterns << vector[1] << ":";
    keywords.setForeground(QBrush(QColor("#c3a6f0")));
    foreach (const QString &pattern, keywordPatterns)
    {
        QString x = pattern + " (\\S)*";
        rule.pattern = QRegularExpression(x);
        rule.format = keywords;
        highlightingRules.append(rule);
    }
    keywords.setForeground(QBrush(QColor("#B27300")));
    foreach (const QString &pattern, keywordPatterns)
    {

        rule.pattern = QRegularExpression(pattern);
        rule.format = keywords;
        highlightingRules.append(rule);
    }
//if/then, do/loop, begin/again...
    keywordPatterns.clear();
    keywordPatterns << vector[2] << "IF\nTHEN";
    keywords.setForeground(QBrush(QColor("#c4c52d")));
    foreach (const QString &pattern, keywordPatterns)
    {
        QStringList Values;
        QString fWord;
        QStringList secondWords;
        int i=0;
            while(i<pattern.size()&&pattern[i]!='\n')
            {
                fWord+=pattern[i];
                i++;
            }
            while(i<pattern.size())
            {
                QString nWord;
                while(i<pattern.size() && pattern[i]!=' ')
                {
                    nWord+=pattern[i];
                    i++;
                }
                secondWords << nWord;
                i++;
            }
        for(int i=0;i<secondWords.size();i++)
        Values << "/(("+fWord+")(?:(?!"+fWord+"|"+secondWords[i]+")[\\w\\s]|(?1))*("+secondWords[i]+":))/gm";
        foreach (const QString &pattern, Values)
        {
            rule.pattern = QRegularExpression(pattern);
            rule.format = keywords;
            highlightingRules.append(rule);
        }
    }
      strings.setForeground(QBrush(QColor("#568e56")));
      rule.pattern = QRegularExpression("\\.\".*\"");
      rule.format = strings;
      highlightingRules.append(rule);

      comments.setForeground(QBrush(QColor("#7e7e7e")));
      rule.pattern = QRegularExpression("#.*");
      rule.format = comments;
      highlightingRules.append(rule);
}
void Highlighter::highlightBlock(const QString &text)
{
    foreach (const HighlightingRule &rule, highlightingRules)
    {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext())
        {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
}
//Line number area functions
int CodeEditor::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    int space = 10 + fontMetrics().horizontalAdvance(QLatin1Char('9')) * digits;

    return space;
}
void CodeEditor::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth()+20, 0, 0, 0);
}
void CodeEditor::updateLineNumberArea(const QRect &rect, int dy)
{
    if (dy)
        lineNumberArea->scroll(0, dy);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());

    if (rect.contains(viewport()->rect()))
        updateLineNumberAreaWidth(0);
}
void CodeEditor::resizeEvent(QResizeEvent *e)
{
    QPlainTextEdit::resizeEvent(e);

    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}
void CodeEditor::highlightCurrentLine()
{
    QList<QTextEdit::ExtraSelection> extraSelections;

    if (!isReadOnly())
    {
        QTextEdit::ExtraSelection selection;

        QColor lineColor = QColor("#565758");

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}
void CodeEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    QPainter painter(lineNumberArea);
    QBrush brush("#2C2E2F");
    QColor otherColor("#c1c1c1");
    QFont font("Consolas",16);
    this->setFont(font);
    painter.setFont(font);
    painter.fillRect(event->rect(),brush);
    QTextBlock block = firstVisibleBlock();
       int blockNumber = block.blockNumber();
       int top = int (blockBoundingGeometry(block).translated(contentOffset()).top());
       int bottom = top + int(blockBoundingRect(block).height());
       while (block.isValid() && top <= event->rect().bottom())
       {
              if (block.isVisible() && bottom >= event->rect().top())
              {
                  QString number = QString::number(blockNumber + 1);
                  painter.setPen(otherColor);
                  painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                                   Qt::AlignRight, number);

              }

              block = block.next();
              top = bottom;
              bottom = top + int(blockBoundingRect(block).height());
              ++blockNumber;
          }
      }
//Completer functions
void CodeEditor::setCompleter(QCompleter *completer)
{
    qDebug() << "setCompleter Called";
    if (c)
        QObject::disconnect(c, nullptr, this, nullptr);

    c = completer;
    if (!c)
        return;
    c->setWidget(this);
    c->popup()->setFocusPolicy(Qt::ClickFocus);
    c->popup()->setStyleSheet(
   R"(
       QScrollBar:horizontal {
           color:#AFAFAF;
       background-color:#2C2E2F;
       }
       QScrollBar::handle:horizontal {
        border: 2px groove #6b6c6d;
       border-radius:4px;
       background-color:#2C2E2F;
           color:#AFAFAF;
           min-width: 30px;
       }
       QScrollBar::add-page:horizontal, QScrollBar::sub-page:horizontal {
           color:#c1c1c1;
       background-color:#2C2E2F;
       }
       QScrollBar:vertical {
           color:#AFAFAF;
       background-color:#2C2E2F;
        }
        QScrollBar::handle:vertical {
        border: 2px groove #6b6c6d;
       border-radius:4px;
       background-color:#2C2E2F;
           color:#AFAFAF;
            min-height: 30px;
        }
        QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {
           background: #2C2E2F;
        }


    QListView
    {
        border-style: solid;
        border-width: 2px;
        border-color: #27292a;
        background-color:#414243;
    color:#c1c1c1;
    }
    QListView::item {
        background-color:#4a4d4f;
        border: 2px solid #4a4d4f;
        min-width: 12px;
        min-height: 12px;
        padding: 4px;
        padding-top: 2px;
        padding-bottom:02px;
        color:#c1c1c1;
    }

    QListView::item:selected {
        border-color: #007ACC;
        color:#FAFAFA;
        background:#007ACC;
    }
    QListView::item:hover{
        background-color: #6b6c6d;
        border:#6b6c6d;
        color:#c1c1c1;
    }
   )");
    c->setCompletionMode(QCompleter::PopupCompletion);
    c->setCaseSensitivity(Qt::CaseInsensitive);
    QObject::connect(c, SIGNAL(activated(QString)),
                     this, SLOT(insertCompletion(QString)));
}
QCompleter *CodeEditor::completer() const
{
    return c;
}
void CodeEditor::insertCompletion(const QString& completion)
{
    if (c->widget() != this)
        return;
    QTextCursor tc = textCursor();
    int extra = completion.length() - c->completionPrefix().length();
    tc.movePosition(QTextCursor::Left);
    tc.movePosition(QTextCursor::EndOfWord);
    tc.insertText(completion.right(extra));
    setTextCursor(tc);
}
QString CodeEditor::textUnderCursor() const
{
    QTextCursor tc = textCursor();
    tc.select(QTextCursor::WordUnderCursor);
    return tc.selectedText();
}
void CodeEditor::focusInEvent(QFocusEvent *e)
{
    if (c)
        c->setWidget(this);
    QPlainTextEdit::focusInEvent(e);
}
void CodeEditor::keyPressEvent(QKeyEvent *e)
{
    if (c && c->popup()->isVisible())
    {
       switch (e->key())
       {
       case Qt::Key_Enter:
       case Qt::Key_Return:
       case Qt::Key_Escape:
       case Qt::Key_Tab:
       case Qt::Key_Backtab:
            e->ignore();
            return;
       default:
           break;
       }
    }
    if((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_S)
    {
        save();
        QPlainTextEdit::keyPressEvent(e);
    }
    bool isShortcut = ((e->modifiers() & Qt::ControlModifier) && e->key() == Qt::Key_E); // CTRL+E
    if (!c || !isShortcut)
        QPlainTextEdit::keyPressEvent(e);
    const bool ctrlOrShift = e->modifiers() & (Qt::ControlModifier | Qt::ShiftModifier);
        if (!c || (ctrlOrShift && e->text().isEmpty()))
            return;

        static QString eow("~!@#$%^&*()_+{}|:\"<>?,./;'[]\\-=");
        bool hasModifier = (e->modifiers() != Qt::NoModifier) && !ctrlOrShift;
        QString completionPrefix = textUnderCursor();

        if (!isShortcut && (hasModifier || e->text().isEmpty()|| completionPrefix.length() < 3
                          || eow.contains(e->text().right(1))))
        {
            c->popup()->hide();
            return;
        }
        if (completionPrefix != c->completionPrefix())
        {
            c->setCompletionPrefix(completionPrefix);
            c->popup()->setCurrentIndex(c->completionModel()->index(0, 0));
        }
        QRect cr = cursorRect();
        cr.setWidth(c->popup()->sizeHintForColumn(0)
                    + c->popup()->verticalScrollBar()->sizeHint().width() + 10);
        c->complete(cr);
}
void CodeEditor::save()
{
    saveBackup();
    QFile file(pathToFile);
    file.open(QIODevice::WriteOnly);
    QTextStream qts(&file);
    qts << this->toPlainText();
    file.close();
}
void CodeEditor::saveBackup()
{
    auto now = std::chrono::system_clock::now();
    auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
    auto epoch = now_ms.time_since_epoch();
    auto value = std::chrono::duration_cast<std::chrono::milliseconds>(epoch);
    long long	value1 = value.count(); value1 /= 1000;
    uint16_t	second = value1 % 60; value1 /= 60;
    uint16_t	minute = value1 % 60; value1 /= 60;
    uint16_t	hour = (3 + (value1 % 24)) % 24; value1 /= 24;
    uint16_t	year = 1970 + uint16_t(value1 / 365);
    uint16_t	month = 1;
    uint16_t	day = (value1 - ((value1 / 365 + 2) / 4)) % (365 + (year % 4 ? 0 : 1));;
    for (; month <= 12 && day > 30 + ((month != 2) ? month % 2 : -2 + ((year % 4 == 0) ? 1 : 0));
        month++, day -= 30 + ((month != 2) ? month % 2 : -2 + ((year % 4 == 0) ? 1 : 0)));

    QString Value = "[" + QString::number(year) + "." + ((month / 10 < 1) ? "0" : "") + QString::number(month) + "." + ((day / 10 < 1) ? "0" : "") + QString::number(day) + "]"
        " [" + QString::number(hour) + "-" + QString::number(minute) + "-" + QString::number(second) + "]" + ".bak";

    if(!QDir(project->BackupFolder+"/" + filename).exists())
        QDir().mkdir(project->BackupFolder+"/" + filename);

    QFile file(project->BackupFolder + filename  + "/" + Value);
    QFile file2(pathToFile);
    file.open(QIODevice::WriteOnly);
    file2.open(QIODevice::ReadOnly);
    QTextStream qts1(&file), qts2(&file2);
    qts1 << qts2.readAll();
    file.close();
    file2.close();
}
void CodeEditor::UpdateText()
{
    CI++;
    if(CI>=100)
    {
        saveBackup();
        CI=0;
    }
    else if(CI%5==0)
    {
        if(CI%4==0)
        {
            delete now_highlighter;
            now_highlighter = new Highlighter(this->document(),compiler->getWords());
            compiler->UpdateCompiler(this->toPlainText());
        }
        if(CI%3==0)
            UpdateCompleter();
    }
}
void CodeEditor::UpdateCompleter()
{
    if(!compiler->UpdateCompleter("words.cwl")) return;
    QCompleter* completer = new QCompleter(this);
    completer->setModel(modelFromFile("words.cwl", completer));
    completer->setModelSorting(QCompleter::CaseInsensitivelySortedModel);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setWrapAround(false);
    setCompleter(completer);
}
