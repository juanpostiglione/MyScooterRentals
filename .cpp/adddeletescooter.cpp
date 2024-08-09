#include "adddeletescooter.h"
#include "ui_adddeletescooter.h"
#include "scooter_management.h"
#include "accountmanagement.h"
#include "employee_manager.h"
#include "mainwindow.h"
#include "myscooters.h"
#include "settingswindow.h"
#include "scooterrequests.h"
#include "adddeletescooter.h"
#include <QSqlQuery>
#include <QPixmap>
#include <QtSql>

addDeleteScooter::addDeleteScooter(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::addDeleteScooter)
{
    ui->setupUi(this);

    this->setStyleSheet("background-color: rgb(1, 68, 3);");

    ui->label_2->setStyleSheet("QLabel { color : rgb(255, 165, 0); }");
    ui->frame->setStyleSheet("QFrame {" "background-color: black;" "}");

    // Set labels to white
    ui->label_3->setStyleSheet("QLabel { color : rgb(255, 255, 255); }");
    ui->label_5->setStyleSheet("QLabel { color : rgb(255, 255, 255); }");
    ui->label_6->setStyleSheet("QLabel { color : rgb(255, 255, 255); }");
    ui->label_7->setStyleSheet("QLabel { color : rgb(255, 255, 255); }");
    ui->label_8->setStyleSheet("QLabel { color : rgb(255, 255, 255); }");
    ui->label_9->setStyleSheet("QLabel { color : rgb(255, 255, 255); }");
    ui->label_10->setStyleSheet("QLabel { color : rgb(255, 255, 255); }");
    ui->label_11->setStyleSheet("QLabel { color : rgb(255, 255, 255); }");
    ui->label_12->setStyleSheet("QLabel { color : rgb(255, 255, 255); }");
    ui->label_13->setStyleSheet("QLabel { color : rgb(255, 255, 255); }");
    ui->label_14->setStyleSheet("QLabel { color : rgb(255, 255, 255); }");
    ui->label_15->setStyleSheet("QLabel { color : rgb(255, 255, 255); }");
    ui->label_16->setStyleSheet("QLabel { color : rgb(255, 255, 255); }");
    ui->label_17->setStyleSheet("QLabel { color : rgb(255, 255, 255); }");
    ui->label_18->setStyleSheet("QLabel { color : rgb(255, 255, 255); }");
    ui->label_19->setStyleSheet("QLabel { color : rgb(255, 255, 255); }");
    ui->label_20->setStyleSheet("QLabel { color : rgb(255, 255, 255); }");
    ui->label_21->setStyleSheet("QLabel { color : rgb(255, 255, 255); }");


    ui->toolButton->setStyleSheet("QToolButton {" "color: white;" "background-color: grey;"  "padding: 3px;"
                                  "}" "QToolButton:hover {" "background-color: orange;""}");

    ui->pushButton->setStyleSheet("QPushButton {" "color: white;" "background-color: grey;"  "padding: 3px;"
                                  "}" "QPushButton:hover {" "background-color: orange;""}");

    ui->pushButton_3->setStyleSheet("QPushButton {" "color: white;" "background-color: grey;"  "padding: 3px;"
                                    "}" "QPushButton:hover {" "background-color: orange;""}");

    ui->pushButton_4->setStyleSheet("QPushButton {" "color: white;" "background-color: grey;"  "padding: 3px;"
                                    "}" "QPushButton:hover {" "background-color: orange;""}");

    ui->comboBox_15->hide();
    ui->comboBox_15->setStyleSheet("QComboBox QAbstractItemView { color: rgb(255, 255, 255); }");

    ui->label_17->hide();
    ui->label_18->hide();
    ui->label_19->hide();

}

addDeleteScooter::~addDeleteScooter()
{
    delete ui;
}

// Function for Push Button 1 (Delete Scooter)
void addDeleteScooter::on_pushButton_clicked()
{
    int scootID = ui->lineEdit->text().toInt();

    qDebug() << scootID;

    scooter_management scootMgmt;
    QString lotName;
    int count = 0;
    QString lotAvailable = "No";
    bool found = false;

    QList<QVariantMap> scooters = scootMgmt.getAllScooters();

    // Search for lot name for given scooter id
    for(int i = 0; i < scooters.size(); i++)
    {
        if(scooters[i].value("scooter_id").toString().toInt() == scootID)
        {
            found = true;
            scootMgmt.deleteScooter(scootID);
            lotName = scooters[i].value("nearest_lot").toString();
            break;
        }
    }

    if(found)
    {
        // Search for all scooters with same lot name
        for(int j = 0; j < scooters.size(); j++)
        {
            if(scooters[j].value("nearest_lot").toString() == lotName)
            {
                count++;
            }
        }

        if(count > 0)
        {
            lotAvailable = "Yes";
        }

        QSqlQuery query;
        query.prepare("UPDATE scooterDetails SET units = :units, status = :status WHERE location = :location");
        query.bindValue(":units", count);
        query.bindValue(":status", lotAvailable);
        query.bindValue(":location", lotName);

        if(query.exec())
        {
            ui->label_19->setText("Scooter deleted");
        }
        else
        {
            ui->label_19->setText("Scooter not deleted");
        }
    }
    else
    {
        ui->label_19->setText("Scooter doesn't exist");
    }
    ui->label_19->show();
}

// Function for Push Button 3 (Add Scooter)
void addDeleteScooter::on_pushButton_3_clicked()
{
    QString scootAvail = ui->lineEdit_2->text();
    QString lotName = ui->lineEdit_3->text();
    int distance = ui->lineEdit_4->text().toInt();
    int rentRate = ui->lineEdit_5->text().toInt();
    QString renterName = ui->lineEdit_6->text();
    QString scootStatus = ui->lineEdit_7->text();

    scooter_management scootMgmt;
    scootMgmt.addScooter(scootAvail, lotName, distance, rentRate, renterName, scootStatus);

    ui->label_17->show();
}

// Function for Push Button 4 (Update Scooter)
void addDeleteScooter::on_pushButton_4_clicked()
{
    int scootID = ui->lineEdit_8->text().toInt();
    QString scootAvail = ui->lineEdit_9->text();
    QString lotName = ui->lineEdit_10->text();
    int distance = ui->lineEdit_11->text().toInt();
    int rentRate = ui->lineEdit_12->text().toInt();
    QString renterName = ui->lineEdit_13->text();
    QString scootStatus = ui->lineEdit_14->text();

    scooter_management scootMgmt;
    scootMgmt.updateScooter(scootID, scootAvail, lotName, distance, rentRate, renterName, scootStatus);

    ui->label_18->show();
}

// When menu button is pressed, menu bar shows up
void addDeleteScooter::on_toolButton_clicked()
{
    ui->comboBox_15->show();
}

// Change windows from menu bar
void addDeleteScooter::on_comboBox_15_activated(int index)
{

    ui->comboBox_15->hide();
    MainWindow* mainWindow = new MainWindow(nullptr);

    accountmanagement acc;
    scooterRequests scooter;
    employee_manager em;
    addDeleteScooter sc;

    acc.accRank = accRank;
    scooter.accRank = accRank;
    em.accRank = accRank;
    sc.accRank = accRank;

    acc.setFilePath(filePath);
    scooter.setFilePath(filePath);
    em.setFilePath(filePath);
    sc.setFilePath(filePath);

    if(accRank == "administrator")
    {
        switch(index)
        {
        case 1: // Employee manager window opens
            this->close();
            acc.close();
            em.setModal(true);
            em.exec();
            break;

        case 2: // Account management window opens
            this->close();
            em.close();
            acc.setModal(true);
            acc.exec();
            break;

        case 3: // Rental Requests window opens
            this->close();
            em.close();
            acc.close();
            sc.close();
            scooter.setModal(true);
            scooter.exec();
            break;

        case 4: // Log Out
            this->close();
            acc.close();
            em.close();
            scooter.close();

            mainWindow->show();
            mainWindow->resize(400,500);
        }
    }
    else
    {
        switch(index)
        {
        case 1: // Employee manager window opens
            this->close();
            acc.close();
            em.setModal(true);
            em.exec();
            break;

        case 2: // Rental requests window opens
            this->close();
            em.close();
            acc.close();
            sc.close();
            em.close();
            scooter.setModal(true);
            scooter.exec();
            break;

        case 3: // Log Out
            this->close();
            acc.close();
            em.close();
            scooter.close();

            mainWindow->show();
            mainWindow->resize(400,500);
        }
    }
}


void addDeleteScooter::setFilePath(QString otherPath)
{
    filePath = otherPath;

    // Image of scooter to display in the window
    QPixmap image(filePath + "/scooter.png");
    ui->label->setPixmap(image);

    ui->comboBox_15->clear();
    QStringList list;


    if(accRank == "employee")
    {
        list = { "Add/Delete/Update a Scooter", "Home", "Rental Requests", "Log Out" };
        ui->comboBox_15->addItems(list);
    }
    else if(accRank == "administrator")
    {
        list = { "Add/Delete/Update a Scooter", "Home", "Account Management", "Rental Requests", "Log Out" };
        ui->comboBox_15->addItems(list);
    }

}


// Hide messages when typing
void addDeleteScooter::on_lineEdit_textChanged()
{
    ui->label_17->hide();
    ui->label_18->hide();
    ui->label_19->hide();
}
void addDeleteScooter::on_lineEdit_2_textChanged()
{
    ui->label_17->hide();
    ui->label_18->hide();
    ui->label_19->hide();
}
void addDeleteScooter::on_lineEdit_3_textChanged()
{
    ui->label_17->hide();
    ui->label_18->hide();
    ui->label_19->hide();
}
void addDeleteScooter::on_lineEdit_4_textChanged()
{
    ui->label_17->hide();
    ui->label_18->hide();
    ui->label_19->hide();
}
void addDeleteScooter::on_lineEdit_5_textChanged()
{
    ui->label_17->hide();
    ui->label_18->hide();
    ui->label_19->hide();
}
void addDeleteScooter::on_lineEdit_6_textChanged()
{
    ui->label_17->hide();
    ui->label_18->hide();
    ui->label_19->hide();
}
void addDeleteScooter::on_lineEdit_7_textChanged()
{
    ui->label_17->hide();
    ui->label_18->hide();
    ui->label_19->hide();
}
void addDeleteScooter::on_lineEdit_8_textChanged()
{
    ui->label_17->hide();
    ui->label_18->hide();
    ui->label_19->hide();
}
void addDeleteScooter::on_lineEdit_9_textChanged()
{
    ui->label_17->hide();
    ui->label_18->hide();
    ui->label_19->hide();
}
void addDeleteScooter::on_lineEdit_10_textChanged()
{
    ui->label_17->hide();
    ui->label_18->hide();
    ui->label_19->hide();
}
void addDeleteScooter::on_lineEdit_11_textChanged()
{
    ui->label_17->hide();
    ui->label_18->hide();
    ui->label_19->hide();
}
void addDeleteScooter::on_lineEdit_12_textChanged()
{
    ui->label_17->hide();
    ui->label_18->hide();
    ui->label_19->hide();
}
void addDeleteScooter::on_lineEdit_13_textChanged()
{
    ui->label_17->hide();
    ui->label_18->hide();
    ui->label_19->hide();
}
void addDeleteScooter::on_lineEdit_14_textChanged()
{
    ui->label_17->hide();
    ui->label_18->hide();
    ui->label_19->hide();
}
