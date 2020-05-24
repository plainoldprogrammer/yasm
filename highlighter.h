#ifndef HIGHLIGHTER_H
#define HIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QPlainTextEdit>
#include <QTextDocument>
#include <QString>
#include <QTextCharFormat>
#include <QRegularExpressionMatchIterator>

class Highlighter : QSyntaxHighlighter
{
public:
    Highlighter(QTextDocument *);
    void highlightBlock(const QString &text);
};

#endif // HIGHLIGHTER_H
