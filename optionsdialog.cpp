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
    addAvailableFontSizes();
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
    return ui->comboBoxThemes->currentText();
}

void OptionsDialog::addAvailableThemes()
{
    ui->comboBoxThemes->addItem("Dark");
    ui->comboBoxThemes->addItem("Light");

    ui->comboBoxThemes->setCurrentIndex(1);
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

QString OptionsDialog::getSelectedFont()
{
    return ui->comboBoxFonts->currentText();
}

void OptionsDialog::addAvailableFontSizes()
{
    QList<int> fontSizes = { 8, 9, 10, 11, 12, 14, 16, 18, 20, 22, 24, 26, 28, 36, 48, 72 };

    for (int i = 0; i < fontSizes.size(); i++)
    {
        ui->comboBoxFontSizes->addItem(QString::number(fontSizes.at(i)));
    }
}

QString OptionsDialog::getSelectedFontSize()
{
    return ui->comboBoxFontSizes->currentText();
}
