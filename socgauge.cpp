#include "socgauge.h"
#include <QTextStream>

SocGauge::SocGauge(QWidget *parent, QRect sizeLoc, int num_laps)
    : QWidget(parent)
{
    this->sizeLoc = sizeLoc;

    QPalette pal;
    pal.setColor(QPalette::Background, Qt::black);
    setAutoFillBackground(true);
    setPalette(pal);

    pal.setColor(QPalette::Foreground, Qt::white);
    number = new QLCDNumber(this);
    number->display(0);
    number->setGeometry(0, sizeLoc.height() * 0.1,
                        sizeLoc.width() - sizeLoc.height(), sizeLoc.height() * 0.8);
    number->setPalette(pal);
    //number->setSegmentStyle(QLCDNumber::Flat);
    number->setFrameStyle(QFrame::NoFrame);

    brush_color = Qt::blue;

    antialiased     = true;
    currentSoc      = 0.0;
    lineFillPct     = 0.0;
    maxLineLength   = sizeLoc.width() - 4;
    laps_remaining  = num_laps + 1;
    eor_soc         = -1;
    lap_start_soc   = -1;
}

void SocGauge::setAntialiased(bool antialiased)
{
    this->antialiased = antialiased;
    update();
}

void SocGauge::startOfLap()
{
    laps_remaining --;

    if (lap_start_soc == -1)
        lap_start_soc = currentSoc;
    else
    {
        int avg_usage = 0;
        lap_soc_usage.append(lap_start_soc - currentSoc);

        for (int i = 0; i < lap_soc_usage.length(); i++)
        {
            avg_usage += lap_soc_usage.at(i);
        }

        avg_usage /= lap_soc_usage.length();

        eor_soc = currentSoc - avg_usage*laps_remaining;

        if (eor_soc < 0)
            brush_color = Qt::red;
        else if (eor_soc < 10)
            brush_color = Qt::yellow;
        else
            brush_color = Qt::green;

        lap_start_soc = currentSoc;
    }
}

void SocGauge::setSoc(int SOC)
{
    this->currentSoc    = SOC;
    this->lineFillPct   = ( (double) (currentSoc / 100));

    if (this->lineFillPct < 0)
        this->lineFillPct = 0;

    if (currentSoc < 10)
        brush_color = Qt::red;
    else if (currentSoc < 25)
        brush_color = Qt::yellow;
    else
        brush_color = Qt::green;


    number->display( currentSoc );
    update();
}

void SocGauge::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    if (antialiased)
        painter.setRenderHint(QPainter::Antialiasing, true);

    QPoint point1(2, sizeLoc.height()/2);
    QPoint point2(2 + lineFillPct * maxLineLength, sizeLoc.height()/2);
    QLine line(point1, point2);

    if ( this->lineFillPct > 0 )
    {
        QPen pen_line(QBrush(brush_color), sizeLoc.height() - 2);
        pen_line.setCapStyle(Qt::FlatCap);
        painter.setPen(pen_line);
        painter.drawLine(line);
    }

    QPen outline(QBrush(Qt::white), 2);
    outline.setCapStyle(Qt::FlatCap);
    painter.setPen(outline);

    painter.drawRect(0, 0, sizeLoc.width(), sizeLoc.height());
}
