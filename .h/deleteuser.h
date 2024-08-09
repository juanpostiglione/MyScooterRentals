#ifndef DELETEUSER_H
#define DELETEUSER_H

#include <QDialog>

namespace Ui {
class deleteUser;
}

class deleteUser : public QDialog
{
    Q_OBJECT

public:
    explicit deleteUser(QWidget *parent = nullptr);
    ~deleteUser();

    QString filePath;
    void setFilePath(QString otherPath);

    // 'employee' or 'administrator'
    QString accRank;

private slots:
    void on_buttonBox_accepted();

    void on_comboBox_currentIndexChanged(int index);

    void on_toolButton_2_clicked();

    void on_buttonBox_rejected();

private:
    Ui::deleteUser *ui;
};

#endif // DELETEUSER_H
