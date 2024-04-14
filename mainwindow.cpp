#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "bsplinewindow.h"
#include <QEventLoop>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
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
    BSplineWindow *w = new BSplineWindow(this);
    w->show();
}


void MainWindow::on_actionrefresh_triggered()
{

}


void MainWindow::on_actionabout_triggered()
{

}


void MainWindow::on_actionhelp_triggered()
{

}

