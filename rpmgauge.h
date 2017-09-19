#ifndef RPMGAUGE_H
#define RPMGAUGE_H

#include <QObject>
#include <QWidget>
#include <QSize>
#include <QLCDNumber>
#include <QLabel>

#include "qpainter.h"

class RpmGauge : public QWidget
{
    Q_OBJECT

public:
    RpmGauge(QWidget *parent = 0, QRect sizeLoc = QRect());

public slots:
    void setAntialiased(bool antialiased);
    void setRpm(int RPM);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    bool antialiased;
    QLCDNumber  *number;
    QLabel      *rpm_text;
    QRect       sizeLoc;

    double lineFillPct;
    int arcBoxWidth;
    int brushWidth;
    int currentRpm;
    int spanAngle;
    int startAngle;
    int maxLineLength;
    int lineStartRed;
    int lineStartGreen;
    int lineStartBlue;
};

#endif // RPMGAUGE_H
