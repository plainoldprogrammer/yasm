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

void OptionsDialog::setDefaultDbFilePath(QString dbFilePath)
{
    defaultDbFilePath = dbFilePath;
    ui->lineEditDbPath->setText(defaultDbFilePath);
}

QString OptionsDialog::getSelectedDbFilePath()
{
    return selectedDbFilePath;
}
