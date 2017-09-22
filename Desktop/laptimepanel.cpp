#include "laptimepanel.h"
#include <QTextStream>

LapTimePanel::LapTimePanel(QWidget *parent, QRect size_loc)
    : QWidget(parent)
{
    this->size_loc = size_loc;

    QPalette pal;
    pal.setColor(QPalette::Foreground, Qt::white);
    pal.setColor(QPalette::Background, Qt::black);
    setAutoFillBackground(true);
    setPalette(pal);

    lap_num = new QLCDNumber(this);
    lap_num->display(0);
    lap_num->setGeometry(size_loc.width() * 0.25, 0, 100, size_loc.height() * 0.10);
    lap_num->setPalette(pal);
    lap_num->setSegmentStyle(QLCDNumber::Flat);
    lap_num->setFrameStyle(QFrame::NoFrame);

    lap_label = new QLabel(this);
    lap_label->setGeometry(0, 0, size_loc.width() * 0.5, size_loc.height() * 0.10);
    lap_label->setAutoFillBackground(true);
    lap_label->setFont( QFont("Times", 20) );
    lap_label->setText(QString("Lap: "));
    lap_label->setStyleSheet("QLabel { background-color : black; color : white; }");

    last_label = new QLabel(this);
    last_label->setGeometry(0, size_loc.height() * 0.33,  size_loc.width(), size_loc.height() * 0.15);
    last_label->setAutoFillBackground(true);
    last_label->setFont( QFont("Times", 20) );
    last_label->setText(QString("Last: "));
    last_label->setStyleSheet("QLabel { background-color : black; color : white; }");

    last_time_label = new QLabel(this);
    last_time_label->setFont( QFont("Times", 16) );
    last_time_label->setGeometry(0, size_loc.height() * 0.45,  size_loc.width(), size_loc.height() * 0.10);
    last_time_label->setAutoFillBackground(true);
    last_time_label->setText("00:00:000");
    last_time_label->setStyleSheet("QLabel { background-color : black; color : white; }");

    best_label = new QLabel(this);
    best_label->setGeometry(0, size_loc.height() * 0.66,  size_loc.width(), size_loc.height() * 0.15);
    best_label->setAutoFillBackground(true);
    best_label->setFont( QFont("Times", 20) );
    best_label->setText(QString("Best: "));
    best_label->setStyleSheet("QLabel { background-color : black; color : white; }");

    best_time_label = new QLabel(this);
    best_time_label->setFont( QFont("Times", 16) );
    best_time_label->setGeometry(0, size_loc.height() * 0.78,  size_loc.width(), size_loc.height() * 0.10);
    best_time_label->setAutoFillBackground(true);
    best_time_label->setText("00:00:000");
    best_time_label->setStyleSheet("QLabel { background-color : black; color : white; }");


    antialiased     = true;
    best_lap_time   = 0;
    last_lap_time   = 0;
    lap_number      = 0;
}

void LapTimePanel::setAntialiased(bool antialiased)
{
    this->antialiased = antialiased;
    update();
}
void setLastTime(int time);
void setBestTime(int time);
void setLapNumber(int lap_num);


void LapTimePanel::setLastTime(int time)
{
    QTextStream out(stdout);

    this->last_lap_time = time;

    int minutes = (time/1000) / 60;
    int seconds = (time/1000) - minutes*60;
    int msecs   = time - seconds * 1000;

    QString string;
    string = "%1:%2:%3";
    string = string.arg(minutes, 2, 'g', 2, QLatin1Char('0'))
            .arg(seconds, 2, 'g', QLatin1Char('0'))
            .arg(msecs, 3, 'g', 3, QLatin1Char('0'));

    last_time_label->setText(string);

    update();
}

void LapTimePanel::setBestTime(int time)
{
    this->best_lap_time = time;

    int minutes = (time/1000) / 60;
    int seconds = (time/1000) - minutes*60;
    int msecs   = time - seconds * 1000;

    QString string;
    string = "%1:%2:%3";
    string = string.arg(minutes, 2, 'g', 2, QLatin1Char('0'))
            .arg(seconds, 2, 'g', QLatin1Char('0'))
            .arg(msecs, 3, 'g', 3, QLatin1Char('0'));

    best_time_label->setText(string);

    update();
}

void LapTimePanel::setLapNumber(int l_num)
{
    this->lap_number = l_num;
    lap_num->display(lap_number);

    update();
}

void LapTimePanel::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
}
