#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "bsplinewindow.h"
#include <QEventLoop>
#include <iostream>
#include <QFileDialog>
#include "applicationconfig.h"
#include <QMessageBox>
#include "helpwindow.h"
#include <QKeyEvent>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , bsplineWindow(nullptr)
    , graphicsView(nullptr)

{
    ui->setupUi(this);
    RenderConfig render("resource/StandfordBunny.render");
    SceneConfig scene("resource/StandfordBunny.scene");
    model = new WireFrameRModel(400, 400, render, scene);
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


void MainWindow::keyPressEvent(QKeyEvent * event){
    switch (event->key()) {
        case Qt::Key_W:{
            model->stepForward();
            break;
        }
        case Qt::Key_S:{
            model->stepBack();
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


void MainWindow::on_actionopen_triggered()
{
    QString file = QFileDialog::getOpenFileName(
                                this,
                                "Select scene file to open",
                                "/home",
                                "Config (*.scene)");

    if (file.size() == 0){
        return;

    }
    SceneConfig scene(file.toStdString());
    file = QFileDialog::getOpenFileName(
                                this,
                                "Select render file to open",
                                "/home",
                                "Config (*.render)");

    if (file.size() == 0){
        return;

    }
    RenderConfig render(file.toStdString());
    model->setConfigs(render, scene);
}


void MainWindow::on_actionsave_triggered()
{
//    QString file = QFileDialog::getSaveFileName(this, "select file to save", "/home", "Config (*.cfg)");
//    if (file.size() == 0){
//        return;
//    }
//    ApplicationConfig config;
//    model->recordDataToConfig(config);
//    try{
//        config.toFile(file.toStdString());
//    }catch (std::exception& e){
//        QMessageBox box(QMessageBox::Icon::Critical, "error", e.what(), QMessageBox::Ok, this);
//        box.exec();
//    }
}


struct threadArgs{
    QImage* q;
    WireFrameRModel* model;
    QProgressBar* bar;
};


void* start_routine(void * args){
    threadArgs* arg = reinterpret_cast<threadArgs*>(args);
    pthread_detach(pthread_self());
    arg->model->calcReyTracing(arg->q, arg->bar);
    delete arg;
    return NULL;
}


void MainWindow::on_actioneditor_triggered()
{
    if (bsplineWindow == nullptr){
        QImage* q = new QImage(600, 600, QImage::Format::Format_ARGB32);
        pthread_t tid;
        threadArgs* args = new threadArgs;
        args->q = q;
        args->model = model;
        bsplineWindow = new BSplineWindow(q,this);
        args->bar = bsplineWindow->getProgressBar();
        pthread_create(&tid, NULL, start_routine, args);
        bsplineWindow->show();
    }
    else {
        bsplineWindow->activateWindow();
        bsplineWindow->raise();
        return;
    }

    QEventLoop loop;

    connect(bsplineWindow, SIGNAL(closed()), &loop, SLOT(quit()));

    loop.exec();

    delete bsplineWindow;
    bsplineWindow = nullptr;

}


void MainWindow::on_actionrefresh_triggered()
{
    //model->clearAngle();
}


void MainWindow::on_actionabout_triggered()
{
    QMessageBox about(QMessageBox::Icon::Information, "about information",
                      "Program RayTracing was made by Michail Sirotkin student of Novosibirk State University FIT faculty group 21203",
                      QMessageBox::StandardButton::NoButton, this);
    about.exec();
}


void MainWindow::on_actionhelp_triggered()
{
    HelpWindow w(this);
    w.show();

    QEventLoop loop;

    connect(bsplineWindow, SIGNAL(closed()), &loop, SLOT(quit()));

    loop.exec();
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

