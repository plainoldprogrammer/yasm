#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include <QDialog>
#include <QDebug>
#include <QFileDialog>
#include <QFontDatabase>
#include <QStringList>

namespace Ui {
class OptionsDialog;
}

class OptionsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit OptionsDialog(QWidget *parent = nullptr);
    ~OptionsDialog();
    void setDefaultDbFilePath(QString);
    QString getSelectedDbFilePath();
    void openFileDialogToSelectDb();
    QString getSelectedTheme();
    QString getSelectedFont();
    QString getSelectedFontSize();

private:
    Ui::OptionsDialog *ui;
    QString defaultDbFilePath;
    QString selectedDbFilePath;
    void addAvailableThemes();
    void addAvailableFonts();
    void addAvailableFontSizes();

private slots:
    void on_pushButtonSelectDbPath_clicked();
};

#endif // OPTIONSDIALOG_H
