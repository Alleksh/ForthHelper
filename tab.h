#ifndef TAB_H
#define TAB_H

#include <QWidget>
#include <QTextEdit>
#include <QPlainTextEdit>
#include <QCompleter>
#include <QPainter>
#include <QTextBlock>
#include <QStringListModel>
#include <QAbstractItemView>
#include <QScrollBar>
#include <QDateTime>
#include <QTextBrowser>
#include "compiler.h"
#include<QMessageBox>
#include <QSyntaxHighlighter>
class Highlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    Highlighter(QTextDocument *parent,QStringList,QStringList);

protected:
    void highlightBlock(const QString &text) override;

private:
    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };
    QVector<HighlightingRule> highlightingRules;

    QTextCharFormat keywordFormat;
    QTextCharFormat classFormat;
    QTextCharFormat LineCommentFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat functionFormat;
};
class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT
public:
    CodeEditor(QTextBrowser*,Project*, QString,QString, QWidget *parent = nullptr);
    ~CodeEditor() override;
    void lineNumberAreaPaintEvent(QPaintEvent*);
    int lineNumberAreaWidth();
    void setCompleter(QCompleter *c);
    QCompleter *completer() const;
    Project* project;
    QString pathToFile, filename;
    void save();
    void saveBackup();
    void UpdateCompleter();
protected:
    void resizeEvent(QResizeEvent *event) override;
    void keyPressEvent(QKeyEvent *e) override;
    void focusInEvent(QFocusEvent *e) override;
private slots:
    void UpdateText();
    void updateLineNumberAreaWidth(int);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect&,int);
    void insertCompletion(const QString&);

private:
    Compiler* compiler;
    QString textUnderCursor() const;
    QCompleter *c = nullptr;
    QWidget *lineNumberArea;
    QTextBrowser *QTbrowser;
    int CI = 0;
    Highlighter* now_highlighter;
};

class LineNumberArea : public QWidget
{
public:
    LineNumberArea(CodeEditor *editor) : QWidget(editor) {
        codeEditor = editor;
    }

    QSize sizeHint() const override {
        return QSize(codeEditor->lineNumberAreaWidth(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        codeEditor->lineNumberAreaPaintEvent(event);
    }

private:
    CodeEditor *codeEditor;
};
#endif // TAB_H
