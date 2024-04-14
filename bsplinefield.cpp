#include "bsplinefield.h"
#include <iostream>
#include <QMouseEvent>
#include <QScrollBar>
BSplineField::BSplineField(QWidget* parent, BSplineModel* model): QGraphicsView(parent), model(model), scene(new QGraphicsScene), item_on_scene(nullptr),
    current_width(model->getImage()->width()), current_height(model->getImage()->height())
{
    model->setView(this);
    this->setScene(scene);
    scene->setSceneRect(0, 0, current_width, current_height);
    this->setMaximumSize(current_width + 20, current_height + 20);
    QPixmap pic = QPixmap::fromImage(*model->getImage());
    item_on_scene = reinterpret_cast<QGraphicsItem*>(scene->addPixmap(pic));
}


void BSplineField::mousePressEvent(QMouseEvent *event){
    std::cout << event->pos().x() << "  " << event->pos().y() << std::endl;
    int pos_x = event->x() + this->horizontalScrollBar()->value();
    int pos_y = event->y() + this->verticalScrollBar()->value();
    this->horizontalScrollBar()->value();
    model->addPoint(pos_x, pos_y);
    QGraphicsView::mousePressEvent(event);
}


void BSplineField::mouseReleaseEvent(QMouseEvent *event){
    QGraphicsView::mouseReleaseEvent(event);
}


void BSplineField::mouseMoveEvent(QMouseEvent *event){
    QGraphicsView::mouseMoveEvent(event);
}


void BSplineField::onModelChange(){
    int width = model->getImage()->width();
    int height = model->getImage()->height();
    if (current_height != height || current_width != width){
        current_width = width;
        current_height = height;
        scene->setSceneRect(0,0, current_width, current_height);
        this->setMaximumSize(current_width + 2, current_height + 2);
    }
    scene->removeItem(item_on_scene);
    QPixmap pic = QPixmap::fromImage(*model->getImage());
    item_on_scene = reinterpret_cast<QGraphicsItem*>(scene->addPixmap(pic));
}

