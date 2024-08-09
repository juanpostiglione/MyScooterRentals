#ifndef RENTSCOOTER_H
#define RENTSCOOTER_H

#include <QDialog>
#include "mainwindow.h"
#include "scooter_management.h"
#include <QDebug>
#include <QtSql>
#include <QFileInfo>

namespace Ui {
class rentScooter;
}

class rentScooter : public QDialog
{
    Q_OBJECT

public:
    explicit rentScooter(QWidget *parent = nullptr);
    ~rentScooter();

    // Set the font attributes of labels
    void setFontAttributes();

    // Set the Labels Position
    void labelPosition();

    // Method to set guestRent
    void setGuest(bool isGuest);

    // Method to set scooter_management pointer
    void setScooterMgmt(scooter_management* s);

    // Method to set current user
    void setCurrentUser(QString& userN);

    // Method to update lots value
    void updateLots();

    // Method to get lots value
    QList<QVariantMap> getLots();

    // Update scooterDetails and scooterIndex tables after an user rents a scooter
    void rentUnit(int numRows);

    QString filePath;
    void setFilePath(QString otherPath);

private slots:

    void on_toolButton_clicked();

    void on_comboBox_activated(int index);

    void on_checkBox_clicked();

    void on_checkBox_2_clicked();

    void on_checkBox_3_clicked();

    void on_checkBox_4_clicked();

    void on_checkBox_5_clicked();

    void on_buttonBox_rejected();

    void on_buttonBox_accepted();

private:
    Ui::rentScooter *ui;
    MainWindow *mainWindow;
    scooter_management *scootMgmt;
    QList<QVariantMap> lots; // Create a list to hold the lot info
    QList<QVariantMap> scooters; // Create a list to hold the scooter records

    QSqlDatabase db; // Database connection object

    bool guestAcc; // Whether current user is logged in.
    QString currentUser;
};

#endif // RENTSCOOTER_H
