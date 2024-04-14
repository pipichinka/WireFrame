#include "bsplinewindow.h"
#include "ui_bsplinewindow.h"
#include "bsplinefield.h"
#include <QKeyEvent>
#include <iostream>
BSplineWindow::BSplineWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BSplineWindow)
{
    model = new BSplineModel(640,480);
    ui->setupUi(this);
    graphicsView = new BSplineField(ui->centralwidget, model);
    graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
    QSizePolicy sizePolicy3(QSizePolicy::Preferred, QSizePolicy::Preferred);
    sizePolicy3.setHorizontalStretch(0);
    sizePolicy3.setVerticalStretch(0);
    sizePolicy3.setHeightForWidth(graphicsView->sizePolicy().hasHeightForWidth());
    graphicsView->setSizePolicy(sizePolicy3);
    graphicsView->setMouseTracking(true);
    graphicsView->setTabletTracking(false);
    graphicsView->setLayoutDirection(Qt::LeftToRight);
    graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    graphicsView->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    graphicsView->setInteractive(false);
    graphicsView->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
    ui->verticalLayout->addWidget(graphicsView);

    this->clearFocus();
}

BSplineWindow::~BSplineWindow()
{
    delete ui;
    delete graphicsView;
}


void BSplineWindow::keyPressEvent(QKeyEvent * event){
    switch (event->key()) {
        case Qt::Key_W:{
            model->stepUp();
            break;
        }
        case Qt::Key_S:{
            model->stepDown();
            break;
        }
        case Qt::Key_A:{
            model->stepLeft();
            break;
        }
        case Qt::Key_D:{
            model->stepRight();
            break;
        }

    }
}


void BSplineWindow::on_spinBoxN_valueChanged(int value)
{
    model->setN(value);
}


void BSplineWindow::on_zoomInButton_clicked()
{
    model->zoomIn();
}


void BSplineWindow::on_zoomOutButton_clicked()
{
    model->zoomOut();
}


void BSplineWindow::on_normalizeButton_clicked()
{
    model->normalize();
}

