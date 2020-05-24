#ifndef SYNTAXHIGHLIGHTER_H
#define SYNTAXHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QPlainTextEdit>
#include <QTextDocument>
#include <QString>
#include <QTextCharFormat>
#include <QRegularExpressionMatchIterator>

class SyntaxHighlighter : QSyntaxHighlighter
{
public:
    SyntaxHighlighter(QTextDocument *);
    void highlightBlock(const QString &text);
};

#endif // SYNTAXHIGHLIGHTER_H
