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
    virtual QPointF* movePickedPoint(int x, int y) override;
    virtual QPointF* movePickedPointDirect(double x, double y) override;
    virtual QPointF* addPoint(int x, int y) override;
    virtual QPointF* removePickedPoint() override;
    virtual void setN(int n) override;
    virtual void setM(int m) override;
    virtual void setM1(int m1) override;
    void zoom(int value);
    void moveEngleByMouse(int x, int y);
    void clearAngle();
    virtual void setSplineColor(QRgb color) override;
    void setSize(int width, int height);
    virtual void readConfig(const ApplicationConfig& config) override;
    virtual void recordDataToConfig(ApplicationConfig& config) override;
};

#endif // WIREFRAMEMODEL_H
