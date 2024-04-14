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


    static const int STEP = 10;


    void rePaint();
public:
    BSplineModel(int height, int width);
    void setView(ViewInterface* view){this->view = view;}
    QImage* getImage(){return image;}
    QPointF* getPointByCoords(int x, int y); // returns NULL if there is no point close
    void addPoint(int x, int y);
    void setN(int n){this->N = n; rePaint();}
    void zoomIn();
    void zoomOut();
    void normalize();
    void stepLeft();
    void stepRight();
    void stepUp();
    void stepDown();
    void setSplineColor(QRgb color){splineColor = color; rePaint();}
    void setPointsColor(QRgb color){pointsColor = color; rePaint();}
};

#endif // BSPLINEMODEL_H
