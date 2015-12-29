#include "drivercomm.h"

DriverComm::DriverComm( QObject* parent )
    : QObject( parent ),
      m_pCommPort( 0x0 )
{

}

DriverComm::~DriverComm( )
{
    if ( m_pCommPort != 0x0 )
    {
        m_pCommPort->close( );
    }
}

bool DriverComm::Init( QString deviceNumber )
{
    foreach ( const QSerialPortInfo &info, QSerialPortInfo::availablePorts( ) )
    {
        qDebug() << "Name : " << info.portName( );
        qDebug() << "Description : " << info.description( );
        qDebug() << "Manufacturer: " << info.manufacturer( );
        qDebug() << "Serial number: " << info.serialNumber( );


        if ( info.serialNumber( ) == deviceNumber )
        {
            m_pCommPort = new QSerialPort;
            m_pCommPort->setPort( info );
        }
        else
        {
            m_pCommPort = 0x0;
        }
    }

return ( m_pCommPort == 0x0 ); // return true if port was not found
}

bool DriverComm::Connect( )
{
    bool                returnError = true;

    if ( m_pCommPort != 0x0 )
    {
        m_pCommPort->setBaudRate( QSerialPort::Baud9600 );
        m_pCommPort->setDataBits( QSerialPort::Data8 );
        m_pCommPort->setParity( QSerialPort::NoParity );
        m_pCommPort->setStopBits( QSerialPort::OneStop );
        m_pCommPort->setFlowControl( QSerialPort::NoFlowControl );

        if ( m_pCommPort->open( QIODevice::ReadWrite ) )
        {
            // Set EditBox to green and text connected
        }
        else
        {
            // make it red and write error
        }

        returnError = false;
    }
    else
    {
        returnError = true;
    }

    return returnError;
}

bool DriverComm::Start( )
{
    QByteArray  startCmd( 5, 0x0 );
    startCmd[0] = 0x12;
    startCmd[1] = 0xC0;
    startCmd[2] = 0x32;
    startCmd[3] = 0x01;
    startCmd[4] = 0x05;

    m_pCommPort->write( startCmd );

return false;
}

bool DriverComm::Stop()
{
    QByteArray  stopCmd( 5, 0x0 );

    m_pCommPort->write( stopCmd );

return false;
}

