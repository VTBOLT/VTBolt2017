#include "rpmgauge.h"
#include <QTextStream>

//#define ARC_TO_TOTAL_RATIO    0.386
#define MAGIC_RATIO             1.6780972451 //( 1/2 + 3*pi/8 )
#define ARC_TO_TOTAL_RATIO      0.5
#define RPM_FILTER_VAL          800
#define MAX_RPM                 8500    // must be less than 1024
#define GRADIENT_ANGLE          0      // 90 < GRADIENT_ANGLE <= 0
#define MIN(X, Y) (((X) < (Y)) ? (X) : (Y))

RpmGauge::RpmGauge(QWidget *parent, QRect sizeLoc)
    : QWidget(parent)
{
    this->sizeLoc = sizeLoc;

    QPalette pal;
    pal.setColor(QPalette::Background, Qt::black);
    pal.setColor(QPalette::Foreground, Qt::white);
    setAutoFillBackground(true);
    setPalette(pal);

    antialiased     = true;
    lineFillPct     = 0.0;
    currentRpm      = 0;
    startAngle      = 225 * 16;
    spanAngle       = 0;
    arcBoxWidth     = MIN( sizeLoc.width()/MAGIC_RATIO, sizeLoc.height() );
    maxLineLength   = 0.75 * M_PI * arcBoxWidth / 2;
    brushWidth      = 0.1 * arcBoxWidth;

    double gradProp = (360. - (90. - GRADIENT_ANGLE))/360.;
    lineStartRed    = 255 + ((255-255) * (1.0-gradProp));
    lineStartGreen  = 102 + ((255-102) * (1.0-gradProp));
    lineStartBlue   = 0   + ((255-0  ) * (1.0-gradProp));

    number = new QLCDNumber(this);
    number->display(0);
    number->setGeometry(arcBoxWidth/3, arcBoxWidth/4, arcBoxWidth*3/4, arcBoxWidth/2);
    number->setPalette(pal);
    number->setSegmentStyle(QLCDNumber::Flat);
    number->setFrameStyle(QFrame::NoFrame);
}

void RpmGauge::setAntialiased(bool antialiased)
{
    this->antialiased = antialiased;
    update();
}

void RpmGauge::setRpm(int RPM)
{
    // RPM low-pass filter
    currentRpm = (RPM_FILTER_VAL * currentRpm + (1024 - RPM_FILTER_VAL) * RPM) >> 10;
    if (currentRpm < 0)
        currentRpm = 0;

    this->spanAngle     = -( (double) currentRpm / (MAX_RPM*ARC_TO_TOTAL_RATIO) ) * 135*16;
    this->lineFillPct   =  ( (double) (currentRpm - MAX_RPM*ARC_TO_TOTAL_RATIO)/(MAX_RPM * (1-ARC_TO_TOTAL_RATIO)));

    if (this->spanAngle > 0)
        this->spanAngle = 0;
    else if (this->spanAngle < -135*16)
        this->spanAngle = -135*16;

    if (this->lineFillPct < 0)
        this->lineFillPct = 0;

    number->display( 10* ( (int)currentRpm/10 ));
    update();
}

void RpmGauge::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    if (antialiased)
        painter.setRenderHint(QPainter::Antialiasing, true);

    QRect arcBoxRect( 0+1, 0+1, arcBoxWidth, arcBoxWidth);
    QRect arcFillRect( brushWidth / 2+1, brushWidth / 2+1,
                       arcBoxWidth - brushWidth, arcBoxWidth - brushWidth);
    QConicalGradient gradient_arc;
    gradient_arc.setCenter(arcBoxRect.center());
    gradient_arc.setAngle(GRADIENT_ANGLE);
    gradient_arc.setColorAt(0, QColor(255, 102, 0));        // GO Hokies!
    gradient_arc.setColorAt(1, QColor(255, 255, 255));

    QPen pen_arc(QBrush(gradient_arc), brushWidth);
    pen_arc.setCapStyle(Qt::FlatCap);
    painter.setPen(pen_arc);
    painter.drawArc(arcFillRect, startAngle, spanAngle);

    if ( this->lineFillPct > 0 )
    {
        QPoint point1(arcBoxWidth / 2, brushWidth / 2+1);
        QPoint point2(arcBoxWidth / 2 + lineFillPct * maxLineLength, brushWidth / 2 +1);
        QLine line(point1, point2);

        QLinearGradient gradient_line;
        gradient_line.setStart(arcBoxWidth / 2+1, brushWidth / 2 +1);
        gradient_line.setFinalStop(arcBoxWidth / 2 + maxLineLength+1, brushWidth / 2 +1);
        gradient_line.setColorAt(0, QColor( lineStartRed, lineStartGreen, lineStartBlue));
        gradient_line.setColorAt(1, QColor(255, 102, 0));

        QPen pen_line(QBrush(gradient_line), brushWidth);
        pen_line.setCapStyle(Qt::FlatCap);
        painter.setPen(pen_line);
        painter.drawLine(line);
    }

    QPen outline(QBrush(Qt::white), 2);
    outline.setCapStyle(Qt::FlatCap);
    painter.setPen(outline);

    QRectF rect_inline(brushWidth+1, brushWidth+1, arcBoxWidth - 2*brushWidth, arcBoxWidth - 2 * brushWidth);

    painter.drawLine( arcBoxWidth/2 + maxLineLength+1, 0, arcBoxWidth/2 + maxLineLength+1, brushWidth +1);
    painter.drawLine( arcBoxWidth/2+1, 0+1, arcBoxWidth/2 + maxLineLength+1, 0+1 );
    painter.drawLine( arcBoxWidth/2+1, brushWidth+1, arcBoxWidth/2 + maxLineLength+1, brushWidth +1);
    painter.drawLine( arcBoxWidth/2 - (int)(0.7071*arcBoxWidth/2) -1, arcBoxWidth/2 + (int)(0.7071*arcBoxWidth/2),
                      arcBoxWidth/2 - (int)(0.7071*(arcBoxWidth/2 - brushWidth)), arcBoxWidth/2 + (int)(0.7071*(arcBoxWidth/2 - brushWidth)) -1);
    painter.drawArc(arcBoxRect, startAngle-16, -135*16);
    painter.drawArc(rect_inline, startAngle-16, -135*16);
}
