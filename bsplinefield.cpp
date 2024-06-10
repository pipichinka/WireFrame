#include "bsplinefield.h"
#include <iostream>
#include <QMouseEvent>
#include <QScrollBar>
#include <QGraphicsItem>
BSplineField::BSplineField(QWidget* parent, QImage* image): QGraphicsView(parent), image(image), scene(new QGraphicsScene(this)), item (nullptr)
{
    setScene(scene);
    scene->setSceneRect(0, 0, image->width(), image->height());
    //this->setMaximumSize(current_width + 10, current_height + 15);
}


BSplineField::~BSplineField(){
    delete scene;
}


void BSplineField::updateImage(){
    QPixmap pic = QPixmap::fromImage(*image);
    if (item){
        delete item;
    }
    item = reinterpret_cast<QGraphicsItem*>(scene->addPixmap(pic));
    delete image;
}
