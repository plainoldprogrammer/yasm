#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include <QDialog>
#include <QDebug>
#include <QFileDialog>

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

private:
    Ui::OptionsDialog *ui;
    QString defaultDbFilePath;
    QString selectedDbFilePath;

private slots:
    void on_pushButtonSelectDbPath_clicked();
};

#endif // OPTIONSDIALOG_H
