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
    QString selectedDbFilePath = QFileDialog::getOpenFileName(this, tr("Open Database"), "c:\\", tr("Database File (*.db)"));

    if (selectedDbFilePath.isEmpty())
    {
        ui->lineEditDbPath->setText(defaultDbFilePath);
    }
    else
    {
        ui->lineEditDbPath->setText(selectedDbFilePath);
    }
}

void OptionsDialog::setDefaultDbFilePath(QString dbFilePath)
{
    defaultDbFilePath = dbFilePath;
    ui->lineEditDbPath->setText(defaultDbFilePath);
}
