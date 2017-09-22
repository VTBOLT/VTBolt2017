#ifndef FILEPARSER_H
#define FILEPARSER_H

#include <QWaitCondition>
#include <QTextStream>
#include <QDataStream>
#include <QThread>
#include <QMutex>
#include <QFile>

class FileParser : public QThread
{
    Q_OBJECT

public:
    FileParser(QObject *parent = 0);
    ~FileParser();

    bool parse();

signals:
    void parsedData(const QByteArray &data);
    void setSOC(const int SOC);
    void setRPM(const int SOC);
    void setMtrTemp(const double SOC);

    void newLatitude(const double latitude);
    void newLongitude(const double longitude);
    void coordsUpdated();

protected:
    void run() override;

private:
    QWaitCondition condition;
    QMutex mutex;
    bool abort;
};

#endif // FILEPARSER_H
