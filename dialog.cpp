#include "dialog.h"
#include "ui_dialog.h"
#include <QString>
#include <QMessageBox>
#include <QPushButton>
#include <QCloseEvent>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    s = "";
}

Dialog::~Dialog()
{
    delete ui;
}


QString Dialog::Get_line()
{
    return s;
}


void Dialog::on_pushButton_clicked()
{
    if (ui->lineEdit->text().isEmpty())
    {
        QMessageBox Mes;
        Mes.setWindowTitle("Ошибка");
        Mes.setText("Заполните все поля");
        Mes.setIcon(QMessageBox::Question);
        QAbstractButton* pButtonYes = Mes.addButton(tr("Ок"), QMessageBox::YesRole);
        Mes.exec();
        if (Mes.clickedButton() == pButtonYes)
        {
            return;
        }
    }
    else
    {
        s = ui->lineEdit->text();
        this->close();
    }
}


void Dialog::on_pushButton_2_clicked()
{
    s = "";
    this->close();
}
