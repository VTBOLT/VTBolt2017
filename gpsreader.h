#ifndef GPSREADER_H
#define GPSREADER_H

#include <QWaitCondition>
#include <QThread>
#include <QMutex>
#include <QTextStream>
#include <QtSerialPort/QSerialPort>
#include <qserialportinfo.h>
#include <QFile>


class GpsReader : public QThread
{
    Q_OBJECT
public:
    GpsReader(QObject *parent = 0);
    ~GpsReader();

    bool init();

signals:
    void newLatitude(const double latitude);
    void newLongitude(const double longitude);
    void newSpeed(const double speed);
    void newHeading(const double heading);
    void coordsUpdated();

public slots:

protected:
    void run() override;

private:
    QWaitCondition condition;
    QMutex mutex;
    QFile *gps_output;
    QTextStream text_stream;

    bool abort;
};

#endif // GPSREADER_H
