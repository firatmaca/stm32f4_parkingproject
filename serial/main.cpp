#include "mainwindow.h"
#include <QApplication>
#include <QtSerialPort/QSerialPortInfo>
#include <QComboBox>
#include <QList>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
