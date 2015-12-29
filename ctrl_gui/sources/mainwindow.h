#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "drivercomm.h"


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
    void on_connectButton_clicked();

    void on_startButton_clicked();

    void on_stopButton_clicked();

    void on_pushButton_clicked();

signals:
    void    connectButtonClicked( void );
    void    startButtonClicked( void );
    void    stopButtonClicked( void );


private:
    Ui::MainWindow*     m_pui;

    DriverComm*         m_pCncComm;

};

#endif // MAINWINDOW_H
