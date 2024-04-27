#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "bsplinewindow.h"
#include <QEventLoop>
#include <iostream>
#include <QFileDialog>
#include "applicationconfig.h"
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
    ApplicationConfig config("resource/config.cfg");
    model->readConfig(config);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionopen_triggered()
{
    QString file = QFileDialog::getOpenFileName(
                                this,
                                "Select one file to open",
                                "/home",
                                "Config (*.cfg)");

    if (file.size() == 0){
        return;
    }
    ApplicationConfig config(file.toStdString());
    model->readConfig(config);
}


void MainWindow::on_actionsave_triggered()
{
    QString file = QFileDialog::getSaveFileName(this, "select file to save", "/home", "Config (*.cfg)");
    if (file.size() == 0){
        return;
    }
    ApplicationConfig config;
    model->recordDataToConfig(config);
    config.toFile(file.toStdString());
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




void MainWindow::on_saveButton_clicked()
{
    on_actionsave_triggered();
}


void MainWindow::on_openButton_clicked()
{
    on_actionopen_triggered();
}


void MainWindow::on_editorButton_clicked()
{
    on_actioneditor_triggered();
}


void MainWindow::on_refreshButton_clicked()
{
    on_actionrefresh_triggered();
}


void MainWindow::on_helpButton_clicked()
{
    on_actionhelp_triggered();
}


void MainWindow::on_aboutButton_clicked()
{
    on_actionabout_triggered();
}

