#include "scooterrequests.h"
#include "ui_scooterrequests.h"
#include "scooter_management.h"
#include "accountmanagement.h"
#include "employee_manager.h"
#include "mainwindow.h"
#include "myscooters.h"
#include "settingswindow.h"
#include "adddeletescooter.h"
#include <QSqlQuery>
#include <QPixmap>
#include <QtSql>


scooterRequests::scooterRequests(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::scooterRequests)
{
    ui->setupUi(this);

    // Set image and UI Style

    ui->lineEdit->hide();
    ui->lineEdit_2->hide();
    ui->lineEdit_3->hide();
    ui->lineEdit_4->hide();

    ui->label_6->hide();
    ui->label_7->hide();
    ui->label_8->hide();

    QPixmap image(filePath + "/scooter.png");
    ui->label->setPixmap(image);

    ui->comboBox_13->hide();
    ui->comboBox_13->setStyleSheet("QComboBox QAbstractItemView { color: rgb(255, 255, 255); }");

    this->setStyleSheet("background-color: rgb(1, 68, 3);");

    ui->label_2->setStyleSheet("QLabel { color : rgb(255, 165, 0); }");
    ui->frame->setStyleSheet("QFrame {" "background-color: black;" "}");

    ui->buttonBox->setStyleSheet("QPushButton{" "color: white;" "background-color: grey;"
                                 "}" "QPushButton:hover {" "background-color: orange;""}");

    ui->buttonBox_2->setStyleSheet("QPushButton{" "color: white;" "background-color: grey;"
                                 "}" "QPushButton:hover {" "background-color: orange;""}");

    ui->toolButton->setStyleSheet("QToolButton {" "color: white;" "background-color: grey;"  "padding: 3px;"
                                    "}" "QToolButton:hover {" "background-color: orange;""}");

    ui->pushButton->setStyleSheet("QPushButton {" "color: white;" "background-color: grey;" "border-radius: 3px;" "padding: 3px;"
                                  "}" "QPushButton:hover {" "background-color: orange;""}");


    ui->pushButton_2->setStyleSheet("QPushButton {" "color: white;" "background-color: grey;" "border-radius: 3px;" "padding: 3px;"
                                    "}" "QPushButton:hover {" "background-color: orange;""}");

    // Make labels white
    ui->label_3->setStyleSheet("QLabel { color : rgb(255, 255, 255); }");
    ui->label_4->setStyleSheet("QLabel { color : rgb(255, 255, 255); }");
    ui->label_5->setStyleSheet("QLabel { color : rgb(255, 255, 255); }");
    ui->label_6->setStyleSheet("QLabel { color : rgb(255, 255, 255); }");
    ui->label_7->setStyleSheet("QLabel { color : rgb(255, 255, 255); }");
    ui->label_8->setStyleSheet("QLabel { color : rgb(255, 255, 255); }");
}

scooterRequests::~scooterRequests()
{
    delete ui;
}

// When Approve button is pressed
void scooterRequests::on_pushButton_clicked()
{
    ui->lineEdit->show();
    ui->lineEdit_2->show();

    ui->label_6->show();
    ui->label_7->show();
}

// When Reject button is pressed
void scooterRequests::on_pushButton_2_clicked()
{
    ui->lineEdit_3->show();
    ui->label_8->show();
}

// When accept is pressed, approveRental function works with the UI
void scooterRequests::on_buttonBox_accepted()
{

    scooter_management scooter;
    QString username = ui->lineEdit->text();
    bool done;
    int scooterID = ui->lineEdit_2->text().toInt(&done);

    //qDebug() << scooterID << "  " << username;

    if(username.isEmpty() || !done)
    {
        ui->label_3->setText("Missing Fields.");
    }

    else
    {
        if(scooter.approveRental(scooterID, username))
        {
            ui->label_3->setText("Rental approved.");
        }
        else
        {
            ui->label_3->setText("Rental approve failed, username or ID were not found.");
        }
    }
}

// When reject button is pressed, rejectRental function works with the UI
void scooterRequests::on_buttonBox_2_accepted()
{
    scooter_management scooter;
    bool done;
    int scooterID = ui->lineEdit_3->text().toInt(&done);

    if(!done)
    {
        ui->label_3->setText("Missing Fields.");
    }

    else
    {
        if(scooter.rejectRental(scooterID))
        {
            ui->label_3->setText("Rental rejected successfully.");
        }

        else
        {
            ui->label_3->setText("Rental reject failed.");
        }
    }
}

// When cancel button for approval is pressed, line contents clear
void scooterRequests::on_buttonBox_rejected()
{
    ui->lineEdit->clear();
    ui->lineEdit_2->clear();
    ui->lineEdit->hide();
    ui->lineEdit_2->hide();

    ui->label_6->hide();
    ui->label_7->hide();
}

// When cancel button for reject is pressed, line contents clear
void scooterRequests::on_buttonBox_2_rejected()
{
    ui->lineEdit_3->clear();
    ui->lineEdit_3->hide();
    ui->label_8->hide();
}

// When menu button is pressed, menu bar shows up
void scooterRequests::on_toolButton_clicked()
{
    ui->comboBox_13->show();
}

// Change windows from menu bar
void scooterRequests::on_comboBox_13_activated(int index)
{

    ui->comboBox_13->hide();
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

            case 3: // Add/delete/update scooter window opens
                this->close();
                em.close();
                acc.close();
                scooter.close();
                sc.setModal(true);
                sc.exec();
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

            case 2: // Add/delete/update scooter window opens
                this->close();
                em.close();
                sc.setModal(true);
                sc.exec();
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

void scooterRequests::setFilePath(QString otherPath)
{
    filePath = otherPath;
    QPixmap image(filePath + "/scooter.png");
    ui->label->setPixmap(image);


    ui->comboBox_13->clear();
    QStringList list;


    if(accRank == "employee")
    {
        list = { "Rental Requests", "Home", "Add/Delete/Update a Scooter", "Log Out" };
        ui->comboBox_13->addItems(list);
    }
    else if(accRank == "administrator")
    {
        list = { "Rental Requests", "Home", "Account Management", "Add/Delete/Update a Scooter", "Log Out" };
        ui->comboBox_13->addItems(list);
    }

    //ui->comboBox_13->show();

}
