#include "scooter_management.h"
#include "ui_scooter_management.h"
#include "ui_rentscooter.h"
#include "mainwindow.h"
#include "rentscooter.h"
#include "myscooters.h"
#include "accountmanagement.h"
#include "settingswindow.h"
#include <QPixmap>
#include <QAbstractItemView>

scooter_management::scooter_management(QWidget *parent)
    : QDialog(parent),
      ui(new Ui::scooter_management),
      mainWindow(nullptr)
{
    ui->setupUi(this);

    ui->comboBox->hide();

    // Set Window Color
    this->setStyleSheet("background-color: rgb(1, 68, 3);");
    ui->label_2->setStyleSheet("QLabel { color : rgb(255, 165, 0); }");
    ui->pushButton->setStyleSheet("QPushButton {" "color: white;" "background-color: grey;" "border-radius: 3px;" "padding: 3px;"
                                    "}" "QPushButton:hover {" "background-color: orange;""}");


    ui->pushButton_2->setStyleSheet("QPushButton {" "color: white;" "background-color: grey;" "border-radius: 3px;" "padding: 3px;"
                                    "}" "QPushButton:hover {" "background-color: orange;""}");

    ui->pushButton_3->setStyleSheet("QPushButton {" "color: white;" "background-color: grey;" "border-radius: 3px;" "padding: 3px;"
                                    "}" "QPushButton:hover {" "background-color: orange;""}");

    ui->pushButton_4->setStyleSheet("QPushButton {" "color: white;" "background-color: grey;" "border-radius: 3px;" "padding: 3px;"
                                    "}" "QPushButton:hover {" "background-color: orange;""}");

    ui->toolButton->setStyleSheet("QToolButton {" "color: white;" "background-color: grey;" "border-radius: 3px;" "padding: 3px;"
                                  "}" "QToolButton:hover {" "background-color: orange;""}");

    ui->frame->setStyleSheet("QFrame {" "background-color: black;}");

    ui->comboBox->setCurrentIndex(0);

    ui->comboBox->setStyleSheet("QComboBox QAbstractItemView { color: rgb(255, 255, 255); }");

    ui->label_3->setStyleSheet("QLabel { color : rgb(255, 255, 255); }");
    ui->label_5->setStyleSheet("QLabel { color : rgb(255, 255, 255); }");
    ui->label_6->setStyleSheet("QLabel { color : rgb(255, 255, 255); }");



}

scooter_management::~scooter_management()
{
    delete ui;
}

// Method to add a new scooter to the database
void scooter_management::addScooter(const QString& sAvailable, const QString& sNearLot, int sLotDistance, int sRentalRate, const QString& sRenter, const QString& sStatus)
{
    // Prepare an SQL query to insert a new scooter record
    QSqlQuery query;
    query.prepare("INSERT INTO scooterIndex (available, nearest_lot, lot_distance, rental_rate, renter, status) VALUES (:available, :nearest_lot, :lot_distance, :rental_rate, :renter, :status)");
    query.bindValue(":available", sAvailable); // Bind the available parameter
    query.bindValue(":nearest_lot", sNearLot); // Bind the nearest lot parameter
    query.bindValue(":lot_distance", sLotDistance); // Bind the lot distance parameter
    query.bindValue(":rental_rate", sRentalRate); // Bind the rental rate parameter
    query.bindValue(":renter", sRenter); // Bind the renter parameter
    query.bindValue(":status", sStatus); // Bind the status parameter

    if (!query.exec()) {
        qDebug() << "Error adding scooter: " << query.lastError().text();
    }
}

// Method to update an existing scooter's details in the database
bool scooter_management::updateScooter(int sID, const QString& sAvailable, const QString& sNearLot, int sLotDistance, int sRentalRate, const QString& sRenter, const QString& sStatus)
{
    // Prepare an SQL query to update a scooter record based on its ID
    QSqlQuery query;
    query.prepare("UPDATE scooterIndex SET available = :available, nearest_lot = :nearest_lot, lot_distance = :lot_distance, rental_rate = :rental_rate, renter = :renter, status = :status WHERE scooter_id = :id");
    query.bindValue(":available", sAvailable); // Bind the available parameter
    query.bindValue(":nearest_lot", sNearLot); // Bind the nearest lot parameter
    query.bindValue(":lot_distance", sLotDistance); // Bind the lot distance parameter
    query.bindValue(":rental_rate", sRentalRate); // Bind the rental rate parameter
    query.bindValue(":renter", sRenter); // Bind the renter parameter
    query.bindValue(":status", sStatus); // Bind the status parameter
    query.bindValue(":id", sID); // Bind the ID parameter

    if (!query.exec()) {
        qDebug() << "Error updating scooter: " << query.lastError().text();
        return false;
    }
    return true;
}

// Method to delete a scooter from the database
void scooter_management::deleteScooter(int sID) {
    // Prepare an SQL query to delete a scooter record based on its ID
    QSqlQuery query;
    query.prepare("DELETE FROM scooterIndex WHERE scooter_id = :id");
    query.bindValue(":id", sID); // Bind the ID parameter

    if (!query.exec()) {
        qDebug() << "Error deleting scooter: " << query.lastError().text();
    }
}

// Method to retrieve all scooters from the database
QList<QVariantMap> scooter_management::getAllScooters() {
    QList<QVariantMap> scooters; // Create a list to hold the scooter records
    QSqlQuery query("SELECT * FROM scooterIndex"); // Execute an SQL query to select all records from the scooterDetails table

    // Iterate over the result set
    while (query.next()) {
        QVariantMap scooter; // Create a map to hold the scooter details
        scooter["scooter_id"] = query.value("scooter_id"); // Retrieve and store the ID value
        scooter["available"] = query.value("available"); // Retrieve and store the available value
        scooter["nearest_lot"] = query.value("nearest_lot"); // Retrieve and store the nearest lot value
        scooter["lot_distance"] = query.value("lot_distance"); // Retrieve and store the lot distance value
        scooter["rental_rate"] = query.value("rental_rate"); // Retrieve and store the rental rate value
        scooter["renter"] = query.value("renter"); // Retrieve and store the renter value
        scooter["status"] = query.value("status"); // Retrieve and store the status value
        scooters.append(scooter); // Add the map to the list
    }

    return scooters; // Return the list of scooters
}

// Method to send a rental request
bool scooter_management::requestRental(int scooterID, const QString& renter)
{
    QSqlQuery query;
    query.prepare("UPDATE scooterIndex SET available = 'No', renter = :renter, status = 'Pending' WHERE scooter_id = :scooter_id");
    query.bindValue(":renter", renter);
    query.bindValue(":scooter_id", scooterID);
    return query.exec();
}

// Method to approve a rental request
bool scooter_management::approveRental(int scooterID, const QString& renter) {
    QSqlQuery query;
    query.prepare("UPDATE scooterIndex SET available = 'No', renter = :renter, status = 'Rented' WHERE scooter_id = :scooter_id");
    query.bindValue(":renter", renter);
    query.bindValue(":scooter_id", scooterID);
    //query.bindValue(":pending", "Pending");

    return query.exec();
}

// Method to reject a rental request
bool scooter_management::rejectRental(int scooterID) {
    QSqlQuery query;
    query.prepare("UPDATE scooterIndex SET available = 'Yes', renter = '', status = 'Available' WHERE scooter_id = :scooter_id");
    query.bindValue(":scooter_id", scooterID);
    //query.bindValue(":pending", "Pending");

    return query.exec();
}

// Method to set guestAcc
void scooter_management::setGuest(bool isGuest)
{
    guestAcc = isGuest;
}


// Method to set *current
void scooter_management::setCurrent(scooter_management* sPoint)
{
    current = sPoint;
}

// Method to set current user
void scooter_management::setCurrentUser(QString& userN)
{
    currentUser = userN;
}


// Method to return the scooter_id from the given lot, or -1 if no scooters are available
int scooter_management::getAvailableFromLot(const QString& sLotName)
{
    QList<QVariantMap> scooters; // Create a list to hold the scooter records
    QSqlQuery query;
    query.prepare("SELECT * FROM scooterIndex");
    query.exec();

    // Iterate over the result set
    while(query.next())
    {
        QVariantMap scooter; // Create a map to hold the scooter details
        scooter["scooter_id"] = query.value("scooter_id"); // Retrieve and store the ID value
        scooter["available"] = query.value("available"); // Retrieve and store the available value
        scooter["nearest_lot"] = query.value("nearest_lot"); // Retrieve and store the nearest lot value
        scooter["lot_distance"] = query.value("lot_distance"); // Retrieve and store the lot distance value
        scooter["rental_rate"] = query.value("rental_rate"); // Retrieve and store the rental rate value
        scooter["renter"] = query.value("renter"); // Retrieve and store the renter value
        scooter["status"] = query.value("status"); // Retrieve and store the status value
        scooters.append(scooter); // Add the map to the list
    }

    for(int index = 0; index < scooters.size(); index++)
    {
        if(scooters[index].value("available") == "Yes" && scooters[index].value("nearest_lot") == sLotName)
        {
            return scooters[index].value("scooter_id").toString().toInt();
        }
    }
    return -1;
}


// Method to get number of rentals for current user
int scooter_management::getNumRented(QString& userN)
{
    QList<QVariantMap> scooters; // Create a list to hold the scooter records
    QSqlQuery query;
    query.prepare("SELECT * FROM scooterIndex");
    query.exec();

    // Iterate over the result set
    while(query.next())
    {
        QVariantMap scooter; // Create a map to hold the scooter details
        scooter["scooter_id"] = query.value("scooter_id"); // Retrieve and store the ID value
        scooter["available"] = query.value("available"); // Retrieve and store the available value
        scooter["nearest_lot"] = query.value("nearest_lot"); // Retrieve and store the nearest lot value
        scooter["lot_distance"] = query.value("lot_distance"); // Retrieve and store the lot distance value
        scooter["rental_rate"] = query.value("rental_rate"); // Retrieve and store the rental rate value
        scooter["renter"] = query.value("renter"); // Retrieve and store the renter value
        scooter["status"] = query.value("status"); // Retrieve and store the status value
        scooters.append(scooter); // Add the map to the list
    }
    int count = 0;

    for(int index = 0; index < scooters.size(); index++)
    {
        if(scooters[index].value("available") == "No" && scooters[index].value("renter").toString() == userN)
        {
            count++;
        }
    }
    return count;
}


// When Menu button is clicked
void scooter_management::on_toolButton_clicked()
{
    ui->comboBox->show();
}

// When an index of the Menu is clicked
void scooter_management::on_comboBox_activated(int index)
{
    ui->comboBox->hide();
    mainWindow = new MainWindow(nullptr);

    rentScooter rentS;
    rentS.setGuest(guestAcc);
    rentS.setScooterMgmt(current);
    rentS.setCurrentUser(currentUser);
    rentS.setFilePath(filePath);

    myscooters myS;
    myS.setGuest(guestAcc);
    myS.setScooterMgmt(current);
    myS.setRScooter(&rentS);
    myS.setCurrentUser(currentUser);
    myS.updateDisplay();
    myS.setFilePath(filePath);

    accountmanagement a;
    a.setFilePath(filePath);

    settingsWindow settings;
    settings.setGuest(guestAcc);
    settings.setCurrentUser(currentUser);
    settings.setAccMgmt(&a);
    settings.setScooterMgmt(current);
    settings.setFilePath(filePath);

    // Switch windows from Menu
    switch(index)
    {

        // Go to Rent a Scooter window
        case 1:
            this->close();
            rentS.setModal(true);
            rentS.exec();
            break;

        // Go to My Scooters window
        case 2:
            this->close();
            myS.setModal(true);
            myS.exec();
            break;

        // Go to Settings window
        case 3:
            this->close();

            settings.setModal(true);
            settings.exec();
            break;

        // Go to log in Window
        case 4:
            this->close();
            rentS.close();
            mainWindow->show();
            mainWindow->resize(400,500);
            break;
    }
}


// When Settings button is pressed
void scooter_management::on_pushButton_clicked()
{
    // Go to settings window and close current window
    this->close();
    accountmanagement a;
    a.setFilePath(filePath);

    settingsWindow settings;
    settings.setGuest(guestAcc);
    settings.setCurrentUser(currentUser);
    settings.setAccMgmt(&a);
    settings.setScooterMgmt(current);
    settings.setFilePath(filePath);
    settings.setModal(true);
    settings.exec();
}


// When Log Out button is pressed
void scooter_management::on_pushButton_3_clicked()
{
    // Go to log in window and close current window
    mainWindow = new MainWindow(nullptr);
    mainWindow->show();
    this->close();
}

// When Rent a Scooter button is pressed
void scooter_management::on_pushButton_2_clicked()
{
    // Go to rent a scooter window and close current window
    this->close();
    rentScooter rent;
    rent.setGuest(guestAcc);
    rent.setScooterMgmt(current);
    rent.setCurrentUser(currentUser);
    rent.setFilePath(filePath);
    rent.setModal(true);
    rent.exec();

}

// When My Scooters button is pressed
void scooter_management::on_pushButton_4_clicked()
{
    // Go to My Scooters window and close current window
    this->close();

    rentScooter rentS;
    rentS.setGuest(guestAcc);
    rentS.setScooterMgmt(current);
    rentS.setCurrentUser(currentUser);
    rentS.setFilePath(filePath);

    myscooters myS;
    myS.setGuest(guestAcc);
    myS.setScooterMgmt(current);
    myS.setRScooter(&rentS);
    myS.setCurrentUser(currentUser);
    myS.updateDisplay();
    myS.setFilePath(filePath);
    myS.setModal(true);
    myS.exec();
}


void scooter_management::setFilePath(QString otherPath)
{
    filePath = otherPath;

    QPixmap picture(filePath + "/scooter.png");

    ui->label->setPixmap(picture);


    // Initialize the SQLite database connection
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(filePath + "/database_q.db");

    // Check if the database opens successfully
    if (!db.open()) {
        qDebug() << "Error: Could not open database."; // Output error message if the database fails to open
        return;
    }


    // Create a SQL query to create the scooterIndex table if it doesn't exist
    QSqlQuery query;
    query.prepare("CREATE TABLE IF NOT EXISTS scooterIndex ("
                  "scooter_id INTEGER PRIMARY KEY AUTOINCREMENT, "
                  "available TEXT, "
                  "nearest_lot TEXT, "
                  "lot_distance INTEGER, " // Distance to nearest lot in miles
                  "rental_rate INTEGER, " // In dollars (per hour)
                  "renter TEXT, "
                  "status TEXT)");
    query.exec();

    if(false)
    {
        // Add scooters if scooterIndex table is empty
        for(int i = 0; i < 20; i++)
        {
            addScooter("Yes", "LOT A", 0, 4, "", "Available");
            addScooter("Yes", "LOT B", 0, 4, "", "Available");
            addScooter("Yes", "LOT C", 0, 4, "", "Available");
            addScooter("Yes", "LOT D", 0, 4, "", "Available");
            addScooter("Yes", "LOT E", 0, 4, "", "Available");
        }
    }
}
