#ifndef CLUSTER_H
#define CLUSTER_H

#include <QMainWindow>
#include "fileparser.h"
#include "alphakalmanfilter.h"
#include "canreader.h"
#include "gpsreader.h"
#include "rpmgauge.h"
#include "socgauge.h"
#include "laptiming.h"
#include "laptimepanel.h"

namespace Ui {
class Cluster;
}

class Cluster : public QMainWindow
{
    Q_OBJECT

public:
    explicit Cluster(QWidget *parent = 0);
    ~Cluster();

private slots:
    void receiveSOC(const int &SOC);
    void receiveRPM(const int &RPM);
    void receiveMtrTemp(const int &mtr_temp);
    void receiveMtrCntrlTemp(const int &mc_temp);
    void receiveHighBattTemp(const int &hb_temp);
    void receiveLapNumber(const int &lap_num);
    void receiveLastLapTime(const int &time);
    void receiveBestLapTime(const int &time);
    void startOfLap();

private:
    Ui::Cluster         *ui;
    RpmGauge            *rpm_gauge;
    SocGauge            *soc_gauge;
    LapTimePanel        *lt_panel;
    //TempGauge           *temp_gauge;
    QWidget             *image_label;

    LapTiming           thread_LT;
    CanReader           thread_CAN;
    GpsReader           thread_GPS;
    FileParser          thread_fp_GPS;
    AlphaKalmanFilter   thread_KF;

    int                 temp_filt_val;
    int                 motor_temp;
    int                 mtr_cntrl_temp;
    int                 high_batt_temp;
};

#endif // CLUSTER_H
