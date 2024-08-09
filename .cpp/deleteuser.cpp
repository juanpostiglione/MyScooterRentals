#include "deleteuser.h"
#include "ui_deleteuser.h"
#include "accountmanagement.h"
#include "promoteaccount.h"
#include "changedata.h"
#include "scooter_management.h"


deleteUser::deleteUser(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::deleteUser)
{
    ui->setupUi(this);

    // Set window style and buttons
    ui->comboBox->hide();
    ui->comboBox->setStyleSheet("QComboBox QAbstractItemView { color: rgb(255, 255, 255); }");

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
    ui->label_7->setStyleSheet("QLabel { color : rgb(255, 255, 255); }");
}

deleteUser::~deleteUser()
{
    delete ui;
}

// When Accept button is pressed, function delete account occurs, connected with the UI
void deleteUser::on_buttonBox_accepted()
{
    accountmanagement acc;
    acc.setFilePath(filePath);

    QString username = ui->lineEdit->text();

    if(username.isEmpty())
    {
        ui->label_2->setText("Missing Fields.");
    }
    else
    {
        if(acc.deleteAccount(username))
        {
            ui->label_2->setText("Account deleted successfully.");

            scooter_management scootMgmt;
            QList<QVariantMap> scooters = scootMgmt.getAllScooters();

            int scootID;
            QString lotName;
            int distance = 0;
            int rentRate = 4;

            for(int j = 0; j < scooters.size(); j++)
            {
                if(scooters[j].value("renter").toString() == username)
                {
                    scootID = scooters[j].value("scooter_id").toString().toInt();
                    lotName = scooters[j].value("nearest_lot").toString();
                    distance = scooters[j].value("lot_distance").toString().toInt();
                    rentRate = scooters[j].value("rental_rate").toString().toInt();

                    scootMgmt.updateScooter(scootID, "Yes", lotName, distance, rentRate, "", "Available");

                    QSqlQuery query;
                    query.prepare("UPDATE scooterDetails SET units = units + 1, status = 'Available' WHERE location = :location");
                    query.bindValue(":location", lotName);

                    if(query.exec())
                    {
                        ui->label_2->setText("Account deleted successfully.");
                    }
                    else
                    {
                        ui->label_2->setText("Failed to update available lots");
                    }
                    ui->label_2->show();
                }
            }
        }
        else
        {
            ui->label_2->setText("Account delete failed, username does not exist");
            ui->label_2->show();
        }
    }
}

// Change windows from the menu bar
void deleteUser::on_comboBox_currentIndexChanged(int index)
{
    accountmanagement acc;
    promoteaccount prom;
    changeData data;

    acc.accRank = accRank;
    prom.accRank = accRank;
    data.accRank = accRank;

    acc.setFilePath(filePath);
    prom.setFilePath(filePath);
    data.setFilePath(filePath);

    switch(index)
    {
    case 1: // Open account management window
        this->close();
        prom.close();
        data.close();
        acc.setModal(true);
        acc.exec();
        break;

    case 2: // Open promote/demote window
        this->close();
        acc.close();
        data.close();
        prom.setModal(true);
        prom.exec();
        break;

    case 3: // Open change data account window
        this->close();
        prom.close();
        acc.close();
        data.setModal(true);
        data.exec();
        break;

    }
}

// When menu button is pressed, menu bar shows up
void deleteUser::on_toolButton_2_clicked()
{
    ui->comboBox->show();
}

// When cancel button is pressed, it clears the content in lines
void deleteUser::on_buttonBox_rejected()
{
    ui->lineEdit->clear();
}


void deleteUser::setFilePath(QString otherPath)
{
    filePath = otherPath;

    // Set image
    QPixmap image(filePath + "/scooter.png");
    ui->label_6->setPixmap(image);
}
