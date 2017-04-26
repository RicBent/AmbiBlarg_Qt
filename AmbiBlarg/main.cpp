#include "mainwindow.h"

#include "settings.h"
#include "ledmanager.h"
#include "arduinocommunication.h"

#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication application(argc, argv);

    Settings::init();
    ArduinoCommunication::init();
    LedManager::init();

    ArduinoCommunication::getInstance()->rescan();

    MainWindow mainWindow;
    mainWindow.show();

    int code = application.exec();

    delete LedManager::getInstance();
    delete ArduinoCommunication::getInstance();
    delete Settings::getInstance();

    return code;
}
