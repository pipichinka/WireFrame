#ifndef BSPLINEFIELD_H
#define BSPLINEFIELD_H

#include <QObject>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QLineEdit>
#include "viewinterface.h"
#include "bsplinemodel.h"
class BSplineField: public QGraphicsView, public ViewInterface
{
    Q_OBJECT
public:
    BSplineField(QWidget* parent, BSplineModel* model, QLineEdit* lineX, QLineEdit* lineY);
    virtual void onModelChange() override;
    void updateLines();
protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void resizeEvent(QResizeEvent* event) override;
private:
    BSplineModel* model;
    QGraphicsScene* scene;
    QGraphicsItem* item_on_scene;
    int current_width;
    int current_height;
    bool isMousePressed; // is used for moving points on screen
    QLineEdit* lineX;
    QLineEdit* lineY;
};

#endif // BSPLINEFIELD_H
