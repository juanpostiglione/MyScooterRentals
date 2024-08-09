#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QDialog>
#include <QDebug>
#include <QtSql>
#include <QFileInfo>
#include "accountmanagement.h"
#include "mainwindow.h"
#include "scooter_management.h"

namespace Ui {
class settingsWindow;
}

class settingsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit settingsWindow(QWidget *parent = nullptr);
    ~settingsWindow();

    // Function to get the width of the window
    int windowWidth = this->width();

    // Function to get the heigth of the window
    int windowHeight = this->height();

    // Function for Label position
    void labelPosition();

    // Method to set guestRent
    void setGuest(bool isGuest);

    // Method to set current user
    void setCurrentUser(QString& userN);

    // Method to set account_management pointer
    void setAccMgmt(accountmanagement* a);

    // Method to set scooter_management pointer
    void setScooterMgmt(scooter_management* s);

    QString filePath;
    void setFilePath(QString otherPath);

private slots:

    // Function for click OK
    void on_buttonBox_2_rejected();

    // Function for click Cancel
    void on_buttonBox_2_accepted();

    // Function for Menu button
    void on_toolButton_2_clicked();

    // Function for dropdown options
    void on_comboBox_2_activated(int index);

private:
    Ui::settingsWindow *ui;

    QSqlDatabase db; // Database connection object

    MainWindow* mainWindow;
    accountmanagement* accMgmt;
    scooter_management* scootMgmt;

    QList<QVariantMap> scooters; // Create a list to hold the scooter records
    bool guestAcc; // Whether current user is logged in.
    QString currentUser;
};

#endif // SETTINGSWINDOW_H
