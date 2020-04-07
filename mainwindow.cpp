#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "snippet.h"
#include "optionsdialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    dbFilePath = "C:\\plainoldprogrammer\\dev\\databases\\snippets.db";
    checkDefaultDbDirectory();
    snippetId = createDbConnection();
    lastCategoryOnDb = getLastCategoryOnDb();
    firstTimeInitializeGUI();

    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Q), this, SLOT(close()));
    new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_O), this, SLOT(on_shortcut_openDb_pressed()));
}

MainWindow::~MainWindow()
{
    delete ui;
    db.close();
}

void MainWindow::firstTimeInitializeGUI()
{
    optionsDialog.setDefaultDbFilePath(dbFilePath);

    ui->actionCut->setEnabled(false);
    ui->actionCopy->setEnabled(false);

    detectIfClipboardHasSomething();
    connect(QApplication::clipboard(), SIGNAL(dataChanged()), this, SLOT(on_clipboard_contentChanged()));

    ui->pushButtonNewCategory->setText("New Category");
    ui->pushButtonRemoveCategory->setText("Remove Category");
    ui->pushButtonNewSnippet->setText("New Snippet");
    ui->pushButtonRemoveSnippet->setText("Remove Snippet");
    this->setWindowTitle("Yasm");
    this->setDefaultEditorFont();

    ui->pushButtonRemoveCategory->setEnabled(false);
    ui->pushButtonNewSnippet->setEnabled(false);
    ui->pushButtonRemoveSnippet->setEnabled(false);
    ui->listWidgetCategories->setEnabled(false);
    ui->listWidgetSnippets->setEnabled(false);
    ui->lineEditSnippetTitle->setEnabled(false);
    ui->textEditSnippetContent->setEnabled(false);

    retrieveDataFromDb();
}

void MainWindow::setDefaultEditorFont()
{
    QFont font;
    font.setFamily("Courier");
    font.setStyleHint(QFont::Monospace);
    font.setFixedPitch(true);
    font.setPointSize(10);
    ui->textEditSnippetContent->setFont(font);

    const int tabStop = 4;
    QFontMetrics metrics(font);
    ui->textEditSnippetContent->setTabStopWidth(tabStop * metrics.width(' '));
}

void MainWindow::enableGUI()
{
    ui->pushButtonRemoveCategory->setEnabled(true);
    ui->pushButtonNewSnippet->setEnabled(true);
    ui->pushButtonRemoveSnippet->setEnabled(true);
    ui->listWidgetCategories->setEnabled(true);
    ui->listWidgetSnippets->setEnabled(true);
    ui->lineEditSnippetTitle->setEnabled(true);
    ui->textEditSnippetContent->setEnabled(true);
}

void MainWindow::disableGUI()
{
    ui->pushButtonRemoveSnippet->setEnabled(false);
    ui->listWidgetSnippets->setEnabled(false);
    ui->lineEditSnippetTitle->setEnabled(false);
    ui->textEditSnippetContent->setEnabled(false);
}

void MainWindow::adjustGUIForSnippetCreation(QString category)
{
    if (thereIsSomeCategoryOnDb() && thereIsSomeSnippetOnDb(category))
    {
        enableGUI();
    }
    else
    {
        ui->pushButtonRemoveCategory->setEnabled(true);
        ui->pushButtonNewSnippet->setEnabled(true);
    }
}

void MainWindow::on_pushButtonNewCategory_clicked()
{
    bool ok;
    QString newCategory = QInputDialog::getText(this, "New Category", "Category Name:\t\t\t\t", QLineEdit::Normal, "", &ok);

    if (ok && !newCategory.isEmpty())
    {
        QSqlQuery sqlQuery;
        if(!isCategoryAlreadyOnDb(newCategory))
        {
            if (sqlQuery.exec("INSERT INTO 'categories' ('id', 'category') VALUES (NULL, '" + newCategory + "');"))
            {
                qDebug() << "New category created on database";

                ui->listWidgetCategories->insertItem(ui->listWidgetCategories->count(), newCategory);
                ui->listWidgetCategories->item(ui->listWidgetCategories->count() - 1)->setIcon(QIcon(":/images/icons/folder.png"));
                ui->listWidgetCategories->setItemSelected(ui->listWidgetCategories->item(ui->listWidgetCategories->count() - 1), true);
                ui->listWidgetCategories->setCurrentRow(ui->listWidgetCategories->count() - 1);

                clearUi();
                disableGUI();
                ui->listWidgetCategories->setEnabled(true);
            }
            else
            {
                qWarning() << "Can't create category on database";
            }
        }
        else
        {
            QMessageBox::warning(this, "Warning", "Category " + newCategory + " already exists");
        }
    }

    adjustGUIForSnippetCreation(newCategory);
}

void MainWindow::on_pushButtonRemoveCategory_clicked()
{
    QString removedCategory = ui->listWidgetCategories->selectedItems().at(0)->text();
    ui->listWidgetCategories->takeItem(ui->listWidgetCategories->currentRow());

    qDebug() << "Removing a category" << removedCategory;

    QSqlQuery sqlQuery;
    if (sqlQuery.exec("DELETE FROM 'snippets' WHERE category='" + removedCategory + "';"))
    {
        qDebug() << "All snippets from category " + removedCategory + " has been removed";
    }
    else
    {
        qWarning() << "Can't removed snippets from category " + removedCategory;
    }

    if (sqlQuery.exec("DELETE FROM 'categories' WHERE category='" + removedCategory + "';"))
    {
        qDebug() << "Category " + removedCategory + " has been removed";
    }
    else
    {
        qWarning() << "Can't remove category " + removedCategory;
    }

    ui->listWidgetSnippets->clear();

    if (ui->listWidgetCategories->count() == 0)
    {
        ui->listWidgetCategories->setEnabled(false);
        ui->listWidgetSnippets->setEnabled(false);
        ui->lineEditSnippetTitle->setEnabled(false);
        ui->textEditSnippetContent->setEnabled(false);
        ui->pushButtonRemoveCategory->setEnabled(false);
        ui->pushButtonNewSnippet->setEnabled(false);
        ui->pushButtonRemoveSnippet->setEnabled(false);
    }

    if (ui->listWidgetCategories->count() > 0)
    {
        qDebug() << "Now the selected category is " + ui->listWidgetCategories->selectedItems().at(0)->text();
    }
    else
    {
        qDebug() << "The categories list is empty";
        clearUi();
    }

    displaySnippets();
}

void MainWindow::on_pushButtonNewSnippet_clicked()
{
    qDebug() << "The current snippetId: " << snippetId;
    snippetId++;
    Snippet *newItem = new Snippet(snippetId);
    newItem->setText(newItem->getTitle());

    QListWidgetItem *selectedCategory = ui->listWidgetCategories->selectedItems().at(0);
    newItem->setCategory(selectedCategory->text());

    QSqlQuery sqlQuery;
    if (sqlQuery.exec("INSERT INTO 'snippets' ('id', 'title', 'snippet', 'category') VALUES (NULL, '" + newItem->getTitle() + "', '" + newItem->getContent() + "', '" + newItem->getCategory() + "');"))
    {
        qDebug() << "New snippet inserted in table snippets";
    }
    else
    {
        qWarning() << "Can't insert a snippet in table snippets";
    }

    ui->listWidgetSnippets->insertItem(ui->listWidgetSnippets->count(), (QListWidgetItem *) newItem);
    ui->listWidgetSnippets->setItemSelected((QListWidgetItem *) newItem, true);
    ui->listWidgetSnippets->setCurrentRow(ui->listWidgetSnippets->count() - 1);
    ui->lineEditSnippetTitle->setText(newItem->getTitle());
    ui->textEditSnippetContent->setPlainText(newItem->getContent());

    enableGUI();
}

void MainWindow::on_pushButtonRemoveSnippet_clicked()
{
    if (ui->listWidgetSnippets->selectedItems().count() > 0 && ui->listWidgetSnippets->currentRow() >= 0)
    {
        bool isTheLastElement = false;
        Snippet *removedItem = (Snippet *) ui->listWidgetSnippets->takeItem(ui->listWidgetSnippets->currentRow());
        qDebug() << "removed item id=[" << removedItem->getId() << "]";

        if (ui->listWidgetSnippets->count()  == 0)
        {
            isTheLastElement = true;
        }

        if (ui->listWidgetSnippets->count() > 0)
        {
            QListWidgetItem *selectedItem = ui->listWidgetSnippets->selectedItems().at(0);
            Snippet *selectedSnippet = (Snippet *) selectedItem;
            ui->lineEditSnippetTitle->setText(selectedSnippet->getTitle());
            ui->textEditSnippetContent->setPlainText(selectedSnippet->getContent());

            QSqlQuery sqlQuery;
            sqlQuery.exec("DELETE FROM 'snippets' WHERE id=" + QString::number(removedItem->getId()) + ";");

        }
        else if (ui->listWidgetSnippets->count() == 0)
        {
            if (isTheLastElement)
            {
                QSqlQuery sqlQuery;
                sqlQuery.exec("DELETE FROM 'snippets' WHERE id=" + QString::number(removedItem->getId()) + ";");
                isTheLastElement = false;
            }

            ui->lineEditSnippetTitle->setText("");
            ui->textEditSnippetContent->setPlainText("");
            disableGUI();
        }
    }
}

void MainWindow::on_listWidgetCategories_itemSelectionChanged()
{
    qDebug() << endl << "onListWidgetCategories_itemSelectionChanged()";
    clearUi();

    if (ui->listWidgetCategories->selectedItems().size() > 0)
    {
        QString currentCategory = ui->listWidgetCategories->selectedItems().at(0)->text();
        qDebug() << "Category changed: " << currentCategory;

        if (isRetrievingDataFromDb)
        {
            if ((currentCategory.compare(lastCategoryOnDb) == 0))
            {
                displaySnippets();
            }
        }
        else
        {
            displaySnippets();
        }
    }

    if (ui->listWidgetSnippets->count() > 0)
    {
        ui->listWidgetSnippets->setEnabled(true);
        ui->pushButtonRemoveSnippet->setEnabled(true);
        ui->lineEditSnippetTitle->setEnabled(true);
        ui->textEditSnippetContent->setEnabled(true);
    }
    else
    {
        ui->listWidgetSnippets->setEnabled(false);
        ui->pushButtonRemoveSnippet->setEnabled(false);
        ui->lineEditSnippetTitle->setEnabled(false);
        ui->textEditSnippetContent->setEnabled(false);
    }
}

void MainWindow::on_listWidgetSnippets_itemSelectionChanged()
{
    if (ui->listWidgetSnippets != NULL)
    {
        if (ui->listWidgetSnippets->count() > 0)
        {
            // Ensure that the list is selected previously by a click.
            if (ui->listWidgetSnippets->selectedItems().count() > 0)
            {
                QListWidgetItem *selectedItem = ui->listWidgetSnippets->selectedItems().at(0);
                Snippet *selectedSnippet = (Snippet *) selectedItem;

                ui->lineEditSnippetTitle->setText(selectedSnippet->getTitle());
                ui->textEditSnippetContent->setPlainText(selectedSnippet->getContent());
            }
        }
    }
}

void MainWindow::on_lineEditSnippetTitle_textChanged()
{
    int selectedSnippetId = -1;

    if (ui->listWidgetSnippets->count() > 0)
    {
        Snippet *selectedSnippet = (Snippet *) (ui->listWidgetSnippets->selectedItems().at(0));
        selectedSnippetId = selectedSnippet->getId();

        selectedSnippet->setTitle(ui->lineEditSnippetTitle->text());
        ui->listWidgetSnippets->selectedItems().at(0)->setText(selectedSnippet->getTitle());

        QSqlQuery sqlQuery;
        sqlQuery.exec("UPDATE 'snippets' SET 'title'='" + selectedSnippet->getTitle() + "' WHERE id=" + QString::number(selectedSnippet->getId()) + ";");
    }
}

void MainWindow::on_textEditSnippetContent_textChanged()
{
    int selectedSnippetId = -1;

    if (ui->listWidgetSnippets->count() > 0)
    {
        Snippet *selectedSnippet = (Snippet *) (ui->listWidgetSnippets->selectedItems().at(0));
        selectedSnippetId = selectedSnippet->getId();

        selectedSnippet->setContent(ui->textEditSnippetContent->toPlainText());

        QSqlQuery sqlQuery;
        sqlQuery.exec("UPDATE 'snippets' SET 'snippet'='" + selectedSnippet->getContent() + "' WHERE id=" + QString::number(selectedSnippet->getId()) + ";");
    }
}

void MainWindow::logListWidgetSnippets()
{
    qDebug() << "Elements on listWidgetSnippets=[" << ui->listWidgetSnippets->count() << "]";
}

int MainWindow::createDbConnection()
{
    qDebug() << "Connecting with the database";

    const QString DRIVER("QSQLITE");

    if (QSqlDatabase::isDriverAvailable)
    {
        qDebug() << "QSQLITE driver is available";
    }

    db = QSqlDatabase::addDatabase(DRIVER);
    db.setDatabaseName(dbFilePath);

    if (db.open())
    {
        qDebug() << "The db has been opened";
    }
    else
    {
        qWarning() << "Can't open db";
    }

    QSqlQuery sqlQuery;
    if (sqlQuery.exec("CREATE TABLE IF NOT EXISTS 'snippets' ('id' INTEGER PRIMARY KEY AUTOINCREMENT, 'title' TEXT, 'snippet' TEXT, 'category' TEXT, 'datetime' DATETIME DEFAULT CURRENT_TIMESTAMP);"))
    {
        qDebug() << "Table snippets has been created";
    }
    else
    {
        qWarning() << "Can't create table snippets";
    }

    if (sqlQuery.exec("CREATE TABLE IF NOT EXISTS 'categories' ('id' INTEGER PRIMARY KEY AUTOINCREMENT, 'category' TEXT, 'datetime' DATETIME DEFAULT CURRENT_TIMESTAMP);"))
    {
        qDebug() << "Table categories has been created";
    }
    else
    {
        qWarning() << "Can't create table categories";
    }

    return getMaxIdFromDb();
}

int MainWindow::getMaxIdFromDb()
{
    int maxId = -1;
    QSqlQuery sqlQuery;

    if (sqlQuery.exec("SELECT seq FROM SQLITE_SEQUENCE WHERE name='snippets';"))
    {
        if(sqlQuery.next())
        {
            qDebug() << "Selecting the max id from db";
            maxId = sqlQuery.value(0).toInt();
        }
    }
    else
    {
        qWarning() << "Can't select the max id from db";
    }

    // On empty table SQL returns -1. Assign a correct initial value.
    if (maxId < 0)
    {
        maxId = 0;
    }

    return maxId;
}

bool MainWindow::isCategoryAlreadyOnDb(QString category)
{
    bool categoryMatch = false;

    QSqlQuery sqlQuery;
    sqlQuery.exec("SELECT category FROM categories ORDER BY datetime ASC;");

    while (sqlQuery.next())
    {
        QString retrievedCategory = sqlQuery.value(0).toString();

        if (category.compare(retrievedCategory, Qt::CaseInsensitive) == 0)
        {
            qDebug() << category + " already exist on database";
            categoryMatch = true;
            break;
        }
    }

    return categoryMatch;
}

bool MainWindow::thereIsSomeCategoryOnDb()
{
    int countCategoriesOnDb = 0;

    QSqlQuery sqlQuery;
    sqlQuery.exec("SELECT COUNT(*) FROM 'categories';");

    if (sqlQuery.next())
    {
        countCategoriesOnDb = sqlQuery.value(0).toInt();
    }

    qDebug() << "categories on db: " + QString::number(countCategoriesOnDb);

    if (countCategoriesOnDb != 0)
    {
        return true;
    }

    return false;
}

bool MainWindow::thereIsSomeSnippetOnDb(QString category)
{
    int countSnippetsOnDb = 0;

    QSqlQuery sqlQuery;
    sqlQuery.exec("SELECT COUNT(*) FROM 'snippets' WHERE category='" + category + "';");

    if (sqlQuery.next())
    {
        countSnippetsOnDb = sqlQuery.value(0).toInt();
    }

    if (countSnippetsOnDb != 0)
    {
        return true;
    }

    return false;
}

void MainWindow::clearUi()
{
    ui->listWidgetSnippets->clear();
    ui->lineEditSnippetTitle->clear();
    ui->textEditSnippetContent->clear();
}

void MainWindow::displaySnippets()
{
    qDebug() << "displaySnippets() start";
    ui->listWidgetSnippets->clear();

    if (ui->listWidgetCategories->selectedItems().size() > 0)
    {
        QString selectedCategory = ui->listWidgetCategories->selectedItems().at(0)->text();
        QSqlQuery sqlQuery;

        if (sqlQuery.exec("SELECT * FROM 'snippets' WHERE category = '" + selectedCategory + "';"))
        {
            QVector<Snippet> snippetsFromDb;
            Snippet *snippetRecoveredFromDb;

            while (sqlQuery.next())
            {
                qDebug() << "Snippet id=[" << sqlQuery.value(0).toInt() << "] title=[" << sqlQuery.value(1).toString() << "]";

                int snippetFromDbId = sqlQuery.value(0).toInt();
                QString snippetFromDbTitle = sqlQuery.value(1).toString();
                QString snippetFromDbContent = sqlQuery.value(2).toString();
                QString snippetFromDbCategory = sqlQuery.value(3).toString();

                snippetRecoveredFromDb = new Snippet(snippetFromDbId);
                snippetRecoveredFromDb->setTitle(snippetFromDbTitle);
                snippetRecoveredFromDb->setText(snippetRecoveredFromDb->getTitle());
                snippetRecoveredFromDb->setContent(snippetFromDbContent);
                snippetRecoveredFromDb->setCategory(snippetFromDbCategory);

                ui->listWidgetSnippets->insertItem(ui->listWidgetSnippets->count(), (QListWidgetItem *) snippetRecoveredFromDb);
                ui->listWidgetSnippets->item(ui->listWidgetSnippets->count() - 1)->setIcon(QIcon(":/images/icons/file.png"));
                ui->listWidgetSnippets->setItemSelected((QListWidgetItem *) snippetRecoveredFromDb, true);
                ui->listWidgetSnippets->setCurrentRow(ui->listWidgetSnippets->count() - 1);
                ui->lineEditSnippetTitle->setText(snippetRecoveredFromDb->getTitle());
                ui->textEditSnippetContent->setPlainText(snippetRecoveredFromDb->getContent());
            }
        }
    }
}

void MainWindow::on_actionExit_triggered()
{
    qDebug() << "Action exit clicked";
    this->close();
}

void MainWindow::on_actionCut_triggered()
{
    if (ui->lineEditSnippetTitle->hasFocus())
    {
        ui->lineEditSnippetTitle->cut();
    }
    else if (ui->textEditSnippetContent->hasFocus())
    {
        ui->textEditSnippetContent->cut();
    }

    ui->actionCut->setEnabled(false);
    ui->actionCopy->setEnabled(false);
}

void MainWindow::on_actionCopy_triggered()
{
    if (ui->lineEditSnippetTitle->hasFocus())
    {
        ui->lineEditSnippetTitle->copy();
    }
    else if (ui->textEditSnippetContent->hasFocus())
    {
        ui->textEditSnippetContent->copy();
    }

    ui->actionCut->setEnabled(false);
    ui->actionCopy->setEnabled(false);
}

void MainWindow::on_actionPaste_triggered()
{

    if (ui->lineEditSnippetTitle->hasFocus())
    {
        ui->lineEditSnippetTitle->paste();
    }
    else if (ui->textEditSnippetContent->hasFocus())
    {
        ui->textEditSnippetContent->paste();
    }
}

void MainWindow::on_actionOptions_triggered()
{
    optionsDialog.setModal(true);

    if (optionsDialog.exec() == QDialog::Accepted)
    {
        qDebug() << "Ok";
        qDebug() << "Read the file: " << optionsDialog.getSelectedDbFilePath();
        openAnotherDb();
    }
    else
    {
        qDebug() << "Cancel";
    }
}

void MainWindow::on_actionAbout_Yasm_triggered()
{
    QMessageBox::information(this, "About Yasm", "Yasm\nYet Another Snippet Manager\nVersion: 1.0 (Dev)\nRelease Date: 2020\nDeveloper: Plainoldprogrammer");
}

void MainWindow::on_lineEditSnippetTitle_selectionChanged()
{
    if (ui->lineEditSnippetTitle->hasSelectedText())
    {
        ui->actionCut->setEnabled(true);
        ui->actionCopy->setEnabled(true);
    }
}

void MainWindow::on_textEditSnippetContent_selectionChanged()
{
    if (ui->textEditSnippetContent->textCursor().hasSelection())
    {
        ui->actionCut->setEnabled(true);
        ui->actionCopy->setEnabled(true);
    }
}

void MainWindow::on_clipboard_contentChanged()
{
    detectIfClipboardHasSomething();
}

void MainWindow::detectIfClipboardHasSomething()
{
    if (QApplication::clipboard()->mimeData()->hasText())
    {
        ui->actionPaste->setEnabled(true);
    }
    else
    {
        ui->actionPaste->setEnabled(false);
    }
}

void MainWindow::retrieveDataFromDb()
{
    isRetrievingDataFromDb = true;

    QSqlQuery sqlQuery;
    if (thereIsSomeCategoryOnDb())
    {
        lastCategoryOnDb = getLastCategoryOnDb();

        if (sqlQuery.exec("SELECT category FROM 'categories' ORDER BY datetime ASC;"))
        {
            qDebug("Reading all categories from the db");

            while (sqlQuery.next())
            {
                QString categoryRecoveredFromDb = sqlQuery.value(0).toString();

                ui->listWidgetCategories->insertItem(ui->listWidgetCategories->count(), categoryRecoveredFromDb);
                ui->listWidgetCategories->item(ui->listWidgetCategories->count() - 1)->setIcon(QIcon(":/images/icons/folder.png"));
                ui->listWidgetCategories->setItemSelected(ui->listWidgetCategories->item(ui->listWidgetCategories->count() - 1), true);
                ui->listWidgetCategories->setCurrentRow(ui->listWidgetCategories->count() - 1);
                ui->listWidgetCategories->setEnabled(true);
            }
        }

        adjustGUIForSnippetCreation(ui->listWidgetCategories->selectedItems().at(0)->text());
    }
    else
    {
        qWarning() << "Can't read the snippets from the db";
    }

    isRetrievingDataFromDb = false;
}

void MainWindow::on_shortcut_openDb_pressed()
{
    QString previousSelectedDbOnDialog = optionsDialog.getSelectedDbFilePath();
    optionsDialog.openFileDialogToSelectDb();
    QString newSelectedDbOnDialog = optionsDialog.getSelectedDbFilePath();

    if ( !(previousSelectedDbOnDialog.compare(newSelectedDbOnDialog) == 0) )
    {
        openAnotherDb();
    }
}

void MainWindow::openAnotherDb()
{
    ui->listWidgetSnippets->clear();
    ui->listWidgetCategories->clear();

    // Close the previous database file in order to open the new selected.
    db.close();

    dbFilePath = optionsDialog.getSelectedDbFilePath();
    createDbConnection();
    snippetId = getMaxIdFromDb();
    retrieveDataFromDb();
}

QString MainWindow::getLastCategoryOnDb()
{
    QSqlQuery sqlQuery;
    sqlQuery.exec("SELECT category FROM categories ORDER BY id DESC LIMIT 1;");
    sqlQuery.next();

    return sqlQuery.value(0).toString();
}

void MainWindow::checkDefaultDbDirectory()
{
    QFileInfo file(dbFilePath);
    QDir dbDirectory(file.path().replace("/", "\\").append("\\"));

    if (!dbDirectory.exists())
    {
        dbDirectory.mkdir(".");
        QMessageBox::information(this, "New Directory Created", "A new directory for the database\nhas been created.");
    }
}
