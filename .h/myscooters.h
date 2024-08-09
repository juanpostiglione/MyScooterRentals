#ifndef MYSCOOTERS_H
#define MYSCOOTERS_H

#include <QDialog>
#include "mainwindow.h"
#include "scooter_management.h"
#include "rentscooter.h"
#include <QDebug>
#include <QtSql>
#include <QFileInfo>

namespace Ui {
class myscooters;
}

class myscooters : public QDialog
{
    Q_OBJECT

public:
    explicit myscooters(QWidget *parent = nullptr);
    ~myscooters();

    // Set the font attributes of labels
    void setFontAttributes();

    // Set the Labels Position
    void labelPosition();

    // Method to set guestRent
    void setGuest(bool isGuest);

    // Method to set scooter_management pointer
    void setScooterMgmt(scooter_management* s);

    // Method to set rentScooter pointer
    void setRScooter(rentScooter* r);

    // Method to set current user
    void setCurrentUser(QString& userN);

    // Update scooterDetails and scooterIndex tables after an user returns a scooter
    void returnUnit(int scootID);

    // Display updated My Scooter info
    void updateDisplay();

    QString filePath;
    void setFilePath(QString otherPath);

private slots:

    void on_toolButton_2_clicked();

    void on_comboBox_2_activated(int index);

    void on_checkBox_6_clicked();

    void on_checkBox_7_clicked();

    void on_checkBox_8_clicked();

    void on_checkBox_9_clicked();

    void on_checkBox_10_clicked();

    void on_buttonBox_2_rejected();

    void on_buttonBox_2_accepted();

private:
    Ui::myscooters *ui;

    MainWindow *mainWindow;
    scooter_management *scootMgmt;
    rentScooter *rScooter;

    QList<QVariantMap> lots; // Create a list to hold the lot info
    QList<QVariantMap> scooters; // Create a list to hold the scooter records
    QSqlDatabase db; // Database connection object

    bool guestAcc; // Whether current user is logged in.
    QString currentUser;

    QString text30;
    QString text31;
    QString text32;
    QString text33;
    QString text34;
};

#endif // MYSCOOTERS_H
