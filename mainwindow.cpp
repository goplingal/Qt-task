#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QDirIterator>
#include <QFileDialog>
#include <QMessageBox>
#include <QIntValidator>
#include <limits>
#include "dialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    timer = nullptr;
    ui->setupUi(this);
    ui->statusbar->setSizeGripEnabled(false);
    ui->lineEdit_7->setValidator(new QIntValidator(100, std::numeric_limits<int>::max(), this));
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::Modify()
{
    QDirIterator it(ui->lineEdit_2->text(), QStringList() << ui->lineEdit->text(), QDir::AllEntries | QDir::NoSymLinks | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    QStringList files;
    while (it.hasNext())
    {
        files << it.next();
    }
    ui->statusbar->showMessage("Найдено файлов для модификации: " + QString::number(files.count()));
    for (auto x : files)
    {
        QFile f(x);
        if (!f.open(QIODevice::ReadOnly))
        {
            return;
        }
        QFileInfo fileinfo(f.fileName());
        QString filename(fileinfo.fileName());
        QByteArray a;
        a = f.readAll();
        for (int i = 0; i < a.size(); i++)
        {
            a[i] = a[i] ^ ui->lineEdit_7->text().toInt();
        }
        f.flush();
        f.close();
        if (ui->checkBox->isChecked())
        {
            f.remove();
        }
        QString save_dir = ui->lineEdit_3->text();
        QFileInfo check_file(save_dir + "/" + filename);
        if (check_file.exists() && check_file.isFile())
        {
            QMessageBox Mes;
            Mes.setWindowTitle("Требуется дейстиве");
            Mes.setText("Файл с таким именем существет (" + filename + "), выберите действие: Перезаписать файл или Модифицировать имя файла");
            Mes.setIcon(QMessageBox::Question);
            QAbstractButton* pButtonYes = Mes.addButton(tr("Перезапись"), QMessageBox::YesRole);
            QAbstractButton* pButtonNo = Mes.addButton(tr("Модификация"), QMessageBox::NoRole);
            Mes.exec();
            if (Mes.clickedButton() == pButtonYes)
            {
                QFile f1(save_dir + "/" + filename);
                if (!f1.open(QIODevice::WriteOnly | QIODevice::Truncate))
                {
                    return;
                }
                f1.write(a);
                f1.flush();
                f1.close();
            }
            else if (Mes.clickedButton() == pButtonNo)
            {
                QFile f1(save_dir + "/" + filename);
                int i = 1;
                QString just_name = filename.section(".", 0, 0);
                QString suff = fileinfo.completeSuffix();
                while (i <= std::numeric_limits<int>::max() - 1)
                {
                    QFileInfo check_file_it(save_dir + "/" + just_name + QString::number(i) + "." + suff);
                    if (check_file_it.exists() && check_file_it.isFile())
                    {
                        i++;
                    }
                    else
                    {
                        QString file_n(check_file_it.fileName());
                        QFile f2(save_dir + "/" + file_n);
                        if (!f2.open(QIODevice::WriteOnly | QIODevice::Truncate))
                        {
                            return;
                        }
                        f2.write(a);
                        f2.flush();
                        f2.close();
                        break;
                    }
                }
            }
        }
        else
        {
            QFile f1(save_dir + "/" + filename);
            if (!f1.open(QIODevice::WriteOnly | QIODevice::Truncate))
            {
                return;
            }
            f1.write(a);
            f1.flush();
            f1.close();
        }
    }
}


void MainWindow::on_pushButton_clicked()
{   
    if (ui->lineEdit->text().isEmpty() || ui->lineEdit_2->text().isEmpty() || (!ui->checkBox->isChecked() && !ui->checkBox_2->isChecked()) || ui->lineEdit_7->text().isEmpty())
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
        MainWindow::Modify();
    }
}


void MainWindow::on_checkBox_stateChanged(int arg1)
{
    if (arg1 == Qt::Checked)
    {
        ui->checkBox_2->setEnabled(false);
    }
    else
    {
        ui->checkBox_2->setEnabled(true);
    }
}


void MainWindow::on_checkBox_2_stateChanged(int arg1)
{
    if (arg1 == Qt::Checked)
    {
        ui->checkBox->setEnabled(false);
    }
    else
    {
        ui->checkBox->setEnabled(true);
    }
}


void MainWindow::on_pushButton_2_clicked()
{
    QString dir= QFileDialog::getExistingDirectory(this, tr("Open Directory"), "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui->lineEdit_2->setText(dir);
}


void MainWindow::on_pushButton_3_clicked()
{
    QString dir= QFileDialog::getExistingDirectory(this, tr("Save to"), "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    ui->lineEdit_3->setText(dir);
}


void MainWindow::on_pushButton_4_clicked()
{
    if (ui->lineEdit->text().isEmpty() || ui->lineEdit_2->text().isEmpty() || (!ui->checkBox->isChecked() && !ui->checkBox_2->isChecked()) || ui->lineEdit_7->text().isEmpty())
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
        Dialog dialog;
        dialog.setModal(true);
        dialog.exec();
        QString s = dialog.Get_line();
        if (s == "")
        {
            return;
        }
        else
        {
            MainWindow::Modify();
            timer = new QTimer(this);
            connect(timer, SIGNAL(timeout()), this, SLOT(timer_slot()));
            timer->start(s.toInt() * 1000);
        }
    }

}


void MainWindow::timer_slot()
{
    MainWindow::Modify();
}


void MainWindow::on_pushButton_5_clicked()
{
    if (timer)
    {
        timer->stop();
    }
    else
    {
        QMessageBox Mes;
        Mes.setWindowTitle("Ошибка");
        Mes.setText("Таймер не запущен");
        Mes.setIcon(QMessageBox::Question);
        QAbstractButton* pButtonYes = Mes.addButton(tr("Ок"), QMessageBox::YesRole);
        Mes.exec();
        if (Mes.clickedButton() == pButtonYes)
        {
            return;
        }
    }
}

