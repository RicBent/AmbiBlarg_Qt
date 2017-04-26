#ifndef ARDUINOCOMMUNICATION_H
#define ARDUINOCOMMUNICATION_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>

enum ConnectionStatus
{
    Status_Disconnected,
    Status_Connected,
    Status_Scanning
};

const qint32    BAUDE_RATE = 115200;
const QString   MAGIC("ABlarg");
const int       TIMEOUT = 3000;

class ArduinoCommunication : public QObject
{
    Q_OBJECT
public:
    static ArduinoCommunication* init(QObject* parent = 0);
    static ArduinoCommunication* getInstance();
    ~ArduinoCommunication();

    void rescan();

    bool writeData(quint8* data, quint64 len);
    bool write8(quint8 val);
    bool write16(quint16 val);
    bool write32(quint32 val);
    bool writeStr(QString val);

    ConnectionStatus getStatus();
    QString getPortName();

protected:
    explicit ArduinoCommunication(QObject* parent = 0);

signals:
    void statusChanged(ConnectionStatus status);

public slots:

private slots:
    void readPortData();
    void scanTimeout();

private:
    static ArduinoCommunication* instance;

    ConnectionStatus status;

    QSerialPort* serial_port;

    int scanIndex;
    QTimer* timeoutTimer;
    QList<QSerialPortInfo> scan_ports;

    void testPort();
    void testPortFinished();
};

#endif // ARDUINOCOMMUNICATION_H
