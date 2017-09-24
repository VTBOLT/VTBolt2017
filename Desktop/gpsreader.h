#ifndef GPSREADER_H
#define GPSREADER_H

#include <QWaitCondition>
#include <QThread>
#include <QMutex>
#include <QSerialPort>
#include <QIODevice>


#define PORTNAME "/dev/tty-usbserial"
#define BAUDRATE QSerialPort::Baud9600
#define DATABITS QSerialPort::Data8
#define PARITY QSerialPort::NoParity
#define STOPBITS QSerialPort::OneStop
#define FLOWCONTROL QSerialPort::NoFlowControl



class GpsReader : public QThread
{
    Q_OBJECT
public:
    GpsReader(QObject *parent = 0);
    ~GpsReader();

    bool init();

signals:
    void setLat(const double lat);
    void setLon(const double lon);
    void setYaw(const double Yaw);

public slots:

protected:
    void run() override;

private:
    QWaitCondition condition;
    QMutex mutex;
    bool abort;

     QSerialPort *serial;
};

#endif // GPSREADER_H
