#include "optionsdialog.h"
#include "ui_optionsdialog.h"

OptionsDialog::OptionsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OptionsDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("Options");

}

OptionsDialog::~OptionsDialog()
{
    delete ui;
}

void OptionsDialog::on_pushButtonSelectDbPath_clicked()
{
    qDebug() << "Select a new Db path";
    QString selectedDbPath = QFileDialog::getOpenFileName(this, tr("Open Database"), "c:\\", tr("Database File (*.db)"));
    ui->lineEditDbPath->setText(selectedDbPath);
}
