#include "accountmanagement.h"
#include "ui_accountmanagement.h"
#include "promoteaccount.h"
#include "employee_manager.h"
#include "changedata.h"
#include "scooterrequests.h"
#include "deleteuser.h"
#include "adddeletescooter.h"

accountmanagement::accountmanagement(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::accountmanagement)
{
    ui->setupUi(this);

    ui->comboBox_3->hide();
    ui->comboBox_3->setStyleSheet("QComboBox QAbstractItemView { color: rgb(255, 255, 255); }");

    // Set style of buttons and window

    this->setStyleSheet("background-color: rgb(1, 68, 3);");

    ui->label_2->setStyleSheet("QLabel { color : rgb(255, 165, 0); }");

    ui->label_3->setStyleSheet("QLabel { color : rgb(255, 255, 255); }");
    ui->label_5->setStyleSheet("QLabel { color : rgb(255, 255, 255); }");

    ui->frame->setStyleSheet("QFrame {" "background-color: black;" "}");

    ui->toolButton->setStyleSheet("QToolButton {" "color: white;" "background-color: grey;"  "padding: 3px;"
                                    "}" "QToolButton:hover {" "background-color: orange;""}");

    ui->pushButton->setStyleSheet("QPushButton {" "color: white;" "background-color: grey;"  "padding: 3px;"
                                    "}" "QPushButton:hover {" "background-color: orange;""}");

    ui->pushButton_2->setStyleSheet("QPushButton {" "color: white;" "background-color: grey;"  "padding: 3px;"
                                    "}" "QPushButton:hover {" "background-color: orange;""}");

    ui->pushButton_4->setStyleSheet("QPushButton {" "color: white;" "background-color: grey;"  "padding: 3px;"
                                    "}" "QPushButton:hover {" "background-color: orange;""}");
}

accountmanagement::~accountmanagement()
{
    delete ui;
    db.close();
}

// Creates a new account with the given username, password, and role.
bool accountmanagement::createAccount(const QString& username, const QString& password, const QString& status) {
    QString pwdEncrypted = encryptPassword(password);

    QSqlQuery query;
    query.prepare("INSERT INTO loginDetails (username, password, status) VALUES (:username, :password, :status)");
    query.bindValue(":username", username);
    query.bindValue(":password", pwdEncrypted);
    query.bindValue(":status", status); //"customer", "employee", or "administrator". Default status is "customer".


    // Execute the query and return whether it was successful
    return query.exec();
}

// Deletes the account with the given username.
bool accountmanagement::deleteAccount(const QString& username) {
    QSqlQuery query;
    query.prepare("DELETE FROM loginDetails WHERE username = :username");
    query.bindValue(":username", username);

    // Execute the query and check the result
    if (!query.exec()) {
        qDebug() << "Failed to execute query:" << query.lastError().text();
        return false;
    }

    // Check if any rows were affected
    if (query.numRowsAffected() == 0) {
        qDebug() << "No rows affected, username may not exist or status already set";
        return false;
    }

    return true;
}

// Promotes a user to an employee or administrator role.
bool accountmanagement::promoteAccount(const QString& username, const QString& newStatus) {
    QSqlQuery query;
    query.prepare("UPDATE loginDetails SET status = :status WHERE username = :username");
    query.bindValue(":status", newStatus);
    query.bindValue(":username", username);

    // Execute the query and check the result
        if (!query.exec()) {
        qDebug() << "Failed to execute query:" << query.lastError().text();
        return false;
    }

    // Check if any rows were affected
    if (query.numRowsAffected() == 0) {
        qDebug() << "No rows affected, username may not exist or status already set";
        return false;
    }

    return true;
}

// Demotes an employee or administrator to a regular user role.
bool accountmanagement::demoteAccount(const QString& username) {
    QSqlQuery query;
    query.prepare("UPDATE loginDetails SET role = 'customer' WHERE username = :username");
    query.bindValue(":username", username);

    // Execute the query and return whether it was successful
    return query.exec();
}

// Changes the username and password of the given user
bool accountmanagement::changeUserPass(QString& oldUser, QString& newUser, QString& newPass)
{
    // Check if username has changed
    if(newUser == "")
    {
        // Username will not change if field is blank
        newUser = oldUser;
    }
    QSqlQuery query;

    // Check if password has changed
    if(newPass != "")
    {
        newPass = encryptPassword(newPass);

        // Change password
        query.prepare("UPDATE loginDetails SET username = :new_user, password = :password WHERE username = :old_user");
        query.bindValue(":new_user", newUser);
        query.bindValue(":password", newPass);
        query.bindValue(":old_user", oldUser);
    }
    else
    {
        // Don't change password
        query.prepare("UPDATE loginDetails SET username = :new_user WHERE username = :old_user");
        query.bindValue(":new_user", newUser);
        query.bindValue(":old_user", oldUser);
    }

    // Execute the query and check the result
    if (!query.exec()) {
        qDebug() << "Failed to execute query:" << query.lastError().text();
        return false;
    }

    // Check if any rows were affected
    if (query.numRowsAffected() == 0) {
        qDebug() << "No rows affected, username may not exist or status already set";
        return false;
    }

    return true;
}

// Retrieves all accounts from the database and returns them as a list of QVariantMap.
QList<QVariantMap> accountmanagement::getAllAccounts() {
    QList<QVariantMap> accounts;
    QSqlQuery query("SELECT * FROM loginDetails");

    // Iterate through the results and add them to the list
    while (query.next()) {
        QVariantMap account;
        account["account_id"] = query.value("account_id");
        account["username"] = query.value("username");
        account["status"] = query.value("status");
        accounts.append(account);
    }

    return accounts;
}


// Function to encrypt each user's password before storing in database.
QString accountmanagement::encryptPassword(QString pwdStr)
{
    QString encodedPwd;
    char16_t newChar;

    char16_t strChar;
    char16_t keyChar;

    //char16_t QChar::unicode() const

    for(int i = 0; i < pwdStr.size(); i++)
    {
        strChar = pwdStr[i].unicode();
        keyChar = pwdKey[i].unicode();

        // XOR password with the key to encrypt.
        newChar = strChar ^ keyChar;

        // Store encrypted password as only printable values
        if(newChar > 79)
        {
            newChar -= 47;
        }
        if(newChar < 79)
        {
            newChar += 47;
        }

        QChar tempChar(newChar);
        encodedPwd.append(tempChar);
    }
    return encodedPwd;
}


// Function to decrypt each user's password after reading from database.
QString accountmanagement::decryptPassword(QString pwdStr)
{
    QString decodedPwd;
    char16_t newChar;

    char16_t strChar;
    char16_t keyChar;

    for(int i = 0; i < pwdStr.size(); i++)
    {
        strChar = pwdStr[i].unicode();
        keyChar = pwdKey[i].unicode();

        // Revert characters back to their potentially unprintable forms.
        if(strChar > 79)
        {
            strChar -= 47;
        }
        if(strChar < 79)
        {
            strChar += 47;
        }

        // XOR encrypted password with the key to decrypt.
        newChar = strChar ^ keyChar;

        QChar tempChar(newChar);
        decodedPwd.append(tempChar);
    }
    return decodedPwd;
}


// When promote account button is pressed
void accountmanagement::on_pushButton_2_clicked()
{
    this->close();
    promoteaccount prom;
    prom.accRank = accRank;
    prom.setFilePath(filePath);

    prom.setModal(true);
    prom.exec();
}


// When Menu button is clicked
void accountmanagement::on_toolButton_clicked()
{
    ui->comboBox_3->show();
}

// Changing windows from the Menu bar
void accountmanagement::on_comboBox_3_currentIndexChanged(int index)
{
    promoteaccount prom;
    employee_manager em;
    accountmanagement acc;
    changeData data;
    deleteUser user;
    scooterRequests scooter;
    addDeleteScooter addD;

    prom.accRank = accRank;
    em.accRank = accRank;
    acc.accRank = accRank;
    data.accRank = accRank;
    user.accRank = accRank;
    scooter.accRank = accRank;
    addD.accRank = accRank;

    prom.setFilePath(filePath);
    em.setFilePath(filePath);
    acc.setFilePath(filePath);
    data.setFilePath(filePath);
    user.setFilePath(filePath);
    scooter.setFilePath(filePath);
    addD.setFilePath(filePath);

    MainWindow* mainWindow = new MainWindow(nullptr);

    switch(index)
    {
    case 1: // Open employee manager window
        this->close();
        prom.close();
        user.close();
        data.close();
        delete mainWindow;
        em.setModal(true);
        em.exec();
        break;

    case 2: // Open add/delete/update scooter window
        this->close();
        prom.close();
        user.close();
        data.close();
        em.close();
        delete mainWindow;
        addD.setModal(true);
        addD.exec();
        break;


    case 3: // Open rental requests window
        this->close();
        prom.close();
        user.close();
        data.close();
        em.close();
        delete mainWindow;
        scooter.setModal(true);
        scooter.exec();
        break;

    case 4: // Log out
        this->close();
        prom.close();
        user.close();
        data.close();
        em.close();
        scooter.close();
        mainWindow->show();
        mainWindow->resize(400,500);
        break;
    }
}

// When change data user button is pressed
void accountmanagement::on_pushButton_4_clicked()
{
    this->close();
    changeData data;
    data.accRank = accRank;
    data.setFilePath(filePath);

    data.setModal(true);
    data.exec();
}

// When delete account button is pressed
void accountmanagement::on_pushButton_clicked()
{
    this->close();
    deleteUser user;
    user.accRank = accRank;
    user.setFilePath(filePath);

    user.setModal(true);
    user.exec();
}

void accountmanagement::setFilePath(QString otherPath)
{
    filePath = otherPath;

    // Image of scooter to display in the window
    QPixmap image(filePath + "/scooter.png");
    ui->label->setPixmap(image);
    ui->comboBox_3->hide();

    // Initialize database
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(filePath + "/database_q.db");


    // Attempt to open the database
    if (!db.open()) {
        qDebug() << "Error: Could not open database.";
        return;
    }

    // Create the loginDetails table if it doesn't already exist
    QSqlQuery query;
    query.prepare("CREATE TABLE IF NOT EXISTS loginDetails ("
                  "account_id INTEGER PRIMARY KEY AUTOINCREMENT, "
                  "username TEXT UNIQUE, "
                  "password TEXT, "
                  "status TEXT)");

    if(query.exec())
    {
        //If there are no accounts, add a new administrator account.
        createAccount("admin0", "admin0", "administrator");
    }
}
