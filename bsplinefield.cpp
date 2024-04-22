#include "bsplinefield.h"
#include <iostream>
#include <QMouseEvent>
#include <QScrollBar>
BSplineField::BSplineField(QWidget* parent, BSplineModel* model, QLineEdit* lineX, QLineEdit* lineY): QGraphicsView(parent), model(model), scene(new QGraphicsScene(this)), item_on_scene(nullptr),
    current_width(model->getImage()->width()), current_height(model->getImage()->height()), isMousePressed(false), lineX(lineX), lineY(lineY)
{
    model->setView(this);
    this->setScene(scene);
    scene->setSceneRect(0, 0, current_width, current_height);
    //this->setMaximumSize(current_width + 10, current_height + 15);
    QPixmap pic = QPixmap::fromImage(*model->getImage());
    item_on_scene = reinterpret_cast<QGraphicsItem*>(scene->addPixmap(pic));
}


void BSplineField::mousePressEvent(QMouseEvent *event){
    int posX = event->x() + this->horizontalScrollBar()->value();
    int posY = event->y() + this->verticalScrollBar()->value();
    QPointF* res = nullptr;
    if (event->button() == Qt::MouseButton::LeftButton){
        isMousePressed = true;
        res = model->peekPoint(posX, posY);
    }
    else if (event->button() == Qt::MouseButton::RightButton){
        res = model->addPoint(posX, posY);
    }
    if (res){
        lineX->setText(QString::number(res->x()));
        lineY->setText(QString::number(res->y()));
    } else{
        lineX->setText("");
        lineY->setText("");
    }
    QGraphicsView::mousePressEvent(event);
}


void BSplineField::mouseReleaseEvent(QMouseEvent *event){
    if (event->button() == Qt::MouseButton::LeftButton){
        isMousePressed = false;
    }
    QGraphicsView::mouseReleaseEvent(event);
}


void BSplineField::mouseMoveEvent(QMouseEvent *event){

    if (isMousePressed){
        int posX = event->x() + this->horizontalScrollBar()->value();
        int posY = event->y() + this->verticalScrollBar()->value();
        QPointF* res = model->movePickedPoint(posX, posY);
        if (res){
            lineX->setText(QString::number(res->x()));
            lineY->setText(QString::number(res->y()));
        }
    }
    QGraphicsView::mouseMoveEvent(event);
}


void BSplineField::onModelChange(){
    int width = model->getImage()->width();
    int height = model->getImage()->height();
    if (current_height != height || current_width != width){
        current_width = width;
        current_height = height;
        scene->setSceneRect(0,0, current_width + 2, current_height + 2);
    }
    scene->removeItem(item_on_scene);
    QPixmap pic = QPixmap::fromImage(*model->getImage());
    item_on_scene = reinterpret_cast<QGraphicsItem*>(scene->addPixmap(pic));
}


void BSplineField::resizeEvent(QResizeEvent *event){
    model->setSize(event->size().width() - 2, event->size().height() - 2);
}

void BSplineField::updateLines(){
    QPointF* res = model->getPickedPoint();
    if (res){
        lineX->setText(QString::number(res->x()));
        lineY->setText(QString::number(res->y()));
    } else{
        lineX->setText("");
        lineY->setText("");
    }
}

