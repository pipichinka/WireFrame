#ifndef BSPLINEFIELD_H
#define BSPLINEFIELD_H

#include <QObject>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QLineEdit>
#include "viewinterface.h"
#include "bsplinemodel.h"
#include <pthread.h>
class BSplineField: public QGraphicsView
{
    Q_OBJECT
public:
    BSplineField(QWidget* parent, QImage* image);
    void updateImage();
    ~BSplineField();
private:
    QImage* image;
    QGraphicsScene* scene;
    QGraphicsItem* item;
};

#endif // BSPLINEFIELD_H
