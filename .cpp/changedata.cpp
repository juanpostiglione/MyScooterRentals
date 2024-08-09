#include "changedata.h"
#include "ui_changedata.h"
#include "accountmanagement.h"
#include "promoteaccount.h"
#include "deleteuser.h"
#include "scooter_management.h"
#include "employee_manager.h"

changeData::changeData(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::changeData)
{
    ui->setupUi(this);

    // Set style of window and buttons
    ui->comboBox->hide();
    ui->comboBox->setStyleSheet("QComboBox QAbstractItemView { color: rgb(255, 255, 255); }");

    ui->buttonBox->hide();


    this->setStyleSheet("background-color: rgb(1, 68, 3);");

    ui->label_5->setStyleSheet("QLabel { color : rgb(255, 165, 0); }");

    ui->frame->setStyleSheet("QFrame {" "background-color: black;" "}");

    ui->buttonBox->setStyleSheet("QPushButton{" "color: white;" "background-color: grey;"
                                 "}" "QPushButton:hover {" "background-color: orange;""}");

    ui->toolButton_2->setStyleSheet("QToolButton {" "color: white;" "background-color: grey;"  "padding: 3px;"
                                    "}" "QToolButton:hover {" "background-color: orange;""}");

    // Set labels to white
    ui->label->setStyleSheet("QLabel { color : rgb(255, 255, 255); }");
    ui->label_2->setStyleSheet("QLabel { color : rgb(255, 255, 255); }");
    ui->label_3->setStyleSheet("QLabel { color : rgb(255, 255, 255); }");
    ui->label_4->setStyleSheet("QLabel { color : rgb(255, 255, 255); }");
    ui->label_7->setStyleSheet("QLabel { color : rgb(255, 255, 255); }");
    ui->label_8->setStyleSheet("QLabel { color : rgb(255, 255, 255); }");
}

changeData::~changeData()
{
    delete ui;
}

// Function to set the UI with the changeUserPass function
void changeData::on_buttonBox_accepted()
{
    ui->buttonBox->hide();
    accountmanagement acc;
    QString currentUsername = ui->lineEdit->text();
    QString newUsername = ui->lineEdit_2->text();
    QString newPass = ui->lineEdit_3->text();

    if(currentUsername.isEmpty() || newUsername.isEmpty() || newPass.isEmpty())
    {
        ui->label_4->setText("Missing Fields.");
    }

    else
    {
        if(acc.changeUserPass(currentUsername, newUsername, newPass))
        {
            ui->label_4->setText("Login info changed.");

            scooter_management scootMgmt;
            QList<QVariantMap> scooters = scootMgmt.getAllScooters();

            int scootID;
            QString scootAvail;
            QString lotName;
            int distance = 0;
            int rentRate = 4;
            QString sStatus;

            for(int j = 0; j < scooters.size(); j++)
            {
                if(scooters[j].value("renter").toString() == currentUsername)
                {
                    scootID = scooters[j].value("scooter_id").toString().toInt();
                    scootAvail = scooters[j].value("available").toString();
                    lotName = scooters[j].value("nearest_lot").toString();
                    distance = scooters[j].value("lot_distance").toString().toInt();
                    rentRate = scooters[j].value("rental_rate").toString().toInt();
                    sStatus = scooters[j].value("status").toString();

                    scootMgmt.updateScooter(scootID, scootAvail, lotName, distance, rentRate, newUsername, sStatus);
                }
            }
        }

        else
        {
            ui->label_4->setText("Username change failed, username does not exist.");
        }
    }

}

// When menu button is clicked
void changeData::on_toolButton_2_clicked()
{
    ui->comboBox->show();
}

// When changin windows from menu bar
void changeData::on_comboBox_currentIndexChanged(int index)
{
    accountmanagement acc;
    promoteaccount prom;
    employee_manager em;
    deleteUser user;

    acc.accRank = accRank;
    prom.accRank = accRank;
    em.accRank = accRank;
    user.accRank = accRank;

    acc.setFilePath(filePath);
    prom.setFilePath(filePath);
    em.setFilePath(filePath);
    user.setFilePath(filePath);

    switch(index)
    {
    case 1: // Open account management window
        this->close();
        prom.close();
        user.close();
        em.close();
        acc.setModal(true);
        acc.exec();

        break;

    case 3: // Open promote/demote account
        this->close();
        acc.close();
        em.close();
        user.close();
        prom.setModal(true);
        prom.exec();
        break;

    case 2: // Open delete account window
        this->close();
        prom.close();
        acc.close();
        em.close();
        user.setModal(true);
        user.exec();
        break;
    }

}

// When finish writing in line edit, button box shows up
void changeData::on_lineEdit_editingFinished()
{
    ui->buttonBox->show();
}

// When cancel button is pressed, button box hide
void changeData::on_buttonBox_rejected()
{
    ui->buttonBox->hide();
}

void changeData::setFilePath(QString otherPath)
{
    filePath = otherPath;

    // Set image for window
    QPixmap image(filePath + "/scooter.png");
    ui->label_6->setPixmap(image);
}
