#ifndef EMPLOYEE_MANAGER_H
#define EMPLOYEE_MANAGER_H

#include <QDialog>
#include "mainwindow.h"

namespace Ui {
class employee_manager;
}

class employee_manager : public QDialog
{
    Q_OBJECT

public:
    explicit employee_manager(QWidget *parent = nullptr);
    ~employee_manager();

    QString filePath;
    void setFilePath(QString otherPath);

    // 'employee' or 'administrator'
    QString accRank;

private slots:
    void on_toolButton_clicked();

    void on_pushButton_4_clicked();

    void on_comboBox_activated(int index);

    void on_pushButton_3_clicked();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::employee_manager *ui;

    MainWindow *mainWindow;

};

#endif // EMPLOYEE_MANAGER_H
