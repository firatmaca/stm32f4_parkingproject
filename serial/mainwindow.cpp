#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QComboBox>
#include <QList>
#include <QTextEdit>
#include <QString>
#include <QTime>
#include <QDebug>
#include <QtSerialPort/QSerialPortInfo>
#include <QtSerialPort>

QSerialPort *serial;
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QList <QSerialPortInfo> list;
    list = QSerialPortInfo::availablePorts();
    for(int i=0 ; i<list.length(); i++){
        ui->comboBox->addItem(list[i].portName());
    }
}
MainWindow::~MainWindow()
{
    delete ui;

}
void MainWindow::on_pushButton_clicked()
{
    serial=new QSerialPort(this);
    serial->setPortName(ui->comboBox->currentText());
    serial->setBaudRate(QSerialPort::Baud115200);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    serial->open(QIODevice::ReadWrite);
    connect(serial,SIGNAL(readyRead()),this,SLOT(serialRecieved()));
    ui->progressBar->setValue(100);
}

void MainWindow::serialRecieved(){
    QString gelen=serial->readAll();
    if(gelen.length()>0){
        QString elde=ui->textEdit->toPlainText();
        ui->textEdit->setText(elde+"\n"+"received =>"+ gelen);
    }

}

void MainWindow::on_pushButton_2_clicked()
{
  QString yollanan=ui->textEdit_2->toPlainText();
  QByteArray inBytes;
  const char *cstrData;
  inBytes=yollanan.toUtf8();
  cstrData=inBytes.constData();
  serial->write(cstrData);
  QString elde=ui->textEdit->toPlainText();
  ui->textEdit->setText(elde+"\n"+"transmited =>"+ yollanan );
}




