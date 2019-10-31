#include "snippet.h"
#include <QDebug>

Snippet::Snippet(int id) : title("Snippet title"), content("Snippet content")
{
	this->setId(id);
	qDebug() << "New snippet created [id=" << getId() << "]";
}

void Snippet::setId(int id)
{
	this->id = id;
}

int Snippet::getId()
{
	return this->id;
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
