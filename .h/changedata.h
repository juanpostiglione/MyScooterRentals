#ifndef CHANGEDATA_H
#define CHANGEDATA_H

#include <QDialog>
#include <QtSql>
#include <QDebug>


namespace Ui {
class changeData;
}

class changeData : public QDialog
{
    Q_OBJECT

public:
    explicit changeData(QWidget *parent = nullptr);
    ~changeData();

    QString filePath;
    void setFilePath(QString otherPath);

    // 'employee' or 'administrator'
    QString accRank;

private slots:
    void on_buttonBox_accepted();

    void on_toolButton_2_clicked();

    void on_comboBox_currentIndexChanged(int index);

    void on_lineEdit_editingFinished();

    void on_buttonBox_rejected();

private:
    Ui::changeData *ui;
};

#endif // CHANGEDATA_H
