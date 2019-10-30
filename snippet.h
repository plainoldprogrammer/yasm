#ifndef SNIPPET_H
#define SNIPPET_H

#include <QString>

class Snippet
{
	public:
		Snippet();
		
		void setTitle(QString);
		QString getTitle();
		
		void setContent(QString);
		QString getContent();
	private:
		QString title;
		QString content;
};

#endif
