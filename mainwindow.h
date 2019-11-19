#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT
	
public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();
	void firstTimeInitializeGUI();
	void setDefaultEditorFont();
	void enableGUI();
	void disableGUI();
	void adjustGUIForSnippetCreation(QString);
	void logListWidgetSnippets();
	void createDBConnection();
	int getMaxIdFromDb();
	bool isCategoryAlreadyOnDb(QString);
	bool thereIsSomeCategoryOnDb();
	bool thereIsSomeSnippetOnDb(QString);
	void clearUi();
	void displaySnippets();
	
private:
	Ui::MainWindow *ui;
	QSqlDatabase db;
	int snippetId;
	
private slots:
	void on_pushButtonNewCategory_clicked();
	void on_pushButtonRemoveCategory_clicked();
	void on_pushButtonNewSnippet_clicked();
	void on_pushButtonRemoveSnippet_clicked();
	void on_listWidgetCategories_clicked();
	void on_listWidgetSnippets_clicked();
	void on_lineEditSnippetTitle_textChanged();
	void on_textEditSnippetContent_textChanged();
};
#endif // MAINWINDOW_H
