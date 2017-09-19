#ifndef BETAKALMANFILTER_H
#define BETAKALMANFILTER_H

#include <QWaitCondition>
#include <QThread>
#include <QMutex>
#include <QGenericMatrix>
#include <armadillo>

using namespace std;
using namespace arma;

class BetaKalmanFilter: public QThread
{
    Q_OBJECT
public:
    BetaKalmanFilter(QObject *parent = 0);
    ~BetaKalmanFilter();

    bool init();

signals:
    void newLongitude(const double longitude);
    void newLatitude(const double latitude);

private slots:
    void setLatitude(const double &latitude);
    void setLongitude(const double &longitude);
    void setRPM(const int &rpm);
    void runIteration();

protected:
    void run() override;

private:
    double longitude;
    double latitude;
    int rpm;

    mat F;
    mat H;
    mat Q;
    mat R;
    mat P;
    mat x;

    QWaitCondition condition;
    QMutex mutex;
    bool abort;
};

#endif // BETAKALMANFILTER_H
