#include "rentscooter.h"
#include "ui_rentscooter.h"
#include "scooter_management.h"
#include "myscooters.h"
#include "settingswindow.h"
#include <QSqlQuery>
#include <QPixmap>
#include <QtSql>

rentScooter::rentScooter(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::rentScooter),
     mainWindow(nullptr)
{
    ui->setupUi(this);

    // Hide index Menu
    ui->comboBox->hide();
    ui->comboBox->setStyleSheet("QComboBox QAbstractItemView { color: rgb(255, 255, 255); }");

    // Hide button box
    ui->buttonBox->hide();

    // Hide message after rent
    ui->label_6->hide();

    // Set Scooter Image
    QPixmap image(filePath + "/scooter.png");
    ui->label->setPixmap(image);

    // Use images as emojis for check boxes
    ui->label_4->setPixmap(image);
    ui->label_10->setPixmap(image);
    ui->label_11->setPixmap(image);
    ui->label_12->setPixmap(image);
    ui->label_13->setPixmap(image);

    setFontAttributes();
    labelPosition();

    // Top label color
    ui->label_2->setStyleSheet("QLabel { color : rgb(255, 165, 0); }");

    ui->frame->setStyleSheet("QFrame {" "background-color: black;}");

    ui->toolButton->setStyleSheet("QToolButton {" "color: white;" "background-color: grey;"  "padding: 3px;"
                                  "}" "QToolButton:hover {" "background-color: orange;""}");

    ui->buttonBox->setStyleSheet("QPushButton{" "color: white;" "background-color: grey;"
                                 "}" "QPushButton:hover {" "background-color: orange;""}");

    // Set Window Color
    this->setStyleSheet("background-color: rgb(1, 68, 3);");

    // Check the Index Menu
    ui->comboBox->setCurrentIndex(1);

    // Make labels white
    ui->label_3->setStyleSheet("QLabel { color : rgb(255, 255, 255); }");
    ui->label_5->setStyleSheet("QLabel { color : rgb(255, 255, 255); }");
    ui->label_6->setStyleSheet("QLabel { color : rgb(255, 255, 255); }");
    ui->label_20->setStyleSheet("QLabel { color : rgb(255, 255, 255); }");
    ui->label_21->setStyleSheet("QLabel { color : rgb(255, 255, 255); }");
    ui->label_22->setStyleSheet("QLabel { color : rgb(255, 255, 255); }");
    ui->label_23->setStyleSheet("QLabel { color : rgb(255, 255, 255); }");
    ui->label_24->setStyleSheet("QLabel { color : rgb(255, 255, 255); }");
}

rentScooter::~rentScooter()
{
    delete ui;
}

// Set the font attributes of labels
void rentScooter::setFontAttributes()
{
    //QFont font = ui->label_20->font();
    ui->label_20->setPalette(Qt::white);
    ui->label_21->setPalette(Qt::white);
    ui->label_22->setPalette(Qt::white);
    ui->label_23->setPalette(Qt::white);
    ui->label_24->setPalette(Qt::white);
}

// Set the Labels Position
void rentScooter::labelPosition()
{
    // Get Width and Height for Labels
    int labelWidth = ui->label_20->width();
    int labelHeight = ui->label_20->height();

    // Get the Center of the Window
    int centerX = (this->height() - labelWidth) / 2;
    int centerY = (this->width() - labelHeight) / 2;

    // Labels Position
    ui->label_20->move(centerX+150, centerY-148);
    ui->label_21->move(centerX+150, centerY-102);
    ui->label_22->move(centerX+150, centerY-56);
    ui->label_23->move(centerX+150, centerY-10);
    ui->label_24->move(centerX+150, centerY+36);
}

// Method to set guestAcc
void rentScooter::setGuest(bool isGuest)
{
    guestAcc = isGuest;
}

// Method to set scooter_management pointer
void rentScooter::setScooterMgmt(scooter_management* s)
{
    scootMgmt = s;
}

// Method to set current user
void rentScooter::setCurrentUser(QString& userN)
{
    currentUser = userN;
}


// Method to update lots value
void rentScooter::updateLots()
{
    QSqlQuery query("SELECT * FROM scooterDetails"); // Execute an SQL query to select all records from the scooterDetails table
    lots.clear();

    // Iterate over the result set
    while (query.next())
    {
        QVariantMap lot; // Create a map to hold the scooter details
        lot["lot_id"] = query.value("lot_id"); // Retrieve and store the ID value
        lot["status"] = query.value("status"); // Retrieve and store the availability value
        lot["location"] = query.value("location"); // Retrieve and store the location value
        lot["units"] = query.value("units"); // Retrieve and store the pricing value
        lots.append(lot); // Add the map to the list
    }
    query.finish();
}

// Method to get lots value
QList<QVariantMap> rentScooter::getLots()
{
    updateLots();
    return lots;
}


// Update scooterDetails and scooterIndex tables after an user rents a scooter
void rentScooter::rentUnit(int numRows)
{
    if(!guestAcc)
    {
        // Check if user is under limit of 5 rentals at the same time
        if(scootMgmt->getNumRented(currentUser) < 5)
        {
            if (numRows < lots.size()) {
                QVariantMap selectedRow = lots.at(numRows);
                QString lotID = selectedRow.value("lot_id").toString();
                QString lotName = selectedRow.value("location").toString();
                int currentUnits = selectedRow.value("units").toInt();

                if (currentUnits > 0) {
                    // Decrement units by 1
                    currentUnits--;
                    QSqlQuery query;

                    int availableID = scootMgmt->getAvailableFromLot(lotName);

                    if(availableID != -1)
                    {
                        scootMgmt->requestRental(availableID, currentUser);

                        if(currentUnits != 0)
                        {
                            // Update the database
                            query.prepare("UPDATE scooterDetails SET units = :units WHERE lot_id = :lot_id");
                            query.bindValue(":units", currentUnits);
                            query.bindValue(":lot_id", lotID);
                        }
                        else
                        {
                            // Update the database and change lot availability
                            query.prepare("UPDATE scooterDetails SET units = :units, status = :status WHERE lot_id = :lot_id");
                            query.bindValue(":units", currentUnits);
                            query.bindValue(":status", "Not Available");
                            query.bindValue(":lot_id", lotID);
                        }

                        if (query.exec()) {
                            qDebug() << "Units updated successfully for lot_id:" << lotID;
                            ui->label_6->setText("Scooter rented, go to My Scooters to see your details");

                            updateLots();

                            // If the list is not empty
                            if(!lots.empty())
                            {
                                // Access to the first row of scooterDetails
                                QVariantMap firstRow = lots.at(0);
                                QString ID = firstRow.value("lot_id").toString();
                                QString Status = firstRow.value("status").toString();
                                QString Location = firstRow.value("location").toString();
                                QString Units = firstRow.value("units").toString();

                                QString text = "LOT ID: " + ID + " | Status: " + Status + " | Location: " + Location + " | Units: " + Units;

                                // Show data in check box
                                ui->label_20->setText(text);
                                ui->label_20->show();
                            }

                            if(lots.size() > 1)
                            {
                                // Access to the second row of scooterDetails
                                QVariantMap secondRow = lots.at(1);
                                QString ID = secondRow.value("lot_id").toString();
                                QString Status = secondRow.value("status").toString();
                                QString Location = secondRow.value("location").toString();
                                QString Units = secondRow.value("units").toString();

                                QString text = "LOT ID: " + ID + " | Status: " + Status + " | Location: " + Location + " | Units: " + Units;

                                // Show data in check box
                                ui->label_21->setText(text);
                                ui->label_21->show();
                            }

                            if(lots.size() > 2)
                            {
                                // Access to the third row of scooterDetails
                                QVariantMap thirdRow = lots.at(2);
                                QString ID = thirdRow.value("lot_id").toString();
                                QString Status = thirdRow.value("status").toString();
                                QString Location = thirdRow.value("location").toString();
                                QString Units = thirdRow.value("units").toString();

                                QString text = "LOT ID: " + ID + " | Status: " + Status + " | Location: " + Location + " | Units: " + Units;

                                // Show data in check box
                                ui->label_22->setText(text);
                                ui->label_22->show();
                            }

                            if(lots.size() > 3)
                            {
                                // Access to the fourth row of scooterDetails
                                QVariantMap fourthRow = lots.at(3);
                                QString ID = fourthRow.value("lot_id").toString();
                                QString Status = fourthRow.value("status").toString();
                                QString Location = fourthRow.value("location").toString();
                                QString Units = fourthRow.value("units").toString();

                                QString text = "LOT ID: " + ID + " | Status: " + Status + " | Location: " + Location + " | Units: " + Units;

                                // Show data in check box
                                ui->label_23->setText(text);
                                ui->label_23->show();
                            }

                            if(lots.size() > 4)
                            {
                                // Access to the fifth row of scooterDetails
                                QVariantMap fifthRow = lots.at(4);
                                QString ID = fifthRow.value("lot_id").toString();
                                QString Status = fifthRow.value("status").toString();
                                QString Location = fifthRow.value("location").toString();
                                QString Units = fifthRow.value("units").toString();

                                QString text = "LOT ID: " + ID + " | Status: " + Status + " | Location: " + Location + " | Units: " + Units;

                                // Show data in check box
                                ui->label_24->setText(text);
                                ui->label_24->show();
                            }

                        } else {
                            qDebug() << "Error updating units:" << query.lastError().text();
                            ui->label_6->setText("There was an error with your rental request");
                        }
                    }
                    else
                    {
                        qDebug() << "Error updating units:" << query.lastError().text();
                        ui->label_6->setText("There was an error with your rental request");
                    }
                } else {
                    qDebug() << "No units available for lot_id:" << lotID;
                    ui->label_6->setText("No units available for this lot");
                }
            }
        }
        else
        {
            qDebug() << "Max rentals reached for user " << currentUser;
            ui->label_6->setText("You may not have more than 5 rentals at a time");
        }
    }
    else
    {
        ui->label_6->setText("You must be signed in to place a rental request");
    }
}


// Show Menu when tool button is clicked
void rentScooter::on_toolButton_clicked()
{
    ui->comboBox->show();
}

// Switch windows from the Menu
void rentScooter::on_comboBox_activated(int index)
{
    ui->comboBox->hide();
    scooter_management window;
    mainWindow = new MainWindow(nullptr);
    window.setCurrentUser(currentUser);
    window.setCurrent(scootMgmt);
    window.setGuest(guestAcc);
    window.setFilePath(filePath);

    rentScooter rentS;
    rentS.setGuest(guestAcc);
    rentS.setScooterMgmt(scootMgmt);
    rentS.setCurrentUser(currentUser);
    rentS.setFilePath(filePath);

    myscooters myS;
    myS.setGuest(guestAcc);
    myS.setScooterMgmt(scootMgmt);
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
    settings.setScooterMgmt(scootMgmt);
    settings.setFilePath(filePath);

    switch(index)
    {
        // Go to Home window
        case 0:
            this->close();
            a.close();
            settings.close();
            myS.close();
            rentS.close();
            window.setModal(true);
            window.exec();
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
            window.close();
            mainWindow->show();
            mainWindow->resize(400,500);
            break;
    }
}


// When first check box is clicked
void rentScooter::on_checkBox_clicked()
{
    // Hide or show button box depending if the check bx is checked or not
    ui->buttonBox->show();

    if(!ui->checkBox->isChecked())
    {
        ui->buttonBox->hide();
    }
}

// When second check box is clicked
void rentScooter::on_checkBox_2_clicked()
{
    // Hide or show button box depending if the check bx is checked or not
    ui->buttonBox->show();

    if(!ui->checkBox_2->isChecked())
    {
        ui->buttonBox->hide();
    }
}

// When third check box is clicked
void rentScooter::on_checkBox_3_clicked()
{
    // Hide or show button box depending if the check bx is checked or not
    ui->buttonBox->show();

    if(!ui->checkBox_3->isChecked())
    {
        ui->buttonBox->hide();
    }
}

// When fourth check box is clicked
void rentScooter::on_checkBox_4_clicked()
{
    // Hide or show button box depending if the check bx is checked or not
    ui->buttonBox->show();

    if(!ui->checkBox_4->isChecked())
    {
        ui->buttonBox->hide();
    }
}

// When fifth check box is clicked
void rentScooter::on_checkBox_5_clicked()
{
    // Hide or show button box depending if the check bx is checked or not
    ui->buttonBox->show();

    if(!ui->checkBox_5->isChecked())
    {
        ui->buttonBox->hide();
    }
}

// When Cancel button is clicked
void rentScooter::on_buttonBox_rejected()
{
    ui->checkBox->setChecked(false);
    ui->checkBox_2->setChecked(false);
    ui->checkBox_3->setChecked(false);
    ui->checkBox_4->setChecked(false);
    ui->checkBox_5->setChecked(false);
    ui->buttonBox->hide();
}

// When OK button is clicked
void rentScooter::on_buttonBox_accepted()
{
    bool checked = ui->checkBox->isChecked();
    bool checked2 = ui->checkBox_2->isChecked();
    bool checked3 = ui->checkBox_3->isChecked();
    bool checked4 = ui->checkBox_4->isChecked();
    bool checked5 = ui->checkBox_5->isChecked();

    // If first row is checked and click OK update scooterDetails
    if(checked && !checked2 && !checked3 && !checked4 && !checked5)
    {
        rentUnit(0);
    }
    else if(!checked && checked2 && !checked3 && !checked4 && !checked5)
    {
        // If second row is checked and click OK update scooterDetails
        rentUnit(1);
    }
    else if(!checked && !checked2 && checked3 && !checked4 && !checked5)
    {
        // If third row is checked and click OK update scooterDetails
        rentUnit(2);
    }
    else if(!checked && !checked2 && !checked3 && checked4 && !checked5)
    {
        // If fourth row is checked and click OK update scooterDetails
        rentUnit(3);
    }
    else if(!checked && !checked2 && !checked3 && !checked4 && checked5)
    {
        // If fifth row is checked and click OK update scooterDetails
        rentUnit(4);
    }
    else if(!checked && !checked2 && !checked3 && !checked4 && !checked5)
    {
        ui->label_6->setText("No lots selected");
        ui->label_6->show();
    }
    else
    {
        ui->label_6->setText("Please select only 1 lot");
        ui->label_6->show();
    }

    // Uncheck all checkboxes after updating the units
    ui->checkBox->setChecked(false);
    ui->checkBox_2->setChecked(false);
    ui->checkBox_3->setChecked(false);
    ui->checkBox_4->setChecked(false);
    ui->checkBox_5->setChecked(false);

    // Hide the button box after the action
    ui->buttonBox->hide();

    // Show message after rent
    ui->label_6->show();
}


void rentScooter::setFilePath(QString otherPath)
{
    filePath = otherPath;
    QPixmap image(filePath + "/scooter.png");
    ui->label->setPixmap(image);

    // Set Database
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(filePath + "/database_q.db");

    if (!db.open()) {
        qDebug() << "Error: Could not open database." << db.lastError();
        return; // Exit the function if the database cannot be opened
    }


    scooters = scootMgmt->getAllScooters();
    int numLotA = 0;
    int numLotB = 0;
    int numLotC = 0;
    int numLotD = 0;
    int numLotE = 0;

    if(!scooters.empty())
    {
        for(int j = 0; j < scooters.size(); j++)
        {
            if(scooters[j]["available"] == "Yes" && scooters[j]["lot_distance"] == 0)
            {
                if(scooters[j]["nearest_lot"] == "LOT A")
                {
                    numLotA++;
                }
                else if(scooters[j]["nearest_lot"] == "LOT B")
                {
                    numLotB++;
                }
                else if(scooters[j]["nearest_lot"] == "LOT C")
                {
                    numLotC++;
                }
                else if(scooters[j]["nearest_lot"] == "LOT D")
                {
                    numLotD++;
                }
                else if(scooters[j]["nearest_lot"] == "LOT E")
                {
                    numLotE++;
                }
            }
        }

        // Add information from scooterIndex to scooterDetails
        QSqlQuery q1;
        q1.prepare("UPDATE scooterDetails SET units = :numA, status = :status WHERE lot_id = :lotA");
        q1.bindValue(":numA", numLotA);
        q1.bindValue(":status", "Available");
        q1.bindValue(":lotA", "UF1");

        QSqlQuery q2;
        q2.prepare("UPDATE scooterDetails SET units = :numB, status = :status WHERE lot_id = :lotB");
        q2.bindValue(":numB", numLotB);
        q2.bindValue(":status", "Available");
        q2.bindValue(":lotB", "UF2");

        QSqlQuery q3;
        q3.prepare("UPDATE scooterDetails SET units = :numC, status = :status WHERE lot_id = :lotC");
        q3.bindValue(":numC", numLotC);
        q3.bindValue(":status", "Available");
        q3.bindValue(":lotC", "UF3");

        QSqlQuery q4;
        q4.prepare("UPDATE scooterDetails SET units = :numD, status = :status WHERE lot_id = :lotD");
        q4.bindValue(":numD", numLotD);
        q4.bindValue(":status", "Available");
        q4.bindValue(":lotD", "UF4");

        QSqlQuery q5;
        q5.prepare("UPDATE scooterDetails SET units = :numE, status = :status WHERE lot_id = :lotE");
        q5.bindValue(":numE", numLotE);
        q5.bindValue(":status", "Available");
        q5.bindValue(":lotE", "UF5");

        q1.exec();
        q2.exec();
        q3.exec();
        q4.exec();
        q5.exec();

        q1.finish();
        q2.finish();
        q3.finish();
        q4.finish();
        q5.finish();

        // Set lot to "Not Available" if no scooters are available at that lot
        QSqlQuery qAvailable;
        qAvailable.prepare("UPDATE scooterDetails SET status = :statusBool WHERE units = :numUnits");
        qAvailable.bindValue(":statusBool", "Not Available");
        qAvailable.bindValue(":numUnits", 0);
        qAvailable.exec();
        qAvailable.finish();
    }

    updateLots();

    // If the list is not empty
    if(!lots.empty())
    {
        // Access to the first row of scooterDetails
        QVariantMap firstRow = lots.at(0);
        QString ID = firstRow.value("lot_id").toString();
        QString Status = firstRow.value("status").toString();
        QString Location = firstRow.value("location").toString();
        QString Units = firstRow.value("units").toString();

        QString text = "LOT ID: " + ID + " | Status: " + Status + " | Location: " + Location + " | Units: " + Units;

        // Show data in check box
        ui->label_20->setText(text);
        ui->label_20->show();
    }

    if(lots.size() > 1)
    {
        // Access to the second row of scooterDetails
        QVariantMap secondRow = lots.at(1);
        QString ID = secondRow.value("lot_id").toString();
        QString Status = secondRow.value("status").toString();
        QString Location = secondRow.value("location").toString();
        QString Units = secondRow.value("units").toString();

        QString text = "LOT ID: " + ID + " | Status: " + Status + " | Location: " + Location + " | Units: " + Units;

        // Show data in check box
        ui->label_21->setText(text);
        ui->label_21->show();
    }

    if(lots.size() > 2)
    {
        // Access to the third row of scooterDetails
        QVariantMap thirdRow = lots.at(2);
        QString ID = thirdRow.value("lot_id").toString();
        QString Status = thirdRow.value("status").toString();
        QString Location = thirdRow.value("location").toString();
        QString Units = thirdRow.value("units").toString();

        QString text = "LOT ID: " + ID + " | Status: " + Status + " | Location: " + Location + " | Units: " + Units;

        // Show data in check box
        ui->label_22->setText(text);
        ui->label_22->show();
    }

    if(lots.size() > 3)
    {
        // Access to the fourth row of scooterDetails
        QVariantMap fourthRow = lots.at(3);
        QString ID = fourthRow.value("lot_id").toString();
        QString Status = fourthRow.value("status").toString();
        QString Location = fourthRow.value("location").toString();
        QString Units = fourthRow.value("units").toString();

        QString text = "LOT ID: " + ID + " | Status: " + Status + " | Location: " + Location + " | Units: " + Units;

        // Show data in check box
        ui->label_23->setText(text);
        ui->label_23->show();
    }

    if(lots.size() > 4)
    {
        // Access to the fifth row of scooterDetails
        QVariantMap fifthRow = lots.at(4);
        QString ID = fifthRow.value("lot_id").toString();
        QString Status = fifthRow.value("status").toString();
        QString Location = fifthRow.value("location").toString();
        QString Units = fifthRow.value("units").toString();

        QString text = "LOT ID: " + ID + " | Status: " + Status + " | Location: " + Location + " | Units: " + Units;

        // Show data in check box
        ui->label_24->setText(text);
        ui->label_24->show();
    }
}
