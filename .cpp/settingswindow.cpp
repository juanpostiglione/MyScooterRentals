#include "settingswindow.h"
#include "ui_settingswindow.h"
#include "scooter_management.h"
#include "rentscooter.h"
#include "myscooters.h"
#include "accountmanagement.h"
#include <QMainWindow>
#include <QtSql>
#include <QDebug>
#include <QFileInfo>
#include <QString>
#include <QChar>
#include <stdio.h>
#include <uchar.h>

settingsWindow::settingsWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::settingsWindow)
{
    ui->setupUi(this);

    // Hide index Menu
    ui->comboBox_2->hide();
    ui->comboBox_2->setStyleSheet("QComboBox QAbstractItemView { color: rgb(255, 255, 255); }");

    QPixmap pix(filePath + "/scooter.png");
    ui->label_8->setPixmap(pix);

    // Set Window Color
    this->setStyleSheet("background-color: rgb(1, 68, 3);");

    ui->label_5->show(); // App name
    ui->label_6->hide(); // Error message box

    // Set Button and Label Colors
    ui->label_7->setStyleSheet("QLabel { color : rgb(255, 165, 0); }");

    ui->frame->setStyleSheet("QFrame {" "background-color: black;}");

    ui->toolButton_2->setStyleSheet("QToolButton {" "color: white;" "background-color: grey;"  "padding: 3px;"
                                  "}" "QToolButton:hover {" "background-color: orange;""}");

    ui->buttonBox_2->setStyleSheet("QPushButton{" "color: white;" "background-color: grey;"
                                 "}" "QPushButton:hover {" "background-color: orange;""}");

    // Set Window Color
    this->setStyleSheet("background-color: rgb(1, 68, 3);");

    // Check the Index Menu
    ui->comboBox_2->setCurrentIndex(3);

    // Initially set Label Position
    labelPosition();

    // Make labels white
    ui->label_3->setStyleSheet("QLabel { color : rgb(255, 255, 255); }");
    ui->label_5->setStyleSheet("QLabel { color : rgb(255, 255, 255); }");
    ui->label_6->setStyleSheet("QLabel { color : rgb(255, 255, 255); }");
    ui->label_9->setStyleSheet("QLabel { color : rgb(255, 255, 255); }");
    ui->label_10->setStyleSheet("QLabel { color : rgb(255, 255, 255); }");
}

settingsWindow::~settingsWindow()
{
    delete ui;
}

// Function for Label position
void settingsWindow::labelPosition()
{
    // Get Width and Height for Labels
    int labelWidth = ui->label_9->width();
    int labelHeight = ui->label_9->height();

    // Get the Center of the Window
    int centerX = (windowWidth - labelWidth) / 2;
    int centerY = (windowHeight - labelHeight) / 2;

    // Labels Position
    ui->label_9->move(centerX, centerY+80);
    ui->label_10->move(centerX, centerY+40);

    // Line Text Position
    ui->lineEdit_5->move(centerX+120, centerY+40);
    ui->lineEdit_6->move(centerX+120, centerY+80);
}


// Method to set guestRent
void settingsWindow::setGuest(bool isGuest)
{
    guestAcc = isGuest;
}

// Method to set current user
void settingsWindow::setCurrentUser(QString& userN)
{
    currentUser = userN;
}

// Method to set account_management pointer
void settingsWindow::setAccMgmt(accountmanagement* a)
{
    accMgmt = a;
}

// Method to set scooter_management pointer
void settingsWindow::setScooterMgmt(scooter_management* s)
{
    scootMgmt = s;
}

// Function for click OK
void settingsWindow::on_buttonBox_2_rejected()
{
    ui->lineEdit_5->clear();
    ui->lineEdit_6->clear();
    ui->label_6->hide();
}

// Function for click Cancel
void settingsWindow::on_buttonBox_2_accepted()
{
    // Strings for sign up credentials
    QString newUsername = ui->lineEdit_5->text();
    QString newPassword = ui->lineEdit_6->text();

    if(!guestAcc)
    {
        if(newUsername == "" && newPassword == "")
        {
            //Nothing happens if neither field is filled
            ui->label_6->hide();
        }
        else if(newUsername.size() > 20)
        {
            ui->label_6->setText("Username must not exceed 20 characters");
            ui->label_6->show();
        }
        else if((newPassword != "" && newPassword.size() < 6) || newPassword.size() > 20)
        {
            ui->label_6->setText("Password length must be between 6 and 20 characters");
            ui->label_6->show();
        }
        else
        {
            QString temp = newUsername;
            // Attempt to update password
            if(accMgmt->changeUserPass(currentUser, newUsername, newPassword))
            {
                // Display different message depending on what information is changed
                if(temp == "")
                {
                    ui->label_6->setText("Password successfully updated");
                }
                else if(newPassword == "")
                {
                    ui->label_6->setText("Username successfully updated");
                }
                else
                {
                    ui->label_6->setText("Login information successfully updated");
                }
                ui->label_6->show();
                ui->lineEdit_5->clear();
                ui->lineEdit_6->clear();

                scooters = scootMgmt->getAllScooters();

                // Update scooterIndex to reflect updated renter username
                for(int i = 0; i < scooters.size(); i++)
                {
                    QString renterUser = scooters[i].value("renter").toString();

                    if(renterUser == currentUser)
                    {
                        int ID = scooters[i].value("scooter_id").toString().toInt();
                        QString nearLot = scooters[i].value("nearest_lot").toString();
                        int lotDistance = scooters[i].value("lot_distance").toString().toInt();
                        int rentRate = scooters[i].value("rental_rate").toString().toInt();
                        QString status = scooters[i].value("status").toString();

                        scootMgmt->updateScooter(ID, "No", nearLot, lotDistance, rentRate, newUsername, status);
                    }
                }
                currentUser = newUsername;
            }
            else
            {
                ui->label_6->setText("There was an error updating your login information");
                ui->label_6->show();
            }
        }
    }
    else
    {
        ui->label_6->setText("You must be signed in to change login information");
        ui->label_6->show();
    }
}

// Show Menu when tool button is clicked
void settingsWindow::on_toolButton_2_clicked()
{
    ui->comboBox_2->show();
}

// Function for dropdown menu
void settingsWindow::on_comboBox_2_activated(int index)
{
    scooter_management window;
    ui->comboBox_2->hide();
    mainWindow = new MainWindow(nullptr);
    window.setCurrentUser(currentUser);
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

    // Clear text fields
    ui->lineEdit_5->clear();
    ui->lineEdit_6->clear();

    // Switch windows from Menu
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

        // Go to My Scooters window
        case 2:
            this->close();
            myS.setModal(true);
            myS.exec();
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

void settingsWindow::setFilePath(QString otherPath)
{
    filePath = otherPath;
    QPixmap pix(filePath + "/scooter.png");
    ui->label_8->setPixmap(pix);

    // Set Database
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(filePath + "/database_q.db");

    if (!db.open()) {
        qDebug() << "Error: Could not open database." << db.lastError();
        return; // Exit the function if the database cannot be opened
    }
}
