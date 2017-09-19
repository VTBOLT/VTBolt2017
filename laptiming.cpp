#include "laptiming.h"

// Royal Oak Circuit
/*
#define LAT_LINE1               42.495068
#define LONG_LINE1             -83.118671
#define LAT_LINE2               42.495061
#define LONG_LINE2             -83.118897
*/

// NJMP Thunder
/*
#define LAT_LINE1               39.360691
#define LONG_LINE1             -75.073677
#define LAT_LINE2               39.360850
#define LONG_LINE2             -75.073559
*/

// NJMP Paddock
#define LAT_LINE1               39.362968
#define LONG_LINE1             -75.074576
#define LAT_LINE2               39.363098
#define LONG_LINE2             -75.074490


#define RPM_2_MPS               0.008531903295
#define RPM_FILTER_VAL          800

LapTiming::LapTiming(QObject *parent)
    : QThread(parent){

    lat_line1       = LAT_LINE1;
    long_line1      = LONG_LINE1;
    lat_line2       = LAT_LINE2;
    long_line2      = LONG_LINE2;

    latUpdated      = false;
    longUpdated     = false;
    lap_index       = 0;
    ms_at_line      = 0;
    ms_after_line   = 0;
    p_time          = 0;
    c_time          = 0;

    lp_speed        = 0;
    p_lat           = 0;
    p_long          = 0;
    c_lat           = 0;
    c_long          = 0;
    lat_intersect   = 0;
    long_intersect  = 0;

    double avg_lat = ( lat_line1 + lat_line2 ) / 2;
    avg_lat = avg_lat * M_PI / 180.0;

    lat_dist    = 111132.92 - 559.82*cos( 2*avg_lat )
                            + 1.175*cos( 4*avg_lat )
                            - 0.0023*cos( 6*avg_lat );

    long_dist   = 111412.84*cos( avg_lat )
                            -93.5*cos( 3*avg_lat )
                            + 0.118*cos( 5*avg_lat );

    timer.start();

    abort = false;
}

LapTiming::~LapTiming()
{
    mutex.lock();
    abort = true;
    condition.wakeOne();
    mutex.unlock();

    wait();
}


bool LapTiming::init()
{
    if (!isRunning()) {
        start(LowPriority);
    }

    return true;
}


void LapTiming::run()
{
    QTextStream out(stdout);

    forever
    {
        mutex.lock();
        condition.wait(&mutex);
        mutex.unlock();

        if ( this->abort )
            return;

        p_time = c_time;
        c_time = timer.elapsed();

        //out << "Ran Loop" << endl;
        //out << "Latitiude:\t" << c_lat << endl;
        //out << "Longitude:\t" << c_long << endl << endl;

        if ( checkIntersection(p_long, p_lat, c_long, c_lat,
                               long_line1, lat_line1, long_line2, lat_line2,
                               long_intersect, lat_intersect) )
        {
            timer.restart();
            emit startOfLap();

            ms_at_line = p_time + timeToCoverDist(p_lat, p_long, lat_intersect, long_intersect, lp_speed);

            if ( race_started )
            {

                int newLapTime = ms_at_line + ms_after_line;

                lapTimes[lap_index] = newLapTime;

                emit newLastLapTime( newLapTime );

                if ( lap_index == 0 || newLapTime < bestLap )
                {
                    bestLap = newLapTime;
                    emit newBestLapTime( newLapTime );
                    //out << "New Best Lap" << endl;
                }

                lap_index++;

                emit newLapNumber(lap_index+1);
                //out << newLapTime << endl;
                //out << "Lap Number: " << 1 + lap_index << endl;
            }
            else
            {
                race_started = true;
                emit newLapNumber(lap_index+1);
                //out << "Lap Number: " << 1 + lap_index << endl;
            }

            ms_after_line = c_time - ms_at_line;
        }
    }
}

void LapTiming::receiveRPM(const int rpm)
{
    // RPM low-pass filter
    lp_speed = (RPM_FILTER_VAL * lp_speed + (1024 - RPM_FILTER_VAL) * rpm) >> 10;
    if (lp_speed < 0)
        lp_speed = 0;
}

void LapTiming::receiveLatitude(const double &latitude)
{
    p_lat = c_lat;
    c_lat = latitude;

    mutex.lock();

    latUpdated = true;

    if ( longUpdated )
    {
        longUpdated = false;
        latUpdated = false;

        condition.wakeOne();
    }

    mutex.unlock();
}

void LapTiming::receiveLongitude(const double &longitude)
{
    p_long = c_long;
    c_long = longitude;

    mutex.lock();

    longUpdated = true;

    if ( latUpdated )
    {
        latUpdated = false;
        longUpdated = false;

        condition.wakeOne();
    }

    mutex.unlock();
}

// converts RPM to meters per second
double LapTiming::rpmToMetersPerSec(int rpm)
{
    return (double)rpm * RPM_2_MPS;
}

// returns the time in milliseconds to cover the distance between two geographical points
int LapTiming::timeToCoverDist(double lat1, double long1, double lat2, double long2, double avg_spd)
{
    double dist = sqrt( pow((abs(lat2-lat1)*lat_dist),2) + pow((abs(long2-long1)*long_dist),2));
    return (int)(1000 * dist / avg_spd);
}

// Returns 1 if the lines intersect, otherwise 0. In addition, if the lines
// intersect the intersection point may be stored in the doubles i_x and i_y.
bool LapTiming::checkIntersection(double p0_x, double p0_y, double p1_x, double p1_y,
                                  double p2_x, double p2_y, double p3_x, double p3_y, double &i_x, double &i_y)
{
    QTextStream out(stdout);

    double s1_x, s1_y, s2_x, s2_y;
    s1_x = p1_x - p0_x;     s1_y = p1_y - p0_y;
    s2_x = p3_x - p2_x;     s2_y = p3_y - p2_y;

    double s, t;
    s = (-s1_y * (p0_x - p2_x) + s1_x * (p0_y - p2_y)) / (-s2_x * s1_y + s1_x * s2_y);
    t = ( s2_x * (p0_y - p2_y) - s2_y * (p0_x - p2_x)) / (-s2_x * s1_y + s1_x * s2_y);

    if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
    {
        // Collision detected
        i_x = p0_x + (t * s1_x);
        i_y = p0_y + (t * s1_y);

        return true;
    }

    return false; // No collision
}
