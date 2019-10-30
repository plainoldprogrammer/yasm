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
	
	ui->listWidgetSnippets->insertItem(ui->listWidgetSnippets->count(), (QListWidgetItem *) newItem);
	ui->lineEditSnippetTitle->setText(newItem->getTitle());
	ui->textEditSnippetContent->setText(newItem->getContent());
	
	enableGUI();
}
