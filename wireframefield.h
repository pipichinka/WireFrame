#ifndef WIREFRAMEFIELD_H
#define WIREFRAMEFIELD_H

#include <QGraphicsView>
#include "viewinterface.h"
#include "wireframemodel.h"
class WireFrameField : public QGraphicsView, ViewInterface
{
    Q_OBJECT
public:
    WireFrameField(QWidget* parent, WireFrameModel* model);
    virtual void onModelChange() override;
protected:
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void resizeEvent(QResizeEvent* event) override;
    virtual void wheelEvent(QWheelEvent* event) override;
private:
    WireFrameModel* model;
    QGraphicsScene* scene;
    QGraphicsItem* item_on_scene;
    int current_width;
    int current_height;
    bool isMousePressed; // is used for moving points on screen
    int prevMouseX;
    int prevMouseY;
};

#endif // WIREFRAMEFIELD_H
