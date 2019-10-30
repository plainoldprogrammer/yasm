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

void MainWindow::on_pushButtonNewSnippet_clicked()
{
	qDebug() << "Creating a new snippet";
	
	// new QListWidgetItem("Snippet", ui->listWidgetSnippets);
	QListWidgetItem *newItem = new QListWidgetItem;
	newItem->setText(QString::number(counter));
	ui->listWidgetSnippets->insertItem(ui->listWidgetSnippets->count(), newItem);
	counter++;
}
