#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QDebug>
#include <QFileInfo>
#include <QString>
#include <QChar>
#include <stdio.h>
#include <uchar.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:

    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

    // Function for button position
    void buttonPosition();

    // Function for Label position
    void labelPosition();

    // Function for Line Text position
    void lineTextPosition();
    
    // Function for Font
    void setFontAttributes(int size, bool bold, bool italic);
    
    // Function to check if data is in database
    void loginDataBase();
    
    // Function to get the width of the window
    int windowWidth = this->width();
    
    // Function to get the heigth of the window
    int windowHeight = this->height();

private slots:
    
    // Function for Push Button 1 ( Login Button )
    void on_pushButton_clicked();
    
    // Function for Push Button 5 ( Back Button )
    void on_pushButton_5_clicked();
    
    // Function for Push Button 2 ( Sign up )
    void on_pushButton_2_clicked();
    
    // Function for Push Button 3 ( Create Account )
    void on_pushButton_3_clicked();
    
    // Function for Push Button 4 ( Next )
    void on_pushButton_4_clicked();

    // Function for Push Button 6 (Continue as Guest)
    void on_pushButton_6_clicked();

private:

    //Edit the filePath variable with the file path for your Group-5-Scooters-main folder
    //Example: filePath = "C:/Users/Bob/Summer_2024/Group-5-Scooters-main";
    QString filePath = "";

    Ui::MainWindow *ui;
    QString pwdKey = "HI0qnP4YuI1WttiUie0e"; // Key used for password encryption and decryption

    // Function to encrypt and decrypt each user's password before storing in database.
    QString encryptPassword(QString pwdStr);

    // Function to decrypt each user's password after reading from database.
    QString decryptPassword(QString pwdStr);
};
#endif // MAINWINDOW_H

