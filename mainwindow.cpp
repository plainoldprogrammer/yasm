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
	this->setWindowTitle("YASM (Alpha)");
	
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
	qDebug() << "Creating a new snippet";
	
	Snippet *newItem = new Snippet();
	newItem->setText("New Snippet");
	newItem->setId(snippetsCount);
	snippetsCount++;
	
	ui->listWidgetSnippets->insertItem(ui->listWidgetSnippets->count(), (QListWidgetItem *) newItem);
	ui->lineEditSnippetTitle->setText(newItem->getTitle());
	ui->textEditSnippetContent->setText(newItem->getContent());
	ui->listWidgetSnippets->setItemSelected((QListWidgetItem *) newItem, true);
	
	enableGUI();
}

void MainWindow::on_listWidgetSnippets_clicked()
{
	QListWidgetItem *selectedItem = ui->listWidgetSnippets->selectedItems().at(0);
	Snippet *selectedSnippet = (Snippet *) selectedItem;
	qDebug() << "Selecting an item list" << selectedSnippet->getId();
}

void MainWindow::on_lineEditSnippetTitle_textChanged()
{
	int selectedSnippetId = -1;
	
	if (ui->listWidgetSnippets->count() > 1)
	{
		Snippet *selectedSnippet = (Snippet *) (ui->listWidgetSnippets->selectedItems().at(0));
		selectedSnippetId = selectedSnippet->getId();
		
		selectedSnippet->setTitle(ui->lineEditSnippetTitle->text());
		ui->listWidgetSnippets->selectedItems().at(0)->setText(selectedSnippet->getTitle());	
	}
	
	qDebug() << "Editing the title of the selected snippet" << selectedSnippetId;
}

void MainWindow::on_textEditSnippetContent_textChanged()
{
	int selectedSnippetId = -1;
	
	if (ui->listWidgetSnippets->count() > 1)
	{
		selectedSnippetId = ((Snippet *) (ui->listWidgetSnippets->selectedItems().at(0)))->getId();
	}
	
	qDebug() << "Editing the content of the selected snippet" << selectedSnippetId;
}
