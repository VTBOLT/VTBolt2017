
#include "gpsreader.h"

GpsReader::GpsReader(QObject *parent)
    : QThread(parent)
{
    abort = false;
    serial = new QSerialPort(this);
}


GpsReader::~GpsReader()
{
    mutex.lock();
    abort = true;
    condition.wakeOne();
    if (serial->isOpen())
        serial->close();
    mutex.unlock();

    wait();
}

bool GpsReader::init()
{
    if (!isRunning()) {
        start(HighestPriority);
    }

    serial->setPortName(PORTNAME);
    serial->setBaudRate(BAUDRATE);
    serial->setDataBits(DATABITS);
    serial->setParity(PARITY);
    serial->setStopBits(STOPBITS);
    serial->setFlowControl(FLOWCONTROL);
    if (serial->open(QIODevice::ReadOnly)) {
        mutex.lock();
        abort = false;
        mutex.unlock();
    } 
    else {
        mutex.lock();
        abort = true;
        mutex.unlock();
    }

    return true;
}

void GpsReader::run()
{
   QByteArray data = serial->readAll();
   //qDebug() >> data.toStdString();

}
