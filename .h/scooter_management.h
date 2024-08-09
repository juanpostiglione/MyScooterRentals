#ifndef SCOOTER_MANAGEMENT_H
#define SCOOTER_MANAGEMENT_H

#include <QDialog>
#include <QtSql>  // Include Qt SQL module for database functionality
#include <QDebug> // Include Qt Debug module for debugging output
#include "mainwindow.h"
#include "ui_rentscooter.h"


namespace Ui {
class scooter_management;
}

class scooter_management : public QDialog
{
    Q_OBJECT

public:
    explicit scooter_management(QWidget *parent = nullptr);
    ~scooter_management();
    // Method to add a new scooter to the database
    void addScooter(const QString& available, const QString& nearLot, int lotDistance, int rentalRate, const QString& renter, const QString& status);

    // Method to update an existing scooter's details in the database
    bool updateScooter(int id, const QString& available, const QString& nearLot, int lotDistance, int rentalRate, const QString& renter, const QString& status);

    // Method to delete a scooter from the database
    void deleteScooter(int id);

    // Method to retrieve all scooters from the database
    QList<QVariantMap> getAllScooters();

    // Method to send a rental request
    bool requestRental(int scooterID, const QString& renter);

    // Method to approve a rental request
    bool approveRental(int scooterID, const QString& renter);

    // Method to reject a rental request
    bool rejectRental(int scooterID);

    // Method to set guestAcc
    void setGuest(bool isGuest);

    // Method to set *current
    void setCurrent(scooter_management* sPoint);

    // Method to set current user
    void setCurrentUser(QString& userN);

    // Method to return the scooter_id from the given lot, or -1 if no scooters are available
    int getAvailableFromLot(const QString& sLotName);

    // Method to get number of rentals for current user
    int getNumRented(QString& userN);

    QString filePath;
    void setFilePath(QString otherPath);

private slots:
    void on_toolButton_clicked();

    void on_comboBox_activated(int index);

    // Settings button
    void on_pushButton_clicked();

    // Log Out button
    void on_pushButton_3_clicked();

    // Rent a Scooter button
    void on_pushButton_2_clicked();

    // My Scooters button
    void on_pushButton_4_clicked();

private:
    Ui::scooter_management *ui;
    MainWindow *mainWindow;
    Ui::rentScooter *ui2;
    // Database connection object
    QSqlDatabase db;

    scooter_management *current; // Pointer to current instance of scooter_management
    bool guestAcc; // Whether current user is logged in.
    QString currentUser;

};

#endif // SCOOTER_MANAGEMENT_H
