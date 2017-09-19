#ifndef CANREADER_H
#define CANREADER_H

#include <QWaitCondition>
#include <QThread>
#include <QMutex>
#include <QTextStream>

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <linux/can/error.h>

class CanReader : public QThread
{
    Q_OBJECT
public:
    CanReader(QObject *parent = 0);
    ~CanReader();

    bool init();

signals:
    void setRPM(const int rpm);
    void setSOC(const int soc);
    void setMtrTemp(const int mtrTemp);
    void setMtrCntrlTemp(const int mtrCntrlTemp);
    void setHighBattTemp(const int highBattTemp);

public slots:

protected:
    void run() override;

private:
    QWaitCondition condition;
    QMutex mutex;
    bool abort;

    int soc;
    int openPort(const char *port);
};

#endif // CANREADER_H
