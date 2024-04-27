#ifndef BSPLINEMODEL_H
#define BSPLINEMODEL_H

#include <QImage>
#include <vector>
#include <QPoint>
#include <QPointF>
#include <QColor>
#include "viewinterface.h"
class BSplineModel
{
    ViewInterface* view;
    QImage* image;
    std::vector<QPointF*> points;
    int N;
    QPointF centerPoint;
    double zoomScale; //default 1.0 max 5.0 min 0.1 step 0.1
    QRgb splineColor;
    QRgb pointsColor;
    int height;
    int width;
    int pickedPointIndex;

    static const int STEP = 10;


    void rePaint();
protected:
    std::vector<QPointF> getBSplinePoints();
public:
    BSplineModel(int height, int width);
    void setView(ViewInterface* view){this->view = view;}
    QImage* getImage(){return image;}
    QPointF* getPickedPoint();
    QPointF* peekPoint(int x, int y); // returns NULL if there is no point close
    virtual QPointF* movePickedPoint(int x, int y);
    virtual QPointF* movePickedPointDirect(double x, double y);
    virtual QPointF* addPoint(int x, int y);
    virtual QPointF* removePickedPoint();
    virtual void setN(int n){this->N = n; rePaint();}
    int getN(){return N;}
    virtual void setM(int m) = 0;
    virtual void setM1(int m1) = 0;
    void zoomIn();
    void zoomOut();
    void normalize();
    void stepLeft();
    void stepRight();
    void stepUp();
    void stepDown();
    virtual void setSplineColor(QRgb color){splineColor = color; rePaint();}
    QRgb getSplineColor(){return splineColor;}
    void setPointsColor(QRgb color){pointsColor = color; rePaint();}
    QRgb getPointsColor(){return pointsColor;}
    void setSize(int width, int height);
};

#endif // BSPLINEMODEL_H
