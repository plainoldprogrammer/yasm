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
	ui->pushButtonRemoveSnippet->setText("Remove Snippet");
	this->setWindowTitle("YASM0 v0.2 (Alpha)");

	ui->pushButtonRemoveSnippet->setEnabled(false);	
	ui->listWidgetSnippets->setEnabled(false);
	ui->lineEditSnippetTitle->setEnabled(false);
	ui->textEditSnippetContent->setEnabled(false);
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
	snippetsCount++;
	Snippet *newItem = new Snippet(snippetsCount);
	newItem->setText(newItem->getTitle());
	
	ui->listWidgetSnippets->insertItem(ui->listWidgetSnippets->count(), (QListWidgetItem *) newItem);
	ui->listWidgetSnippets->setItemSelected((QListWidgetItem *) newItem, true);
	ui->lineEditSnippetTitle->setText(newItem->getTitle());
	ui->textEditSnippetContent->setText(newItem->getContent());
	
	enableGUI();
}

void MainWindow::on_pushButtonRemoveSnippet_clicked()
{
	if (ui->listWidgetSnippets->selectedItems().count() > 0 && ui->listWidgetSnippets->currentRow() >= 0)
	{
		Snippet *removedItem = (Snippet *) ui->listWidgetSnippets->takeItem(ui->listWidgetSnippets->currentRow());
		qDebug() << "removed item id=[" << removedItem->getId() << "]";
		
		if (ui->listWidgetSnippets->count() > 0)
		{
			QListWidgetItem *selectedItem = ui->listWidgetSnippets->selectedItems().at(0);
			Snippet *selectedSnippet = (Snippet *) selectedItem;
			ui->lineEditSnippetTitle->setText(selectedSnippet->getTitle());
			ui->textEditSnippetContent->setText(selectedSnippet->getContent());
		}
		else if (ui->listWidgetSnippets->count() == 0)
		{
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

void MainWindow::logListWidgetSnippets()
{
	qDebug() << "Elements on listWidgetSnippets=[" << ui->listWidgetSnippets->count() << "]";
}
