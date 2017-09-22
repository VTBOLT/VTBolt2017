#ifndef LAPTIMEPANEL_H
#define LAPTIMEPANEL_H

#include <QObject>
#include <QWidget>
#include <QSize>
#include <QLCDNumber>
#include <QLabel>

#include "qpainter.h"

class LapTimePanel : public QWidget
{
    Q_OBJECT

public:
    LapTimePanel(QWidget *parent = 0, QRect size_loc = QRect());

public slots:
    void setAntialiased(bool antialiased);
    void setLastTime(int time);
    void setBestTime(int time);
    void setLapNumber(int l_num);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    bool        antialiased;
    QLCDNumber  *lap_num;
    QLabel      *lap_label;
    QLabel      *last_label;
    QLabel      *best_label;
    QLabel      *last_time_label;
    QLabel      *best_time_label;
    QRect       size_loc;

    int         best_lap_time;
    int         last_lap_time;
    int         lap_number;
};

#endif // LAPTIMEPANEL_H
