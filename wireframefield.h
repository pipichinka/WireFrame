#ifndef WIREFRAMEFIELD_H
#define WIREFRAMEFIELD_H

#include <QGraphicsView>
#include "viewinterface.h"
#include "wireframermodel.h"
class WireFrameField : public QGraphicsView, ViewInterface
{
    Q_OBJECT
public:
    WireFrameField(QWidget* parent, WireFrameRModel* model);
    virtual void onModelChange() override;
protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void resizeEvent(QResizeEvent* event) override;
    virtual void wheelEvent(QWheelEvent* event) override;
private:
    WireFrameRModel* model;
    QGraphicsScene* scene;
    QGraphicsItem* item_on_scene;
    int current_width;
    int current_height;
    bool isMousePressed; // is used for moving points on screen
    int prevMouseX;
    int prevMouseY;
};

#endif // WIREFRAMEFIELD_H
