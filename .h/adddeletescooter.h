#ifndef ADDDELETESCOOTER_H
#define ADDDELETESCOOTER_H

#include <QDialog>

namespace Ui {
class addDeleteScooter;
}

class addDeleteScooter : public QDialog
{
    Q_OBJECT

public:
    explicit addDeleteScooter(QWidget *parent = nullptr);
    ~addDeleteScooter();

    QString filePath;
    void setFilePath(QString otherPath);

    // 'employee' or 'administrator'
    QString accRank;

private slots:

    // Function for Push Button 1 (Delete Scooter)
    void on_pushButton_clicked();

    // Function for Push Button 3 (Add Scooter)
    void on_pushButton_3_clicked();

    // Function for Push Button 4 (Delete Scooter)
    void on_pushButton_4_clicked();

    void on_toolButton_clicked();

    void on_comboBox_15_activated(int index);


    // Hide messages when text boxes are used
    void on_lineEdit_textChanged();
    void on_lineEdit_2_textChanged();
    void on_lineEdit_3_textChanged();
    void on_lineEdit_4_textChanged();
    void on_lineEdit_5_textChanged();
    void on_lineEdit_6_textChanged();
    void on_lineEdit_7_textChanged();
    void on_lineEdit_8_textChanged();
    void on_lineEdit_9_textChanged();
    void on_lineEdit_10_textChanged();
    void on_lineEdit_11_textChanged();
    void on_lineEdit_12_textChanged();
    void on_lineEdit_13_textChanged();
    void on_lineEdit_14_textChanged();


private:
    Ui::addDeleteScooter *ui;
};

#endif // ADDDELETESCOOTER_H

