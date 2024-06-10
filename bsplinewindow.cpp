#include "bsplinewindow.h"
#include "ui_bsplinewindow.h"
#include "bsplinefield.h"
#include <QKeyEvent>
#include <iostream>
#include <QCloseEvent>
#include <QColorDialog>
#include <QErrorMessage>
#include <QMessageBox>
BSplineWindow::BSplineWindow(QImage* image, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BSplineWindow)
{
    ui->setupUi(this);
    graphicsView = new BSplineField(ui->centralwidget, image);
    graphicsView->setObjectName(QString::fromUtf8("graphicsView"));
    QSizePolicy sizePolicy3(QSizePolicy::Expanding, QSizePolicy::Expanding);
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
    delete graphicsView;
    delete ui;
}

void BSplineWindow::closeEvent(QCloseEvent* event){
    emit closed();
    event->accept();
    //QMainWindow::closeEvent(event);
}


void BSplineWindow::updateImage(){
    graphicsView->updateImage();
}

QProgressBar* BSplineWindow::getProgressBar(){
    return ui->progressBar;
}


void BSplineWindow::on_progressBar_valueChanged(int value)
{
    if (value == 100){
        updateImage();
    }
}

