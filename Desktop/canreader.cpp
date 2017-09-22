
#include "canreader.h"

CanReader::CanReader(QObject *parent)
    : QThread(parent)
{
    abort = false;
}


CanReader::~CanReader()
{
    mutex.lock();
    abort = true;
    condition.wakeOne();
    mutex.unlock();

    wait();
}

bool CanReader::init()
{
    if (!isRunning()) {
        start(HighestPriority);
    }

    return true;
}

void CanReader::run()
{
    QTextStream out(stdout);
    QList<int> mtrCntrlTemps;

    int RPM             = 0;
    int SOC             = 0;
    int mtrTemp         = 0;
    int moduleA         = 0;
    int moduleB         = 0;
    int moduleC         = 0;
    int gateDrvrBrd     = 0;
    int highCellTemp    = 0;

        forever{
            if(this->abort)
                return;
            RPM = 20;
            SOC                 = 5;
            mtrTemp             = 111;
            moduleA             = 17;
            moduleB             = 15;
            moduleC             = 13;
            gateDrvrBrd         = 10;
            highCellTemp        = 10;

            emit setMtrCntrlTemp( *std::max_element(mtrCntrlTemps.begin(), mtrCntrlTemps.end()) );
            emit setMtrTemp( mtrTemp );
            emit setRPM( RPM );
            emit setHighBattTemp( highCellTemp );
            emit setSOC( SOC );
        }
}

