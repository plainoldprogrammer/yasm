#include "highlighter.h"

Highlighter::Highlighter(QTextDocument * document) : QSyntaxHighlighter(document)
{

}

void Highlighter::highlightBlock(const QString &text)
{
    QTextCharFormat myClassFormat;
    myClassFormat.setFontWeight(QFont::Bold);
    myClassFormat.setForeground(Qt::blue);

    QRegularExpression expression("");
    QRegularExpressionMatchIterator i = expression.globalMatch(text);
    while (i.hasNext())
    {
      QRegularExpressionMatch match = i.next();
      setFormat(match.capturedStart(), match.capturedLength(), myClassFormat);
    }
}
