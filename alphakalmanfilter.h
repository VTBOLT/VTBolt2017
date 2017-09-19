#ifndef ALPHAKALMANFILTER_H
#define ALPHAKALMANFILTER_H

#include <QWaitCondition>
#include <QThread>
#include <QMutex>
#include <QGenericMatrix>
#include <QFile>
#include <armadillo>

using namespace std;
using namespace arma;

class AlphaKalmanFilter: public QThread
{
    Q_OBJECT
public:
    AlphaKalmanFilter(QObject *parent = 0);
    ~AlphaKalmanFilter();

    bool init();

signals:
    void newLongitude(const double longitude);
    void newLatitude(const double latitude);

private slots:
    void setLatitude(const double &latitude);
    void setLongitude(const double &longitude);
    void runIteration();

protected:
    void run() override;

private:
    double longitude;
    double latitude;

    mat F;
    mat H;
    mat Q;
    mat R;
    mat P;
    mat x;

    QWaitCondition condition;
    QMutex mutex;
    QFile *kalman_output;
    QTextStream text_stream;
    bool abort;
};

#endif // ALPHAKALMANFILTER_H
