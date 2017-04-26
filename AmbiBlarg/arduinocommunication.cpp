#include "arduinocommunication.h"

#include <QDebug>

ArduinoCommunication* ArduinoCommunication::instance = NULL;

ArduinoCommunication* ArduinoCommunication::init(QObject* parent)
{
    if (instance != NULL)
        throw new std::runtime_error("ArduinoCommunication already inited.");

    instance = new ArduinoCommunication(parent);
    return instance;
}

ArduinoCommunication* ArduinoCommunication::getInstance()
{
    if (instance == NULL)
        throw new std::runtime_error("ArduinoCommunication not inited.");

    return instance;
}

ArduinoCommunication::ArduinoCommunication(QObject* parent) : QObject(parent)
{
    status = Status_Disconnected;
    serial_port = NULL;
}

ArduinoCommunication::~ArduinoCommunication()
{
    if (serial_port != NULL)
    {
        if (serial_port->isOpen())
        {
            serial_port->waitForBytesWritten();
            serial_port->close();
        }

        delete serial_port;
    }
}


ConnectionStatus ArduinoCommunication::getStatus()
{
    return status;
}

QString ArduinoCommunication::getPortName()
{
    if (serial_port == NULL)
        return NULL;

    return serial_port->portName();
}


void ArduinoCommunication::rescan()
{
    if (status == Status_Scanning)
    {
        scanIndex = -1;
        return;
    }

    if (serial_port != NULL)
    {
        if (serial_port->isOpen())
            serial_port->close();

        delete serial_port;
    }

    scan_ports = QSerialPortInfo::availablePorts();

    if (scan_ports.size() == 0)
    {
        status = Status_Disconnected;
        return;
    }

    scanIndex = 0;
    status = Status_Scanning;

    emit statusChanged(status);

    testPort();
}

void ArduinoCommunication::testPort()
{
    if (scanIndex >= scan_ports.length())
        throw new std::runtime_error("Requested out of index port.");

    const QSerialPortInfo& info = scan_ports.at(scanIndex);

    qDebug() << "Testing port" << info.portName();

    serial_port = new QSerialPort(this);

    serial_port->setPortName(info.portName());
    serial_port->open(QSerialPort::ReadWrite);
    serial_port->setBaudRate(QSerialPort::Baud115200);
    serial_port->setDataBits(QSerialPort::Data8);
    serial_port->setParity(QSerialPort::NoParity);
    serial_port->setStopBits(QSerialPort::OneStop);
    serial_port->setFlowControl(QSerialPort::NoFlowControl);

    connect(serial_port, SIGNAL(readyRead()), this, SLOT(readPortData()));

    timeoutTimer = new QTimer(this);
    timeoutTimer->setInterval(TIMEOUT);
    timeoutTimer->setSingleShot(true);
    connect(timeoutTimer, SIGNAL(timeout()), SLOT(scanTimeout()));
    timeoutTimer->start();
}

void ArduinoCommunication::readPortData()
{
    if (!timeoutTimer->isActive())
        return;

    timeoutTimer->stop();

    QString data = serial_port->readAll();
    qDebug() << "Port answer" << data;

    if (data.contains(MAGIC))
        status = Status_Connected;

    testPortFinished();
}

void ArduinoCommunication::scanTimeout()
{
    qDebug() << "Port timeout";
    testPortFinished();
}

void ArduinoCommunication::testPortFinished()
{
    if (status == Status_Connected)
    {
        emit statusChanged(status);
        qDebug() << "Scanning finished: Connected.";
        scan_ports.clear();
        return;
    }

    if (serial_port->isOpen())
        serial_port->close();

    delete serial_port;
    serial_port = NULL;

    if (scanIndex >= scan_ports.size()-1)
    {
        status = Status_Disconnected;
        emit statusChanged(status);
        qDebug() << "Scanning finished: No device found.";
        scan_ports.clear();
        return;
    }

    scanIndex++;
    testPort();
}


bool ArduinoCommunication::writeData(quint8* data, quint64 len)
{
    if (status != Status_Connected)
        return false;

    serial_port->write((const char*)data, len);

    return true;
}

bool ArduinoCommunication::write8(quint8 val)
{
    return writeData((quint8*)&val, 1);
}

bool ArduinoCommunication::write16(quint16 val)
{
    return writeData((quint8*)&val, 2);
}

bool ArduinoCommunication::write32(quint32 val)
{
    return writeData((quint8*)&val, 4);
}

bool ArduinoCommunication::writeStr(QString val)
{
    if (status != Status_Connected)
        return false;

    serial_port->write(val.toLatin1());

    return true;
}
