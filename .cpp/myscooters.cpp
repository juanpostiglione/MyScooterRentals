#include "myscooters.h"
#include "ui_myscooters.h"
#include "scooter_management.h"
#include "rentscooter.h"
#include "settingswindow.h"
#include <QSqlQuery>
#include <QPixmap>
#include <QtSql>

myscooters::myscooters(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::myscooters)
{
    ui->setupUi(this);

    // Hide index Menu
    ui->comboBox_2->hide();
    ui->comboBox_2->setStyleSheet("QComboBox QAbstractItemView { color: rgb(255, 255, 255); }");

    // Hide button box
    ui->buttonBox_2->hide();

    // Hide message after rent
    ui->label_6->hide();

    // Set Scooter Image
    QPixmap image(filePath + "/scooter.png");
    ui->label_5->setPixmap(image);

    // Use images as emojis for check boxes
    ui->label_14->setPixmap(image);
    ui->label_15->setPixmap(image);
    ui->label_16->setPixmap(image);
    ui->label_17->setPixmap(image);
    ui->label_18->setPixmap(image);

    setFontAttributes();
    labelPosition();


    // Top label color
    ui->label_3->setStyleSheet("QLabel { color : rgb(255, 165, 0); }");

    ui->frame->setStyleSheet("QFrame {" "background-color: black;}");

    ui->toolButton_2->setStyleSheet("QToolButton {" "color: white;" "background-color: grey;"  "padding: 3px;"
                                  "}" "QToolButton:hover {" "background-color: orange;""}");

    ui->buttonBox_2->setStyleSheet("QPushButton{" "color: white;" "background-color: grey;"
                                 "}" "QPushButton:hover {" "background-color: orange;""}");

    // Set Window Color
    this->setStyleSheet("background-color: rgb(1, 68, 3);");

    // Check the Index Menu
    ui->comboBox_2->setCurrentIndex(2);

    // Make labels white
    ui->label_6->setStyleSheet("QLabel { color : rgb(255, 255, 255); }");
    ui->label_7->setStyleSheet("QLabel { color : rgb(255, 255, 255); }");
    ui->label_9->setStyleSheet("QLabel { color : rgb(255, 255, 255); }");
    ui->label_30->setStyleSheet("QLabel { color : rgb(255, 255, 255); }");
    ui->label_31->setStyleSheet("QLabel { color : rgb(255, 255, 255); }");
    ui->label_32->setStyleSheet("QLabel { color : rgb(255, 255, 255); }");
    ui->label_33->setStyleSheet("QLabel { color : rgb(255, 255, 255); }");
    ui->label_34->setStyleSheet("QLabel { color : rgb(255, 255, 255); }");
}

myscooters::~myscooters()
{
    delete ui;
}

// Set the font attributes of labels
void myscooters::setFontAttributes()
{
    //QFont font = ui->label_20->font();
    ui->label_30->setPalette(Qt::white);
    ui->label_31->setPalette(Qt::white);
    ui->label_32->setPalette(Qt::white);
    ui->label_33->setPalette(Qt::white);
    ui->label_34->setPalette(Qt::white);
}

// Set the Labels Position
void myscooters::labelPosition()
{
    // Get Width and Height for Labels
    int labelWidth = ui->label_30->width();
    int labelHeight = ui->label_30->height();

    // Get the Center of the Window
    int centerX = (this->height() - labelWidth) / 2;
    int centerY = (this->width() - labelHeight) / 2;

    // Labels Position
    ui->label_30->move(centerX+187, centerY-148);
    ui->label_31->move(centerX+187, centerY-102);
    ui->label_32->move(centerX+187, centerY-56);
    ui->label_33->move(centerX+187, centerY-10);
    ui->label_34->move(centerX+187, centerY+36);
}

// Method to set guestAcc
void myscooters::setGuest(bool isGuest)
{
    guestAcc = isGuest;
}

// Method to set scooter_management pointer
void myscooters::setScooterMgmt(scooter_management* s)
{
    scootMgmt = s;
}

// Method to set rentScooter pointer
void myscooters::setRScooter(rentScooter* r)
{
    rScooter = r;
}

// Method to set current user
void myscooters::setCurrentUser(QString& userN)
{
    currentUser = userN;
}


// Update scooterDetails and scooterIndex tables after an user returns a scooter
void myscooters::returnUnit(int scootID)
{
    bool found = false;
    int currentUnits = 0;
    QString lotName;
    int distance = 0;
    
    if(!guestAcc)
    {
        scooters = scootMgmt->getAllScooters();
        
        // Search for lot name for given scooter id
        for(int i = 0; i < scooters.size(); i++)
        {
            if(scooters[i].value("scooter_id").toString().toInt() == scootID)
            {
                lotName = scooters[i].value("nearest_lot").toString();
                distance = scooters[i].value("lot_distance").toString().toInt();
                found = true;
                break;
            }
        }
        
        if (found)
        {
            // Get currentUnits of lot
            for(int j = 0; j < scooters.size(); j++)
            {
                if(scooters[j].value("nearest_lot").toString() == lotName && scooters[j].value("available").toString() == "Yes")
                {
                    currentUnits++;
                }
            }
            currentUnits++; // Add current scooter to number of available scooters

            // Set scooter to be available for rentals
            if(scootMgmt->updateScooter(scootID, "Yes", lotName, distance, 4, "", "Available"))
            {
                // Update the database and change lot availability
                QSqlQuery query;
                query.prepare("UPDATE scooterDetails SET units = :units, status = :status WHERE location = :location");
                query.bindValue(":units", currentUnits);
                query.bindValue(":status", "Available");
                query.bindValue(":location", lotName);

                if (query.exec())
                {
                    qDebug() << "Units updated successfully for location:" << lotName;
                    ui->label_6->setText("Scooter successfully returned!");
                }
                else
                {
                    qDebug() << "Error updating units:" << query.lastError().text();
                    ui->label_6->setText("There was an error with your rental request");
                }

                updateDisplay();
            }
            else
            {
                ui->label_6->setText("There was an error with your return");
            }
        }
        else
        {
            qDebug() << "no scooter matching ID";
            ui->label_6->setText("There was an error with your return");
        }
    }
    else
    {
        ui->label_6->setText("You must be signed in to view your scooters");
    }
}


// Display updated My Scooter info
void myscooters::updateDisplay()
{
    lots = rScooter->getLots();
    scooters = scootMgmt->getAllScooters();

    ui->label_9->hide();

    // Create default UI elements
    text30 = "Rental Slot Available";
    text31 = "Rental Slot Available";
    text32 = "Rental Slot Available";
    text33 = "Rental Slot Available";
    text34 = "Rental Slot Available";
    ui->label_30->setText(text30);
    ui->label_31->setText(text31);
    ui->label_32->setText(text32);
    ui->label_33->setText(text33);
    ui->label_34->setText(text34);

    int index1 = -1;
    int index2 = -1;
    int index3 = -1;
    int index4 = -1;
    int index5 = -1;

    //Display scooter info if user is logged in
    if(!guestAcc)
    {
        ui->label_9->setText("Select a Scooter to Return:");
        ui->label_9->show();

        for(int i = 0; i < scooters.size(); i++)
        {
            QString userN = scooters[i].value("renter").toString();

            if(userN == currentUser)
            {
                if(index1 == -1)
                {
                    index1 = i;

                    QString ID = scooters[i].value("scooter_id").toString();
                    QString nearLot = scooters[i].value("nearest_lot").toString();
                    QString lotDistance = scooters[i].value("lot_distance").toString();
                    QString rentRate = scooters[i].value("rental_rate").toString();
                    QString sStatus = scooters[i].value("status").toString();
                    
                    text30 = ID;
                    QString text = "ID: " + ID + " | Closest Lot: " + nearLot + " | Distance from Lot: "
                                   + lotDistance + " miles | Rental Rate: $" + rentRate + " per hour | " + sStatus;

                    // Display data
                    ui->label_14->show();
                    ui->checkBox_6->show();
                    ui->label_30->setText(text);
                    ui->label_30->show();

                }
                else if(index2 == -1)
                {
                    index2 = i;

                    QString ID = scooters[i].value("scooter_id").toString();
                    QString nearLot = scooters[i].value("nearest_lot").toString();
                    QString lotDistance = scooters[i].value("lot_distance").toString();
                    QString rentRate = scooters[i].value("rental_rate").toString();
                    QString sStatus = scooters[i].value("status").toString();

                    text31 = ID;
                    QString text = "ID: " + ID + " | Closest Lot: " + nearLot + " | Distance from Lot: "
                                   + lotDistance + " miles | Rental Rate: $" + rentRate + " per hour | " + sStatus;

                    // Display data
                    ui->label_15->show();
                    ui->checkBox_7->show();
                    ui->label_31->setText(text);
                    ui->label_31->show();
                }
                else if(index3 == -1)
                {
                    index3 = i;

                    QString ID = scooters[i].value("scooter_id").toString();
                    QString nearLot = scooters[i].value("nearest_lot").toString();
                    QString lotDistance = scooters[i].value("lot_distance").toString();
                    QString rentRate = scooters[i].value("rental_rate").toString();
                    QString sStatus = scooters[i].value("status").toString();
                    
                    text32 = ID;
                    QString text = "ID: " + ID + " | Closest Lot: " + nearLot + " | Distance from Lot: "
                                   + lotDistance + " miles | Rental Rate: $" + rentRate + " per hour | " + sStatus;

                    // Display data
                    ui->label_16->show();
                    ui->checkBox_8->show();
                    ui->label_32->setText(text);
                    ui->label_32->show();
                }
                else if(index4 == -1)
                {
                    index4 = i;

                    QString ID = scooters[i].value("scooter_id").toString();
                    QString nearLot = scooters[i].value("nearest_lot").toString();
                    QString lotDistance = scooters[i].value("lot_distance").toString();
                    QString rentRate = scooters[i].value("rental_rate").toString();
                    QString sStatus = scooters[i].value("status").toString();

                    text33 = ID;
                    QString text = "ID: " + ID + " | Closest Lot: " + nearLot + " | Distance from Lot: "
                                   + lotDistance + " miles | Rental Rate: $" + rentRate + " per hour | " + sStatus;

                    // Display data
                    ui->label_17->show();
                    ui->checkBox_9->show();
                    ui->label_33->setText(text);
                    ui->label_33->show();
                }
                else if(index5 == -1)
                {
                    index5 = i;

                    QString ID = scooters[i].value("scooter_id").toString();
                    QString nearLot = scooters[i].value("nearest_lot").toString();
                    QString lotDistance = scooters[i].value("lot_distance").toString();
                    QString rentRate = scooters[i].value("rental_rate").toString();
                    QString sStatus = scooters[i].value("status").toString();

                    text34 = ID;
                    QString text = "ID: " + ID + " | Closest Lot: " + nearLot + " | Distance from Lot: "
                                   + lotDistance + " miles | Rental Rate: $" + rentRate + " per hour | " + sStatus;

                    // Display data
                    ui->label_18->show();
                    ui->checkBox_10->show();
                    ui->label_34->setText(text);
                    ui->label_34->show();
                }
            }
        }
    }
    else
    {
        ui->label_9->setText("Sign in to View your Scooters");
        ui->label_9->show();
    }

    ui->label_30->show();
    ui->label_31->show();
    ui->label_32->show();
    ui->label_33->show();
    ui->label_34->show();
}


// Show Menu when tool button is clicked
void myscooters::on_toolButton_2_clicked()
{
    ui->comboBox_2->show();
}

// Switch windows from the Menu
void myscooters::on_comboBox_2_activated(int index)
{
    ui->comboBox_2->hide();
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
    myS.setCurrentUser(currentUser);
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
        window.setModal(true);
        window.exec();
        break;

        // Go to Rent a Scooter window
    case 1:
        this->close();
        rentS.setModal(true);
        rentS.exec();
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
void myscooters::on_checkBox_6_clicked()
{
    // Hide or show button box depending if the check bx is checked or not
    ui->buttonBox_2->show();

    if(!ui->checkBox_6->isChecked())
    {
        ui->buttonBox_2->hide();
    }
}

// When second check box is clicked
void myscooters::on_checkBox_7_clicked()
{
    // Hide or show button box depending if the check bx is checked or not
    ui->buttonBox_2->show();

    if(!ui->checkBox_7->isChecked())
    {
        ui->buttonBox_2->hide();
    }
}

// When third check box is clicked
void myscooters::on_checkBox_8_clicked()
{
    // Hide or show button box depending if the check bx is checked or not
    ui->buttonBox_2->show();

    if(!ui->checkBox_8->isChecked())
    {
        ui->buttonBox_2->hide();
    }
}

// When fourth check box is clicked
void myscooters::on_checkBox_9_clicked()
{
    // Hide or show button box depending if the check bx is checked or not
    ui->buttonBox_2->show();

    if(!ui->checkBox_9->isChecked())
    {
        ui->buttonBox_2->hide();
    }
}

// When fifth check box is clicked
void myscooters::on_checkBox_10_clicked()
{
    // Hide or show button box depending if the check bx is checked or not
    ui->buttonBox_2->show();

    if(!ui->checkBox_10->isChecked())
    {
        ui->buttonBox_2->hide();
    }
}

// When Cancel button is clicked
void myscooters::on_buttonBox_2_rejected()
{
    ui->checkBox_6->setChecked(false);
    ui->checkBox_7->setChecked(false);
    ui->checkBox_8->setChecked(false);
    ui->checkBox_9->setChecked(false);
    ui->checkBox_10->setChecked(false);
    ui->buttonBox_2->hide();
}

// When OK button is clicked
void myscooters::on_buttonBox_2_accepted()
{
    bool checked = ui->checkBox_6->isChecked();
    bool checked2 = ui->checkBox_7->isChecked();
    bool checked3 = ui->checkBox_8->isChecked();
    bool checked4 = ui->checkBox_9->isChecked();
    bool checked5 = ui->checkBox_10->isChecked();

    // If first row is checked and click OK update scooterDetails
    if(checked && !checked2 && !checked3 && !checked4 && !checked5)
    {
        if(text30 != "Rental Slot Available")
        {
            returnUnit(text30.toInt());
        }
        else
        {
            ui->label_6->setText("Please select a scooter");
            ui->label_6->show();
        }
    }
    else if(!checked && checked2 && !checked3 && !checked4 && !checked5)
    {
        // If second row is checked and click OK update scooterDetails
        if(text31 != "Rental Slot Available")
        {
            returnUnit(text31.toInt());
        }
        else
        {
            ui->label_6->setText("Please select a scooter");
            ui->label_6->show();
        }
    }
    else if(!checked && !checked2 && checked3 && !checked4 && !checked5)
    {
        // If third row is checked and click OK update scooterDetails
        if(text32 != "Rental Slot Available")
        {
            returnUnit(text32.toInt());
        }
        else
        {
            ui->label_6->setText("Please select a scooter");
            ui->label_6->show();
        }
    }
    else if(!checked && !checked2 && !checked3 && checked4 && !checked5)
    {
        // If fourth row is checked and click OK update scooterDetails
        if(text33 != "Rental Slot Available")
        {
            returnUnit(text33.toInt());
        }
        else
        {
            ui->label_6->setText("Please select a scooter");
            ui->label_6->show();
        };
    }
    else if(!checked && !checked2 && !checked3 && !checked4 && checked5)
    {
        // If fifth row is checked and click OK update scooterDetails
        if(text34 != "Rental Slot Available")
        {
            returnUnit(text34.toInt());
        }
        else
        {
            ui->label_6->setText("Please select a scooter");
            ui->label_6->show();
        }
    }
    else if(!checked && !checked2 && !checked3 && !checked4 && !checked5)
    {
        ui->label_6->setText("No scooters selected");
        ui->label_6->show();
    }
    else
    {
        ui->label_6->setText("Please select only 1 scooter");
        ui->label_6->show();
    }

    // Uncheck all checkboxes after updating the units
    ui->checkBox_6->setChecked(false);
    ui->checkBox_7->setChecked(false);
    ui->checkBox_8->setChecked(false);
    ui->checkBox_9->setChecked(false);
    ui->checkBox_10->setChecked(false);

    // Hide the button box after the action
    ui->buttonBox_2->hide();

    // Show message after rent
    ui->label_6->show();
}


void myscooters::setFilePath(QString otherPath)
{
    filePath = otherPath;
    QPixmap image(filePath + "/scooter.png");
    ui->label_5->setPixmap(image);

    // Set Database
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(filePath + "/database_q.db");

    if (!db.open()) {
        qDebug() << "Error: Could not open database." << db.lastError();
        return; // Exit the function if the database cannot be opened
    }
}
