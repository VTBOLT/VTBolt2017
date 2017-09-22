/********************************************************************************
** Form generated from reading UI file 'cluster.ui'
**
** Created by: Qt User Interface Compiler version 4.8.7
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CLUSTER_H
#define UI_CLUSTER_H

#include <QtCore/QVariant>
#include <QtGui> //ideally remove to make smaller executable, theorettically unnecessary
#include "cluster.h"
#include <QMenuBar>
#include <QStatusBar>
#include <QApplication>
/*
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLCDNumber>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QWidget>
*/
QT_BEGIN_NAMESPACE

class Ui_Cluster
{
public:
    QWidget *centralWidget;
    QLCDNumber *motor_temp;
    QLCDNumber *high_batt_temp;
    QLCDNumber *mtr_cntrl_temp;
    QMenuBar *menuBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *Cluster)
    {
        if (Cluster->objectName().isEmpty())
            Cluster->setObjectName(QString::fromUtf8("Cluster"));
        Cluster->resize(800, 480);
        centralWidget = new QWidget(Cluster);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        motor_temp = new QLCDNumber(centralWidget);
        motor_temp->setObjectName(QString::fromUtf8("motor_temp"));
        motor_temp->setGeometry(QRect(350, 400, 131, 51));
        high_batt_temp = new QLCDNumber(centralWidget);
        high_batt_temp->setObjectName(QString::fromUtf8("high_batt_temp"));
        high_batt_temp->setGeometry(QRect(10, 400, 131, 51));
        mtr_cntrl_temp = new QLCDNumber(centralWidget);
        mtr_cntrl_temp->setObjectName(QString::fromUtf8("mtr_cntrl_temp"));
        mtr_cntrl_temp->setGeometry(QRect(180, 400, 131, 51));
        Cluster->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(Cluster);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 800, 26));
        Cluster->setMenuBar(menuBar);
        statusBar = new QStatusBar(Cluster);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        Cluster->setStatusBar(statusBar);

        retranslateUi(Cluster);

        QMetaObject::connectSlotsByName(Cluster);
    } // setupUi

    void retranslateUi(QMainWindow *Cluster)
    {
        Cluster->setWindowTitle(QApplication::translate("Cluster", "Cluster", 0));
    } // retranslateUi

};

namespace Ui {
    class Cluster: public Ui_Cluster {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CLUSTER_H
