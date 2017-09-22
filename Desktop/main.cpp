#include "cluster.h"
#include "qpainter.h"
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
/*
    QGraphicsScene scene;
    QGraphicsView view(&scene);
    QGraphicsPixmapItem item( QPixmap("BOLT.png") );
    scene.addItem(&item);
    view.show();
*/
    Cluster w;
    QPalette pal;
    pal.setColor(QPalette::Background, Qt::black);
    w.setAutoFillBackground(true);
    w.setPalette(pal);
    w.setCursor(Qt::BlankCursor);
    w.setWindowState(w.windowState() ^ Qt::WindowFullScreen);
    w.show();
    return a.exec();
}
