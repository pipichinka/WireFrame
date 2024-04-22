#include "bsplinewindow.h"
#include "ui_bsplinewindow.h"
#include "bsplinefield.h"
#include <QKeyEvent>
#include <iostream>
#include <QCloseEvent>
#include <QColorDialog>
#include <QErrorMessage>
#include <QMessageBox>
BSplineWindow::BSplineWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BSplineWindow)
{
    model = new BSplineModel(640,480);
    ui->setupUi(this);
    graphicsView = new BSplineField(ui->centralwidget, model, ui->lineEditX, ui->lineEditY);
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
    delete ui;
    delete graphicsView;
    delete model;
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
        case Qt::Key_Backspace:{
            model->removePickedPoint();
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


void BSplineWindow::on_okButton_clicked()
{
    this->close();
}


void BSplineWindow::on_colorSplineButton_clicked()
{
    QColor color = QColorDialog::getColor(model->getSplineColor(), this, "select color for points");
    if (color.isValid()){
        model->setSplineColor(color.rgba());
    }
}


void BSplineWindow::on_colorPointsButton_clicked()
{
    QColor color = QColorDialog::getColor(model->getPointsColor(), this, "select color for points");
    if (color.isValid()){
        model->setPointsColor(color.rgba());
    }
}

void BSplineWindow::on_changeXYButton_clicked()
{
    bool ok;
    double x = ui->lineEditX->text().toDouble(&ok);
    if (!ok){
        graphicsView->updateLines();
        QMessageBox box(QMessageBox::Icon::Critical, "error", "invalid X value", QMessageBox::Ok, this);
        box.exec();
        return;
    }
    double y = ui->lineEditY->text().toDouble(&ok);
    if (!ok){
        graphicsView->updateLines();
        QMessageBox box(QMessageBox::Icon::Critical, "error", "invalid Y value", QMessageBox::Ok, this);
        box.exec();
        std::cout << "error y" << std::endl;
        return;
    }
    auto result = model->movePickedPointDirect(x, y);
    if (result == nullptr){
        graphicsView->updateLines();
        QMessageBox box(QMessageBox::Icon::Critical, "error", "no point picked to change", QMessageBox::Ok, this);
        box.exec();
        std::cout << "error point" << std::endl;
    }

}

