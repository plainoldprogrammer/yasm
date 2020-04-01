#include "optionsdialog.h"
#include "ui_optionsdialog.h"

OptionsDialog::OptionsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OptionsDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("Options");
    this->setMinimumSize(this->size());
    this->setMaximumSize(this->size());
}

OptionsDialog::~OptionsDialog()
{
    delete ui;
}

void OptionsDialog::on_pushButtonSelectDbPath_clicked()
{
    openFileDialogToSelectDb();
}

void OptionsDialog::setDefaultDbFilePath(QString dbFilePath)
{
    defaultDbFilePath = dbFilePath;
    ui->lineEditDbPath->setText(defaultDbFilePath);
}

QString OptionsDialog::getSelectedDbFilePath()
{
    return selectedDbFilePath;
}

/*
 *	This public member function is implemented because the file dialog should be openen also by a
 *	shortcut (Ctrl + O) from the main window and is not possible to call directly a private slot.
 */
void OptionsDialog::openFileDialogToSelectDb()
{
    selectedDbFilePath = QFileDialog::getOpenFileName(this, tr("Open Database"), "c:\\", tr("Database File (*.db)")).replace("/", "\\");

    if (selectedDbFilePath.isEmpty())
    {
        ui->lineEditDbPath->setText(defaultDbFilePath);
    }
    else if (selectedDbFilePath.compare(ui->lineEditDbPath->text()) == 0)
    {
        qDebug() << "Selected the same file that is already selected";
        ui->lineEditDbPath->setText(selectedDbFilePath);
    }
    else
    {
        qDebug() << "A different file has been selected";
        ui->lineEditDbPath->setText(selectedDbFilePath);
    }
}
