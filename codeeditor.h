#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QWidget>
#include <QPlainTextEdit>
#include <QPainter>
#include <QTextBlock>
#include <QSettings>

class CodeEditor : public QPlainTextEdit
{
    Q_OBJECT

public:
    CodeEditor(QWidget *parent = nullptr);

    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();

protected:
    void resizeEvent(QResizeEvent *event) override;

private slots:
    void updateLineNumberAreaWidth(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &rect, int dy);

private:
    QWidget *lineNumberArea;
    QSettings settings;
};

#endif // CODEEDITOR_H
