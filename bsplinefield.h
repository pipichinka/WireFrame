#ifndef BSPLINEFIELD_H
#define BSPLINEFIELD_H

#include <QObject>
#include <QGraphicsView>
#include <QGraphicsScene>

#include "viewinterface.h"
#include "bsplinemodel.h"
class BSplineField: public QGraphicsView, public ViewInterface
{
    Q_OBJECT
public:
    BSplineField(QWidget* parent, BSplineModel* model);
    virtual void onModelChange() override;
protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
private:
    BSplineModel* model;
    QGraphicsScene* scene;
    QGraphicsItem* item_on_scene;
    int current_width;
    int current_height;
};

#endif // BSPLINEFIELD_H
