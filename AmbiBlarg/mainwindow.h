#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "arduinocommunication.h"

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void updateStatusLabel(ConnectionStatus status);

//protected:
//    void closeEvent(QCloseEvent *) Q_DECL_OVERRIDE;

    void on_testButton_clicked();

    void on_rescanButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
