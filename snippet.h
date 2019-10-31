#ifndef SNIPPET_H
#define SNIPPET_H

#include <QListWidgetItem>
#include <QString>

class Snippet : QListWidgetItem
{
	public:
		Snippet(int);
		
		void setId(int);
		int getId();
		
		void setTitle(QString);
		QString getTitle();
		
		void setContent(QString);
		QString getContent();
		
		void setText(QString);
	private:
		int id;
		QString title;
		QString content;
};

#endif
