#include <QDateTime>

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


void MainWindow::on_buttonCommSettings_clicked( )
{
    SettingsDialog diag;
    diag.exec( );

}

void MainWindow::on_buttonConnect_clicked( )
{
    QString router_driver( "TI0CMS0" );

    m_pCncComm->Init( router_driver );
    if ( !m_pCncComm->Connect( ) )
    {
        m_pui->buttonConnect->setEnabled( false );

        m_pui->buttonStart->setEnabled( true );
        m_pui->buttonStop->setEnabled( true );
    }
    else
    {

    }
}

void MainWindow::on_buttonStart_clicked( )
{
    quint16  ui16StartSpeed   = m_pui->spinStartSpeed->value( );
    quint16  ui16NominalSpeed = m_pui->spinNominalSpeed->value( );
    quint8   ui8AccelRate     = m_pui->spinAccelRate->value( );
    quint8   ui8TimeBase_ms   = m_pui->spinTimeBase->value( );
    quint8   ui8AccelChange   = m_pui->spinAccelChange->value( );

    m_pCncComm->ConfigureStepper( ui16StartSpeed, ui16NominalSpeed );
    m_pCncComm->Start( 0xC0, ui8AccelRate, ui8TimeBase_ms, ui8AccelChange );

    QDateTime   currentDateAndTime = QDateTime::currentDateTime( );
    m_pui->plainLogText->appendPlainText( currentDateAndTime.toString( "hh:mm:ss.zzz") );
}

void MainWindow::on_buttonStop_clicked( )
{
    m_pCncComm->Stop( );
}
