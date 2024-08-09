#ifndef ACCOUNTMANAGEMENT_H
#define ACCOUNTMANAGEMENT_H

#include <QDialog>
#include <QtSql>
#include <QDebug>

namespace Ui {
class accountmanagement;
}

class accountmanagement : public QDialog
{
    Q_OBJECT

public:
    explicit accountmanagement(QWidget *parent = nullptr);
    ~accountmanagement();

    // Creates a new account with the given username, password, and status.
    bool createAccount(const QString& username, const QString& password, const QString& status);

    // Deletes the account with the given username.
    bool deleteAccount(const QString& username);

    // Promotes a user to an employee or administrator role.
    bool promoteAccount(const QString& username, const QString& newStatus);

    // Demotes an employee or administrator to a regular user role.
    bool demoteAccount(const QString& username);

    // Changes the username and password of the given user
    bool changeUserPass(QString& oldUser, QString& newUser, QString& newPass);

    // Retrieves all accounts from the database and returns them as a list of QVariantMap.
    QList<QVariantMap> getAllAccounts();

    QString filePath;
    void setFilePath(QString otherPath);

    // 'employee' or 'administrator'
    QString accRank;

private slots:
    void on_pushButton_2_clicked();

    void on_toolButton_clicked();

    void on_comboBox_3_currentIndexChanged(int index);

    void on_pushButton_4_clicked();

    void on_pushButton_clicked();

private:
    Ui::accountmanagement *ui;

    QSqlDatabase db; // Database connection object.

    QString pwdKey = "HI0qnP4YuI1WttiUie0e"; // Key used for password encryption and decryption

    // Function to encrypt and decrypt each user's password before storing in database.
    QString encryptPassword(QString pwdStr);

    // Function to decrypt each user's password after reading from database.
    QString decryptPassword(QString pwdStr);
};

#endif // ACCOUNTMANAGEMENT_H

