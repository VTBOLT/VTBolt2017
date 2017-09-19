#include "alphakalmanfilter.h"


AlphaKalmanFilter::AlphaKalmanFilter(QObject *parent)
    : QThread(parent)
{
    this->latitude  = 999;
    this->longitude = 999;

    F = eye<mat>(2,2);
    H = eye<mat>(2,2);
    Q = eye<mat>(2,2) * 0.001;
    R = eye<mat>(2,2) * 0.1;
    P = eye<mat>(2,2);

    QString filename = "kalmanOut.txt";
    kalman_output = new QFile(filename);
    if ( kalman_output->open(QIODevice::ReadWrite) )
    {
        text_stream.setDevice(kalman_output);
    }


    abort = false;
}

AlphaKalmanFilter::~AlphaKalmanFilter()
{
    mutex.lock();
    abort = true;
    condition.wakeOne();
    mutex.unlock();

    wait();
}

bool AlphaKalmanFilter::init()
{
    if (!isRunning()) {
        start(LowPriority);
    }

    return true;
}

void AlphaKalmanFilter::run()
{
    QTextStream out(stdout);
    double latToSend;
    double longToSend;

    mat K;

    mat u0(2,1);
    mat u1(2,1);
    mat cov0(2,2);
    mat cov1(2,2);

    mat x(2,1);

    mutex.lock();
    condition.wait(&mutex);
    mutex.unlock();

    x(0,0) = this->longitude;
    x(1,0) = this->latitude;

    forever
    {
        mutex.lock();
        condition.wait(&mutex);
        mutex.unlock();

        if ( this->abort )
            return;

        // Predict
        x = F * x;
        P = F * P * F.t() + Q;

        // Update
        u0      = H * x;
        cov0    = H * P * H.t();

        u1(0,0) = this->longitude;
        u1(1,0) = this->latitude;
        cov1    = R;

        K       = (P * H.t()) * (cov0 + cov1).i();

        x       = x + K * (u1 - u0);
        P       = P - K * H * P;

        longToSend = x(0,0);
        latToSend = x(1,0);

        emit newLongitude( longToSend );
        emit newLatitude( latToSend );

        text_stream << longitude << endl;
        text_stream << latitude << endl;
        text_stream << endl;

        out << "Kalman Values" << endl;
        out << longitude << endl;
        out << latitude << endl;
        out << endl;
    }
}


void AlphaKalmanFilter::setLongitude(const double &longitude)
{
    this->longitude = longitude;
}

void AlphaKalmanFilter::setLatitude(const double &latitude)
{
    this->latitude = latitude;
}

void AlphaKalmanFilter::runIteration()
{
    mutex.lock();
    condition.wakeOne();
    mutex.unlock();
}
