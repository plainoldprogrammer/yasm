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

    addAvailableThemes();
    addAvailableFonts();
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
    selectedDbFilePath = defaultDbFilePath;
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
    QString previousSelectedDbFilePath = selectedDbFilePath;
    selectedDbFilePath = QFileDialog::getOpenFileName(this, tr("Open Database"), "c:\\", tr("Database File (*.db)")).replace("/", "\\");

    if (selectedDbFilePath.isEmpty())
    {
        selectedDbFilePath = previousSelectedDbFilePath;
        ui->lineEditDbPath->setText(defaultDbFilePath);
    }
    else if (selectedDbFilePath.compare(ui->lineEditDbPath->text()) == 0)
    {
        ui->lineEditDbPath->setText(selectedDbFilePath);
    }
    else
    {
        ui->lineEditDbPath->setText(selectedDbFilePath);
    }
}

QString OptionsDialog::getSelectedTheme()
{
    return ui->comboBoxTheme->currentText();
}

void OptionsDialog::addAvailableThemes()
{
    ui->comboBoxTheme->addItem("Dark");
    ui->comboBoxTheme->addItem("Light");

    ui->comboBoxTheme->setCurrentIndex(1);
}

void OptionsDialog::addAvailableFonts()
{
    QFontDatabase fontDatabase;
    const QStringList fontFamilies = fontDatabase.families();

    QStringList::const_iterator constIterator;
    for (constIterator = fontFamilies.constBegin(); constIterator != fontFamilies.constEnd(); constIterator++)
    {
        ui->comboBoxFonts->addItem(*constIterator);
    }
}
