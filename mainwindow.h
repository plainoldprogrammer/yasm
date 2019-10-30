#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

static int snippetsCount = 0;

class MainWindow : public QMainWindow
{
	Q_OBJECT
	
public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();
	void firstTimeInitializeGUI();
	void enableGUI();
	
private:
	Ui::MainWindow *ui;
	
private slots:
	void on_pushButtonNewSnippet_clicked();
	void on_listWidgetSnippets_clicked();
	void on_lineEditSnippetTitle_textChanged();
};
#endif // MAINWINDOW_H
