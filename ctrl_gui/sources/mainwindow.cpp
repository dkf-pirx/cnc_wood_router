#include "mainwindow.h"
#include "settingsdialog.h"

#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_pui(new Ui::MainWindow)
{
    m_pui->setupUi(this);

    m_pCncComm = new DriverComm( );
}

MainWindow::~MainWindow()
{
    delete m_pui;

    delete m_pCncComm;
}

void MainWindow::on_connectButton_clicked( )
{
    QString router_driver( "TI0CMS0" );

    m_pCncComm->Init( router_driver );
    if ( !m_pCncComm->Connect( ) )
    {
        m_pui->connectButton->setEnabled( false );
    }
    else
    {

    }

}

void MainWindow::on_startButton_clicked( )
{
    m_pCncComm->Start( );
}

void MainWindow::on_stopButton_clicked( )
{
    m_pCncComm->Stop( );
}

void MainWindow::on_pushButton_clicked()
{
    SettingsDialog diag;
    diag.exec( );

}
