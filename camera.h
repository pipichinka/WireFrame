#ifndef CAMERA_H
#define CAMERA_H

#include "applicationconfig.h"
class Camera
{
    QGenericMatrix<4,4, double> cameraMatrix;
    QGenericMatrix<4,4, double> proectionMatrix;
    Point3d pos;
    Point3d up;
    Point3d forward;
    Point3d right;
    double zf;
    double zb;
    double sw;
    double sh;
    double height;
    double width;
    QGenericMatrix<4,4, double> makeRotMatrix();
    QGenericMatrix<4,4, double> makeMoveMatrix();
    QGenericMatrix<4,4, double> makeProectionMatrix();
public:
    Camera(const RenderConfig& conf, int height, int width);
    void moveRight(double t);
    void moveLeft(double t);
    void moveForward(double t);
    void moveBack(double t);
    void zoom(double m);
    void setHeight(int height){this->height = (double) height;}
    void setWidth(int width){this->width = (double) width;}
    void rotate(double phi, bool isX);
    const QGenericMatrix<4,4, double>& getCameraMatrix() const{return cameraMatrix;}
    const QGenericMatrix<4,4, double>& getProectionMatrix() const{return proectionMatrix;}
    double getHeight() const {return height;}
    double getWidth() const {return width;}
    double getSW()const{return sw;}
    double getSH()const{return sh;}
    const Point3d& getRight() const {return right;}
    const Point3d& getUp() const {return up;}
    Point3d getFrameCenter() const {return pos + zf * forward;}
    const Point3d& getPos() const {return pos;}
    double getZf() const {return zf;}
};

#endif // CAMERA_H
