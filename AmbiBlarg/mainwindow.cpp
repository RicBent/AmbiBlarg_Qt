#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QColorDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    serial_port = new QSerialPort(this);

    foreach (QSerialPortInfo info, QSerialPortInfo::availablePorts())
    {
        serial_port->setPortName(info.portName());
        serial_port->open(QSerialPort::ReadWrite);
        serial_port->setBaudRate(QSerialPort::Baud115200);
        serial_port->setDataBits(QSerialPort::Data8);
        serial_port->setParity(QSerialPort::NoParity);
        serial_port->setStopBits(QSerialPort::OneStop);
        serial_port->setFlowControl(QSerialPort::NoFlowControl);

        // Blarg. Whatever. This will end up in a proper class (singleton) anyways
        break;
    }

    connect(serial_port, SIGNAL(readyRead()), this, SLOT(doStuff()));
}

MainWindow::~MainWindow()
{
    if (serial_port->isOpen())
        serial_port->close();

    delete serial_port;
    delete ui;
}

void MainWindow::doStuff()
{
    qDebug() << QString(serial_port->readAll());
}

void MainWindow::on_testButton_clicked()
{
    QColor color = QColorDialog::getColor(Qt::white, this, "AmbiBlarg");

    if (!color.isValid())
        return;

    writeColor(color);
}

void MainWindow::closeEvent(QCloseEvent *)
{
    writeColor(Qt::black);
}

void MainWindow::writeColor(QColor color)
{
    // Write Magic
    serial_port->write(QString("ABlarg").toLatin1());

    // Write Led Count
    quint16 ledCount = 80;
    serial_port->write((const char*)&ledCount, 2);


    // Write Color

    quint8 r = color.red();
    quint8 g = color.green();
    quint8 b = color.blue();

    for (quint16 i = 0; i < ledCount; i++)
    {
        serial_port->write((const char*)&r, 1);
        serial_port->write((const char*)&g, 1);
        serial_port->write((const char*)&b, 1);
    }
}
