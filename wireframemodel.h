#ifndef WIREFRAMEMODEL_H
#define WIREFRAMEMODEL_H

#include "bsplinemodel.h"
#include <QGenericMatrix>




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

    std::vector<std::vector<Point3d>> bspline3DPoints;
    std::vector<std::vector<Point3d>> circles;


    void rePaint(bool isFull);
    QPointF calcOnePointOnScreen(QGenericMatrix<1, 4, double>& point, QPainter& painter);
    void drawXYZ(QPainter& painter);
public:
    WireFrameModel(int height, int width);
    void setView(ViewInterface* view){this->view = view;}
    QImage* getImage(){return image;}
    virtual QPointF* movePickedPoint(int x, int y) override;
    virtual QPointF* movePickedPointDirect(double x, double y) override;
    virtual QPointF* addPoint(int x, int y) override;
    virtual QPointF* removePickedPoint() override;
    virtual void setN(int n) override;
    virtual int getM()override{ return M;}
    virtual int getM1()override{ return M1;}
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
