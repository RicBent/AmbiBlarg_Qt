#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "ledmanager.h"

#include <QColorDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    updateStatusLabel(ArduinoCommunication::getInstance()->getStatus());

    connect(ArduinoCommunication::getInstance(), SIGNAL(statusChanged(ConnectionStatus)), this, SLOT(updateStatusLabel(ConnectionStatus)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateStatusLabel(ConnectionStatus status)
{
    QString statusStr;

    if (status == Status_Scanning)
        statusStr = "Scanning...";
    else if (status == Status_Connected)
        statusStr = "Connected (" + ArduinoCommunication::getInstance()->getPortName() + ")";
    else
        statusStr = "Disconnected";

    ui->statusLabel->setText(QString("Connection Status: %1").arg(statusStr));
}

void MainWindow::on_testButton_clicked()
{
    QColor color = QColorDialog::getColor(Qt::white, this, this->windowTitle());

    if (!color.isValid())
        return;

    LedManager* leds = LedManager::getInstance();

    leds->setColor(color);
    leds->updateLeds();
}

void MainWindow::on_rescanButton_clicked()
{
    ArduinoCommunication::getInstance()->rescan();
}
