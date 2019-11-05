#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "snippet.h"
#include <QDebug>
#include <QListWidgetItem>
#include <QSqlDatabase>
#include <QDir>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent)
, ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	createDBConnection();
	snippetId = getMaxIdFromDb();
	firstTimeInitializeGUI();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::firstTimeInitializeGUI()
{
	ui->pushButtonNewSnippet->setText("New Snippet");
	ui->pushButtonRemoveSnippet->setText("Remove Snippet");
	this->setWindowTitle("YASM0 v0.4 (Alpha)");

	ui->pushButtonRemoveSnippet->setEnabled(false);	
	ui->listWidgetSnippets->setEnabled(false);
	ui->lineEditSnippetTitle->setEnabled(false);
	ui->textEditSnippetContent->setEnabled(false);
	
	QSqlQuery sqlQuery;
	
	if (sqlQuery.exec("SELECT * FROM 'snippets';"))
	{
		bool isAnySnippetOnDb = false;
		
		qDebug() << "Reading all snippets from the db";
		QVector<Snippet> snippetsFromDb;	
		Snippet *snippetRecoveredFromDb;
		
		while (sqlQuery.next())
		{
			qDebug() << "Snippet id=[" << sqlQuery.value(0).toInt() << "] title=[" << sqlQuery.value(1).toString() << "]";
			
			int snippetFromDbId = sqlQuery.value(0).toInt();
			QString snippetFromDbTitle = sqlQuery.value(1).toString();
			QString snippetFromDbContent = sqlQuery.value(2).toString();
			isAnySnippetOnDb = true;
			
			snippetRecoveredFromDb = new Snippet(snippetFromDbId);
			snippetRecoveredFromDb->setTitle(snippetFromDbTitle);
			snippetRecoveredFromDb->setText(snippetRecoveredFromDb->getTitle());
			snippetRecoveredFromDb->setContent(snippetFromDbContent);
			
			ui->listWidgetSnippets->insertItem(ui->listWidgetSnippets->count(), (QListWidgetItem *) snippetRecoveredFromDb);
			ui->listWidgetSnippets->setItemSelected((QListWidgetItem *) snippetRecoveredFromDb, true);
			ui->listWidgetSnippets->setCurrentRow(ui->listWidgetSnippets->count() - 1);
			ui->lineEditSnippetTitle->setText(snippetRecoveredFromDb->getTitle());
			ui->textEditSnippetContent->setText(snippetRecoveredFromDb->getContent());
		}
		
		if (isAnySnippetOnDb)
		{
			enableGUI();
		}
	}
	else
	{
		qWarning() << "Can't read the snippets from the db";
	}
}

void MainWindow::enableGUI()
{
	ui->pushButtonRemoveSnippet->setEnabled(true);
	ui->listWidgetSnippets->setEnabled(true);
	ui->lineEditSnippetTitle->setEnabled(true);
	ui->textEditSnippetContent->setEnabled(true);
}

void MainWindow::disableGUI()
{
	ui->pushButtonRemoveSnippet->setEnabled(false);
	ui->listWidgetSnippets->setEnabled(false);
	ui->lineEditSnippetTitle->setEnabled(false);
	ui->textEditSnippetContent->setEnabled(false);
}

void MainWindow::on_pushButtonNewSnippet_clicked()
{
	snippetId++;
	Snippet *newItem = new Snippet(snippetId);
	newItem->setText(newItem->getTitle());
	
	QSqlQuery sqlQuery;
	
	if (sqlQuery.exec("INSERT INTO 'snippets' ('id', 'title', 'snippet') VALUES (NULL, '" + newItem->getTitle() + "', '" + newItem->getContent() + "');"))
	{
		qDebug() << "New snippet inserted in table snippets";
	}
	else
	{
		qWarning() << "Can't insert a snippet in table snippets";
	}
	
	ui->listWidgetSnippets->insertItem(ui->listWidgetSnippets->count(), (QListWidgetItem *) newItem);
	ui->listWidgetSnippets->setItemSelected((QListWidgetItem *) newItem, true);
	ui->listWidgetSnippets->setCurrentRow(ui->listWidgetSnippets->count() - 1);
	ui->lineEditSnippetTitle->setText(newItem->getTitle());
	ui->textEditSnippetContent->setText(newItem->getContent());
	
	enableGUI();
}

void MainWindow::on_pushButtonRemoveSnippet_clicked()
{
	if (ui->listWidgetSnippets->selectedItems().count() > 0 && ui->listWidgetSnippets->currentRow() >= 0)
	{
		bool isTheLastElement = false;
		Snippet *removedItem = (Snippet *) ui->listWidgetSnippets->takeItem(ui->listWidgetSnippets->currentRow());
		qDebug() << "removed item id=[" << removedItem->getId() << "]";
		
		if (ui->listWidgetSnippets->count()  == 0)
		{
			isTheLastElement = true;
		}
		
		if (ui->listWidgetSnippets->count() > 0)
		{
			QListWidgetItem *selectedItem = ui->listWidgetSnippets->selectedItems().at(0);
			Snippet *selectedSnippet = (Snippet *) selectedItem;
			ui->lineEditSnippetTitle->setText(selectedSnippet->getTitle());
			ui->textEditSnippetContent->setText(selectedSnippet->getContent());
			
			QSqlQuery sqlQuery;
			sqlQuery.exec("DELETE FROM 'snippets' WHERE id=" + QString::number(removedItem->getId()) + ";");
			
		}
		else if (ui->listWidgetSnippets->count() == 0)
		{
			if (isTheLastElement)
			{
				QSqlQuery sqlQuery;
				sqlQuery.exec("DELETE FROM 'snippets' WHERE id=" + QString::number(removedItem->getId()) + ";");
				isTheLastElement = false;
			}
			
			ui->lineEditSnippetTitle->setText("");
			ui->textEditSnippetContent->setText("");
			disableGUI();
		}
	}
}

void MainWindow::on_listWidgetSnippets_clicked()
{
	QListWidgetItem *selectedItem = ui->listWidgetSnippets->selectedItems().at(0);
	Snippet *selectedSnippet = (Snippet *) selectedItem;
	
	ui->lineEditSnippetTitle->setText(selectedSnippet->getTitle());
	ui->textEditSnippetContent->setText(selectedSnippet->getContent());
	qDebug() << "Snippet selected [id=" << selectedSnippet->getId() << "]";
}

void MainWindow::on_lineEditSnippetTitle_textChanged()
{
	int selectedSnippetId = -1;
	
	if (ui->listWidgetSnippets->count() > 0)
	{
		Snippet *selectedSnippet = (Snippet *) (ui->listWidgetSnippets->selectedItems().at(0));
		selectedSnippetId = selectedSnippet->getId();
		
		selectedSnippet->setTitle(ui->lineEditSnippetTitle->text());
		ui->listWidgetSnippets->selectedItems().at(0)->setText(selectedSnippet->getTitle());	
		
		QSqlQuery sqlQuery;
		sqlQuery.exec("UPDATE 'snippets' SET 'title'='" + selectedSnippet->getTitle() + "' WHERE id=" + QString::number(selectedSnippet->getId()) + ";");
	}
}

void MainWindow::on_textEditSnippetContent_textChanged()
{
	int selectedSnippetId = -1;
	
	if (ui->listWidgetSnippets->count() > 0)
	{
		Snippet *selectedSnippet = (Snippet *) (ui->listWidgetSnippets->selectedItems().at(0));
		selectedSnippetId = selectedSnippet->getId();
		
		selectedSnippet->setContent(ui->textEditSnippetContent->toPlainText());
		
		QSqlQuery sqlQuery;
		sqlQuery.exec("UPDATE 'snippets' SET 'snippet'='" + selectedSnippet->getContent() + "' WHERE id=" + QString::number(selectedSnippet->getId()) + ";");
	}
}

void MainWindow::logListWidgetSnippets()
{
	qDebug() << "Elements on listWidgetSnippets=[" << ui->listWidgetSnippets->count() << "]";
}

void MainWindow::createDBConnection()
{
	qDebug() << "Connecting with the database";
	
	const QString DRIVER("QSQLITE");	
	
	if (QSqlDatabase::isDriverAvailable)
	{
		qDebug() << "QSQLITE driver is available";
	}
	
	QString dbFolder = "C:\\tmp\\";
	QDir dbDirectory(dbFolder);
	QString sqliteFileName = "snippets.db";
	QString dbURI = dbFolder + sqliteFileName;
	
	db = QSqlDatabase::addDatabase(DRIVER);
	db.setDatabaseName(dbURI);
	
	if (!dbDirectory.exists())
	{
		dbDirectory.mkdir(".");
		
		QMessageBox::StandardButton notification;
		notification = QMessageBox::information(this, "New Database Created", "A new directory and a new database\nhas beencreated");
	}
	
	if (db.open())
	{
		qDebug() << "The db has been opened";
	}
	else
	{
		qWarning() << "Can't open db";
	}
	
	QSqlQuery sqlQuery;
	if (sqlQuery.exec("CREATE TABLE IF NOT EXISTS 'snippets' ('id' INTEGER PRIMARY KEY AUTOINCREMENT, 'title' TEXT, 'snippet' TEXT, 'datetime' DATETIME DEFAULT CURRENT_TIMESTAMP)"))
	{
		qDebug() << "Table snippets has been created";
	}
	else
	{
		qWarning() << "Can't create table snippets";
	}
}

int MainWindow::getMaxIdFromDb()
{
	int maxId = -1;
	QSqlQuery sqlQuery;
	
	if (sqlQuery.exec("SELECT seq FROM SQLITE_SEQUENCE;"))
	{
		if(sqlQuery.next())
		{
			qDebug() << "Selecting the max id from db";
			maxId = sqlQuery.value(0).toInt();
		}
	}
	else
	{
		qWarning() << "Can't select the max id from db";
	}
	
	return maxId;
}
