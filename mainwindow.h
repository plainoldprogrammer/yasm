#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

static int counter = 1;

class MainWindow : public QMainWindow
{
	Q_OBJECT
	
public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();
	void firstTimeInitializeGUI();
	
private:
	Ui::MainWindow *ui;
	
private slots:
	void on_pushButtonNewSnippet_clicked();
};
#endif // MAINWINDOW_H
