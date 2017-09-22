#ifndef LAPTIMING_H
#define LAPTIMING_H

#include <QWaitCondition>
#include <QThread>
#include <QMutex>
#include <QTextStream>
#include <QElapsedTimer>
#include <QTime>
#include <math.h>
#include <cmath>

class LapTiming : public QThread
{
    Q_OBJECT
public:
    LapTiming(QObject *parent = 0);
    ~LapTiming();

    bool init();

signals:
    void newLastLapTime(const int time);
    void newBestLapTime(const int time);
    void newLapNumber(const int lap_num);
    void startOfLap();

public slots:
    void receiveRPM(const int rpm);
    void receiveLongitude(const double &longitude);
    void receiveLatitude(const double &latitude);

protected:
    void run() override;

private:
    QWaitCondition  condition;
    QMutex          mutex;
    bool            abort;

    bool            race_started;
    bool            latUpdated;
    bool            longUpdated;
    int             lp_speed;
    int             lap_index;
    int             p_time, c_time;
    int             ms_at_line, ms_after_line;
    int             lapTimes[20];
    int             bestLap;
    double          p_lat, p_long, c_lat, c_long;
    double          lat_line1, long_line1, lat_line2, long_line2;
    double          lat_intersect, long_intersect;
    double          lat_dist, long_dist;
    QElapsedTimer   timer;

    double          rpmToMetersPerSec(int rpm);
    int             timeToCoverDist(double lat1, double long1, double lat2, double long2, double avg_spd);
    bool            checkIntersection(double p0_x, double p0_y, double p1_x, double p1_y,
                                      double p2_x, double p2_y, double p3_x, double p3_y, double &i_x, double &i_y);
};


#endif // LAPTIMING_H
