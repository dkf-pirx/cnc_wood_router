#ifndef DRIVERCOMM_H
#define DRIVERCOMM_H

#include <QObject>
#include <QDebug>


#include <QSerialPort>
#include <QSerialPortInfo>


class DriverComm : public QObject
{
    Q_OBJECT
public:
    explicit DriverComm( QObject *parent = 0 );
    ~DriverComm( );

    // returns false if initialization was successful and comm port has been found.
    // returns true if comm port to stepper motor driver was not found.
    bool    Init( QString deviceNumber );
    // return false if device connected to the stepper motor driver successful.
    bool    Connect( );

    bool    ConfigureStepper( quint16 ui16StartSpeed, quint16 ui16NominalSpeed );
    bool    Start(quint8 ui8Configuration, quint8 ui8AccelRate, quint8 ui8TimeBase_ms, quint8 ui8AccelChange );
    bool    Stop( );

private:
    QSerialPort*        m_pCommPort;
};

#endif // DRIVERCOMM_H
