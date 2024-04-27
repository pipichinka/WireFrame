#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "bsplinewindow.h"
#include <QEventLoop>
#include <iostream>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , bsplineWindow(nullptr)
    , graphicsView(nullptr)
    , model(new WireFrameModel(640, 480))

{
    ui->setupUi(this);
    graphicsView = new WireFrameField(ui->centralwidget, model);
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
    ui->verticalLayout_2->addWidget(graphicsView);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionopen_triggered()
{

}


void MainWindow::on_actionsave_triggered()
{

}


void MainWindow::on_actioneditor_triggered()
{
    if (bsplineWindow == nullptr){
        bsplineWindow = new BSplineWindow(model,this);
    }
    else {
        bsplineWindow->activateWindow();
        bsplineWindow->raise();
        return;
    }
    bsplineWindow->show();

    QEventLoop loop;

    connect(bsplineWindow, SIGNAL(closed()), &loop, SLOT(quit()));

    loop.exec();

    delete bsplineWindow;
    bsplineWindow = nullptr;

}


void MainWindow::on_actionrefresh_triggered()
{
    model->clearAngle();
}


void MainWindow::on_actionabout_triggered()
{

}


void MainWindow::on_actionhelp_triggered()
{

}
