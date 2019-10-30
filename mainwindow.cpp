#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
: QMainWindow(parent)
, ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	
	ui->pushButtonNewSnippet->setText("New Snippet");
	this->setWindowTitle("YASM (Alpha)");
	
	ui->listViewSnippets->setEnabled(false);
	ui->lineEditSnippetTitle->setEnabled(false);
	ui->textEditSnippetContent->setEnabled(false);
}

MainWindow::~MainWindow()
{
	delete ui;
}

