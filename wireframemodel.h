#ifndef WIREFRAMEMODEL_H
#define WIREFRAMEMODEL_H

#include "bsplinemodel.h"
#include <QGenericMatrix>


typedef QGenericMatrix<1, 4, double> Point3d;


class WireFrameModel: public BSplineModel
{

    ViewInterface* view;
    QImage* image;
    int height;
    int width;
    int M;
    int M1;
    QGenericMatrix<4,4, double> rotaitionMatrix;
    QGenericMatrix<4,4, double> cameraMatrix;


    void rePaint(bool isFull);
    QPointF calcOnePointOnScreen(QGenericMatrix<1, 4, double>& point, QPainter& painter);
public:
    WireFrameModel(int height, int width);
    void setView(ViewInterface* view){this->view = view;}
    QImage* getImage(){return image;}
    virtual QPointF* movePickedPoint(int x, int y);
    virtual QPointF* movePickedPointDirect(double x, double y);
    virtual QPointF* addPoint(int x, int y);
    virtual QPointF* removePickedPoint();
    virtual void setN(int n);
    virtual void setM(int m);
    virtual void setM1(int m1);
    void zoom(int value);
    void moveEngleByMouse(int x, int y);
    void clearAngle();
    virtual void setSplineColor(QRgb color);
    void setSize(int width, int height);
};

#endif // WIREFRAMEMODEL_H
