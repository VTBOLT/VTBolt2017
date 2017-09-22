#include "cluster.h"
#include "ui_cluster.h"

#define NUM_LAPS    8

Cluster::Cluster(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Cluster)
{
    QTextStream out(stdout);

    ui->setupUi(this);

    // Sizes for BOLT III dash
    //QRect rpm_rect = QRect(20, 20, 780, 400);
    //QRect soc_rect = QRect(800, 550, 200, 50);
    //QRect lt_rect = QRect(800, 20, 200, 300);

    QRect rpm_rect = QRect(20, 20, 630, 350);
    rpm_gauge = new RpmGauge(this, rpm_rect);
    //rpm_gauge->setObjectName(QStringLiteral("rpmGauge"));
    rpm_gauge->setObjectName("rpmGauge");
    rpm_gauge->setGeometry( rpm_rect );

    QRect soc_rect = QRect(580, 400, 200, 50);
    soc_gauge = new SocGauge(this, soc_rect, NUM_LAPS);
    //soc_gauge->setObjectName(QStringLiteral("socGauge"));
    soc_gauge->setObjectName("socGauge");
    soc_gauge->setGeometry( soc_rect );

    QRect lt_rect = QRect(650, 20, 150, 300);
    lt_panel = new LapTimePanel(this, lt_rect);
    //lt_panel->setObjectName(QStringLiteral("ltPanel"));
    lt_panel->setObjectName("ltPanel");
    lt_panel->setGeometry( lt_rect );

    QPalette pal;
    pal.setColor(QPalette::Background, Qt::black);
    pal.setColor(QPalette::Foreground, Qt::white);

    ui->motor_temp->setPalette(pal);
    ui->motor_temp->setSegmentStyle(QLCDNumber::Flat);
    ui->motor_temp->setFrameStyle(QFrame::NoFrame);

    ui->mtr_cntrl_temp->setPalette(pal);
    ui->mtr_cntrl_temp->setSegmentStyle(QLCDNumber::Flat);
    ui->mtr_cntrl_temp->setFrameStyle(QFrame::NoFrame);

    ui->high_batt_temp->setPalette(pal);
    ui->high_batt_temp->setSegmentStyle(QLCDNumber::Flat);
    ui->high_batt_temp->setFrameStyle(QFrame::NoFrame);

    this->temp_filt_val   = 512;
    this->motor_temp     = 0;
    this->mtr_cntrl_temp  = 0;
    this->high_batt_temp  = 0;

    connect(&thread_CAN, SIGNAL(setRPM(int)), this, SLOT(receiveRPM(int)));
    //connect(&thread_CAN, SIGNAL(setRPM(int)), &thread_kf, SLOT(receiveRPM(int)));
    connect(&thread_CAN, SIGNAL(setSOC(int)), this, SLOT(receiveSOC(int)));
    connect(&thread_CAN, SIGNAL(setMtrTemp(int)), this, SLOT(receiveMtrTemp(int)));
    connect(&thread_CAN, SIGNAL(setMtrCntrlTemp(int)), this, SLOT(receiveMtrCntrlTemp(int)));
    connect(&thread_CAN, SIGNAL(setHighBattTemp(int)), this, SLOT(receiveHighBattTemp(int)));
/*
    connect(&thread_GPS, SIGNAL(newLatitude(double)), &thread_KF, SLOT(setLatitude(double)));
    connect(&thread_GPS, SIGNAL(newLongitude(double)), &thread_KF, SLOT(setLongitude(double)));
    connect(&thread_GPS, SIGNAL(coordsUpdated()), &thread_KF, SLOT(runIteration()));
    //connect(&thread_fp_GPS, SIGNAL(newLatitude(double)), &thread_kf, SLOT(setLatitude(double)));
    //connect(&thread_fp_GPS, SIGNAL(newLongitude(double)), &thread_kf, SLOT(setLongitude(double)));
    //connect(&thread_fp_GPS, SIGNAL(coordsUpdated()), &thread_kf, SLOT(runIteration()));

    connect(&thread_KF, SIGNAL(newLatitude(double)), &thread_LT, SLOT(receiveLatitude(double)));
    connect(&thread_KF, SIGNAL(newLongitude(double)), &thread_LT, SLOT(receiveLongitude(double)));

    //connect(&thread_LT, SIGNAL(newLapNumber(int)), this, SLOT(receiveLapNumber(int)));
    //connect(&thread_LT, SIGNAL(newLastLapTime(int)), this, SLOT(receiveLastLapTime(int)));
    //connect(&thread_LT, SIGNAL(newBestLapTime(int)), this, SLOT(receiveBestLapTime(int)));
    //connect(&thread_LT, SIGNAL(startOfLap()), this, SLOT(startOfLap()));
*/
    //thread_KF.init();
    thread_CAN.init();
    //thread_GPS.init();
    thread_LT.init();
    //thread_fp_GPS.parse();
}

Cluster::~Cluster()
{
    delete &thread_CAN;
    //delete &thread_GPS;
    delete &thread_LT;
   //delete &thread_KF;
    //delete &thread_fp_GPS;
    delete ui;
}

void Cluster::receiveSOC(const int &SOC)
{
    soc_gauge->setSoc(SOC);
    //soc_gauge->setSoc(18);
}

void Cluster::receiveRPM(const int &RPM)
{
    thread_LT.receiveRPM( RPM );
    //thread_LT.receiveRPM( 1310 );

    rpm_gauge->setRpm( RPM );
    //rpm_gauge->setRpm( 8500 );
}

void Cluster::receiveMtrTemp(const int &mtr_temp)
{
    this->motor_temp = (temp_filt_val * this->motor_temp + (1024 - temp_filt_val) * mtr_temp) >> 10;
    ui->motor_temp->display(this->motor_temp);
}

void Cluster::receiveMtrCntrlTemp(const int &mc_temp)
{
    this->mtr_cntrl_temp = (temp_filt_val * this->mtr_cntrl_temp + (1024 - temp_filt_val) * mc_temp) >> 10;
    ui->mtr_cntrl_temp->display(this->mtr_cntrl_temp);
}

void Cluster::receiveHighBattTemp(const int &hb_temp)
{
    this->high_batt_temp = (temp_filt_val * this->high_batt_temp + (1024 - temp_filt_val) * hb_temp) >> 10;
    ui->high_batt_temp->display(this->high_batt_temp);
}

void Cluster::receiveLapNumber(const int &lap_num)
{
    lt_panel->setLapNumber(lap_num);
}

void Cluster::receiveLastLapTime(const int &time)
{
    lt_panel->setLastTime(time);
}

void Cluster::receiveBestLapTime(const int &time)
{
    lt_panel->setBestTime(time);
}

void Cluster::startOfLap()
{
    soc_gauge->startOfLap();
}

