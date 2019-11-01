#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "snippet.h"
#include <QDebug>
#include <QListWidgetItem>

MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent)
, ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	firstTimeInitializeGUI();
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::firstTimeInitializeGUI()
{
	ui->pushButtonNewSnippet->setText("New Snippet");
	this->setWindowTitle("YASM0 v0.1 (Alpha)");
	
	ui->listWidgetSnippets->setEnabled(false);
	ui->lineEditSnippetTitle->setEnabled(false);
	ui->textEditSnippetContent->setEnabled(false);
}

void MainWindow::enableGUI()
{
	ui->listWidgetSnippets->setEnabled(true);
	ui->lineEditSnippetTitle->setEnabled(true);
	ui->textEditSnippetContent->setEnabled(true);
}

void MainWindow::on_pushButtonNewSnippet_clicked()
{
	snippetsCount++;
	Snippet *newItem = new Snippet(snippetsCount);
	newItem->setText(newItem->getTitle());
	
	ui->listWidgetSnippets->insertItem(ui->listWidgetSnippets->count(), (QListWidgetItem *) newItem);
	ui->listWidgetSnippets->setItemSelected((QListWidgetItem *) newItem, true);
	ui->lineEditSnippetTitle->setText(newItem->getTitle());
	ui->textEditSnippetContent->setText(newItem->getContent());
	
	enableGUI();
}

void MainWindow::on_listWidgetSnippets_clicked()
{
	QListWidgetItem *selectedItem = ui->listWidgetSnippets->selectedItems().at(0);
	Snippet *selectedSnippet = (Snippet *) selectedItem;
	
	ui->lineEditSnippetTitle->setText(selectedSnippet->getTitle());
	ui->textEditSnippetContent->setText(selectedSnippet->getContent());
	qDebug() << "Selecting an item list" << selectedSnippet->getId();
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
	}
}

void MainWindow::on_pushButtonRemoveSelectedSnippet_clicked()
{
	qDebug() << "Removing the selected snippet";
}

void MainWindow::logListWidgetSnippets()
{
	qDebug() << "Elements on listWidgetSnippets=[" << ui->listWidgetSnippets->count() << "]";
}
