#ifndef SNIPPET_H
#define SNIPPET_H

#include <QListWidgetItem>
#include <QString>

class Snippet : QListWidgetItem
{
	public:
		Snippet();
		
		void setTitle(QString);
		QString getTitle();
		
		void setContent(QString);
		QString getContent();
		void setText(QString);
	private:
		QString title;
		QString content;
};

#endif
