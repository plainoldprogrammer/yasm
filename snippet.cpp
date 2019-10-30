#include "snippet.h"
#include <QDebug>

Snippet::Snippet() : title(""), content("")
{
	qDebug() << "New snippet created";
}

void Snippet::setTitle(QString title)
{
	this->title = title;
}

QString Snippet::getTitle()
{
	return this->title;
}

void Snippet::setContent(QString content)
{
	this->content = content;
}

QString Snippet::getContent()
{
	return this->content;
}

void Snippet::setText(QString text)
{
	QListWidgetItem::setText(text);
}
