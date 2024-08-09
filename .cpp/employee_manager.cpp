#include "employee_manager.h"
#include "ui_employee_manager.h"
#include <QPixmap>
#include "accountmanagement.h"
#include "mainwindow.h"
#include "scooterrequests.h"
#include "adddeletescooter.h"
employee_manager::employee_manager(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::employee_manager)
{
    ui->setupUi(this);

    this->show();

    // Set style button and window
    ui->comboBox->hide();
    ui->comboBox->setStyleSheet("QComboBox QAbstractItemView { color: rgb(255, 255, 255); }");

    this->setStyleSheet("background-color: rgb(1, 68, 3);");

    ui->label_2->setStyleSheet("QLabel { color : rgb(255, 165, 0); }");
    ui->label_3->setStyleSheet("QLabel { color : rgb(255, 255, 255); }");
    ui->label_5->setStyleSheet("QLabel { color : rgb(255, 255, 255); }");

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

    ui->frame->setStyleSheet("QFrame {" "background-color: black;" "}");

}



employee_manager::~employee_manager()
{
    delete ui;
}

// When menu button is pressed, menubar shows up
void employee_manager::on_toolButton_clicked()
{
    ui->comboBox->show();
}


// When Account Management button is pressed
void employee_manager::on_pushButton_4_clicked()
{
    this->close();
    accountmanagement acc;
    acc.accRank = accRank;
    acc.setFilePath(filePath);

    acc.setModal(true);
    acc.exec();
}

// When Account Management button is pressed in Menu
void employee_manager::on_comboBox_activated(int index)
{
    accountmanagement acc;
    scooterRequests req;
    addDeleteScooter sc;

    acc.accRank = accRank;
    req.accRank = accRank;
    sc.accRank = accRank;

    acc.setFilePath(filePath);
    req.setFilePath(filePath);
    sc.setFilePath(filePath);

    if(accRank == "administrator")
    {
        switch(index)
        {
            case 1: // Open account management window
                this->close();
                req.close();
                acc.setModal(true);
                acc.exec();
                sc.close();
                break;

            case 2: // Open add, delete, update scooter window
                this->close();
                acc.close();
                req.close();
                sc.setModal(true);
                sc.exec();
                break;

            case 3: // Open scooter request window
                this->close();
                acc.close();
                sc.close();
                req.setModal(true);
                req.exec();
                break;

            case 4: // Log Out
                this->close();
                acc.close();
                req.close();
                sc.close();
                // Go to log in window and close current window
                mainWindow = new MainWindow(nullptr);
                mainWindow->show();
                break;
        }
    }
    else
    {
        switch(index)
        {
            // Standard employees cannot edit account details.
            case 1: // Open add, delete, update scooter window
                this->close();
                acc.close();
                req.close();
                sc.setModal(true);
                sc.exec();
                break;

            case 2: // Open scooter request window
                this->close();
                acc.close();
                sc.close();
                req.setModal(true);
                req.exec();
                break;

            case 3: // Log Out
                this->close();
                acc.close();
                req.close();
                sc.close();
                // Go to log in window and close current window
                mainWindow = new MainWindow(nullptr);
                mainWindow->show();
                break;
        }
    }
}

// When approve reject scooter button is pressed
void employee_manager::on_pushButton_3_clicked()
{
    this->close();
    scooterRequests req;
    req.accRank = accRank;
    req.setFilePath(filePath);

    req.setModal(true);
    req.exec();
}

// When log out button is clicked
void employee_manager::on_pushButton_clicked()
{
    // Go to log in window and close current window
    mainWindow = new MainWindow(nullptr);
    mainWindow->show();
    this->close();
}

// When add, delete, update scooter button is pressed
void employee_manager::on_pushButton_2_clicked()
{
    this->close();
    addDeleteScooter sc;
    sc.accRank = accRank;
    sc.setFilePath(filePath);

    sc.setModal(true);
    sc.exec();
}


void employee_manager::setFilePath(QString otherPath)
{
    filePath = otherPath;

    // Set image
    QPixmap image(filePath + "/scooter.png");
    ui->label->setPixmap(image);
    ui->label->show();
    QStringList list;

    ui->comboBox->clear();

    if(accRank == "employee")
    {
        list = { "Home", "Add/Delete/Update a Scooter", "Rental Requests", "Log Out" };
        ui->comboBox->addItems(list);
        ui->pushButton_4->hide();
    }
    else if(accRank == "administrator")
    {
        list = { "Home", "Account Management", "Add/Delete/Update a Scooter", "Rental Requests", "Log Out" };
        ui->comboBox->addItems(list);
        ui->pushButton_4->show();
    }

    //ui->comboBox->show();
}
