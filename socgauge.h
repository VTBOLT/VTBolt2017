#ifndef SOCGAUGE_H
#define SOCGAUGE_H

#include <QObject>
#include <QWidget>
#include <QSize>
#include <QLCDNumber>

#include "qpainter.h"

class SocGauge : public QWidget
{
    Q_OBJECT

public:
    SocGauge(QWidget *parent = 0, QRect sizeLoc = QRect(), int num_laps = 8);

public slots:
    void setAntialiased(bool antialiased);
    void setSoc(int SOC);
    void startOfLap();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    bool antialiased;
    QLCDNumber *number;
    QRect sizeLoc;
    QList<int> lap_soc_usage;
    QColor brush_color;

    double lineFillPct;
    double currentSoc;
    int maxLineLength;
    int eor_soc;
    int laps_remaining;
    int lap_start_soc;
};

#endif // SOCGAUGE_H
