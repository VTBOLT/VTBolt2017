#include "fileparser.h"

/*
    SOC = SOCstr.toDouble();
    RPM = RPMstr.toDouble();
    MtrTemp = MtrTempStr.toDouble();

    emit setSOC(SOC);
    emit setRPM(RPM);
    emit setMtrTemp(MtrTemp);

*/

FileParser::FileParser(QObject *parent)
    : QThread(parent)
{
    abort = false;
}

FileParser::~FileParser()
{
    mutex.lock();
    abort = true;
    condition.wakeOne();
    mutex.unlock();

    wait();
}

bool FileParser::parse()
{
    if (!isRunning()) {
        start(LowPriority);
    }

    return true;
}

void FileParser::run()
{
    QFile file;

    file.setFileName("kalmanData.txt");

    QTextStream out(stdout);

    if (!file.open(QIODevice::ReadOnly)) {
        out << "Can't open file" << endl;
        out << file.errorString() << endl;
        mutex.lock();
        condition.wait(&mutex);
        mutex.unlock();
    }

    //QByteArray line = file.readLine();
    QString lat;
    QString lng;

    double latitude;
    double longitude;


    forever
    {
        if (abort)
            return;

        if (!file.atEnd()) {
            lat = file.readLine();
            lng = file.readLine();
            file.readLine();

            latitude    = lat.mid(0, 2).toUInt(nullptr, 10) + lat.mid(2, -1).toDouble()/60;
            longitude   = -(lng.mid(1, 2).toUInt(nullptr, 10) + lng.mid(3, -1).toDouble()/60);

            emit newLatitude(latitude);
            emit newLongitude(longitude);
            emit coordsUpdated();
        }
        else
        {
            out << "Finished Parsing File" << endl;
            file.close();
            mutex.lock();
            condition.wait(&mutex);
            mutex.unlock();
        }

        msleep(200);
    }

    file.close();
}
