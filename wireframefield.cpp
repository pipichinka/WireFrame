#include "wireframefield.h"

#include <iostream>
#include <QMouseEvent>
#include <QScrollBar>
#include <QGraphicsItem>
WireFrameField::WireFrameField(QWidget* parent, WireFrameRModel* model):
    QGraphicsView(parent), model(model), scene(new QGraphicsScene(this)), item_on_scene(nullptr),
        current_width(model->getImage()->width()), current_height(model->getImage()->height()), isMousePressed(false)
{
    model->setView(this);
    this->setScene(scene);
    scene->setSceneRect(0, 0, current_width, current_height);
    QPixmap pic = QPixmap::fromImage(*model->getImage());
    item_on_scene = reinterpret_cast<QGraphicsItem*>(scene->addPixmap(pic));
}



void WireFrameField::mousePressEvent(QMouseEvent *event){
    int posX = event->x() + this->horizontalScrollBar()->value();
    int posY = event->y() + this->verticalScrollBar()->value();
    if (event->button() == Qt::MouseButton::LeftButton){
        isMousePressed = true;
        prevMouseX = posX;
        prevMouseY = posY;
    }
    QGraphicsView::mousePressEvent(event);
}


void WireFrameField::mouseReleaseEvent(QMouseEvent *event){
    if (event->button() == Qt::MouseButton::LeftButton){
        isMousePressed = false;
    }
    QGraphicsView::mouseReleaseEvent(event);
}


void WireFrameField::mouseMoveEvent(QMouseEvent *event){

    if (isMousePressed){
        int posX = event->x() + this->horizontalScrollBar()->value();
        int posY = event->y() + this->verticalScrollBar()->value();
        model->moveEngleByMouse(-posX + prevMouseX, -posY + prevMouseY);
        prevMouseX = posX;
        prevMouseY = posY;
    }
    QGraphicsView::mouseMoveEvent(event);
}


void WireFrameField::onModelChange(){
    int width = model->getImage()->width();
    int height = model->getImage()->height();
    if (current_height != height || current_width != width){
        current_width = width;
        current_height = height;
        scene->setSceneRect(0,0, current_width + 2, current_height + 2);
    }
    scene->removeItem(item_on_scene);
    delete item_on_scene;
    QPixmap pic = QPixmap::fromImage(*model->getImage());
    item_on_scene = reinterpret_cast<QGraphicsItem*>(scene->addPixmap(pic));
}


void WireFrameField::resizeEvent(QResizeEvent *event){
    model->setSize(event->size().height() - 2, event->size().width() - 2);
}


void WireFrameField::wheelEvent(QWheelEvent* event){
    QPoint delta = event->angleDelta() / 8;
    model->zoom(delta.y());
}

