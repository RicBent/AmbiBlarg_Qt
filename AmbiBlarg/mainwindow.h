#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>
#include <QSerialPortInfo>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *) Q_DECL_OVERRIDE;

public slots:
    void doStuff();

private slots:
    void on_testButton_clicked();

private:
    Ui::MainWindow *ui;

    QSerialPort* serial_port;

    void writeColor(QColor color);
};

#endif // MAINWINDOW_H
