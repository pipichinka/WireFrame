#include "box.h"
#include <QPointF>
Box::Box(const BoxConfig& cfg):opticParams(cfg.opticParams)
{
    min(0,0) = cfg.min(0,0);
    min(1,0) = cfg.min(1,0);
    min(2,0) = cfg.min(2,0);
    min(3,0) = 1;

    max(0,0) = cfg.max(0,0);
    max(1,0) = cfg.max(1,0);
    max(2,0) = cfg.max(2,0);
    max(3,0) = 1;
}


void Box::drowWireFrame(QPainter& p, const Camera& camera) const{
    Point3d p1 = min;
    p1 = camera.getCameraMatrix() * p1;
    p1 /= p1(3,0);
    p1 = camera.getProectionMatrix() * p1;
    p1 /= p1(3,0);
    p1(0,0) = p1(0,0) * camera.getWidth() / 2 + camera.getWidth() / 2;
    p1(1,0) = -p1(1,0) * camera.getHeight() / 2 + camera.getHeight() / 2;
    QPointF p1OnScreen(p1(0,0), p1(1,0));

    Point3d p2 = min;
    p2(0,0) = max(0,0);
    p2 = camera.getCameraMatrix() * p2;
    p2 /= p2(3,0);
    p2 = camera.getProectionMatrix() * p2;
    p2 /= p2(3,0);
    p2(0,0) = p2(0,0) * camera.getWidth() / 2 + camera.getWidth() / 2;
    p2(1,0) = -p2(1,0) * camera.getHeight() / 2 + camera.getHeight() / 2;
    QPointF p2OnScreen(p2(0,0), p2(1,0));

    Point3d p3 = min;
    p3(1,0) = max(1,0);
    p3 = camera.getCameraMatrix() * p3;
    p3 /= p3(3,0);
    p3 = camera.getProectionMatrix() * p3;
    p3 /= p3(3,0);
    p3(0,0) = p3(0,0) * camera.getWidth() / 2 + camera.getWidth() / 2;
    p3(1,0) = -p3(1,0) * camera.getHeight() / 2 + camera.getHeight() / 2;
    QPointF p3OnScreen(p3(0,0), p3(1,0));


    Point3d p4 = min;
    p4(2,0) = max(2,0);
    p4 = camera.getCameraMatrix() * p4;
    p4 /= p4(3,0);
    p4 = camera.getProectionMatrix() * p4;
    p4 /= p4(3,0);
    p4(0,0) = p4(0,0) * camera.getWidth() / 2 + camera.getWidth() / 2;
    p4(1,0) = -p4(1,0) * camera.getHeight() / 2 + camera.getHeight() / 2;
    QPointF p4OnScreen(p4(0,0), p4(1,0));

    Point3d p5 = min;
    p5(2,0) = max(2,0);
    p5(1,0) = max(1,0);
    p5 = camera.getCameraMatrix() * p5;
    p5 /= p5(3,0);
    p5 = camera.getProectionMatrix() * p5;
    p5 /= p5(3,0);
    p5(0,0) = p5(0,0) * camera.getWidth() / 2 + camera.getWidth() / 2;
    p5(1,0) = -p5(1,0) * camera.getHeight() / 2 + camera.getHeight() / 2;
    QPointF p5OnScreen(p5(0,0), p5(1,0));

    Point3d p6 = min;
    p6(2,0) = max(2,0);
    p6(0,0) = max(0,0);
    p6 = camera.getCameraMatrix() * p6;
    p6 /= p6(3,0);
    p6 = camera.getProectionMatrix() * p6;
    p6 /= p6(3,0);
    p6(0,0) = p6(0,0) * camera.getWidth() / 2 + camera.getWidth() / 2;
    p6(1,0) = -p6(1,0) * camera.getHeight() / 2 + camera.getHeight() / 2;
    QPointF p6OnScreen(p6(0,0), p6(1,0));

    Point3d p7 = min;
    p7(0,0) = max(0,0);
    p7(1,0) = max(1,0);
    p7 = camera.getCameraMatrix() * p7;
    p7 /= p7(3,0);
    p7 = camera.getProectionMatrix() * p7;
    p7 /= p7(3,0);
    p7(0,0) = p7(0,0) * camera.getWidth() / 2 + camera.getWidth() / 2;
    p7(1,0) = -p7(1,0) * camera.getHeight() / 2 + camera.getHeight() / 2;
    QPointF p7OnScreen(p7(0,0), p7(1,0));

    Point3d p8 = max;
    p8 = camera.getCameraMatrix() * p8;
    p8 /= p8(3,0);
    p8 = camera.getProectionMatrix() * p8;
    p8 /= p8(3,0);
    p8(0,0) = p8(0,0) * camera.getWidth() / 2 + camera.getWidth() / 2;
    p8(1,0) = -p8(1,0) * camera.getHeight() / 2 + camera.getHeight() / 2;
    QPointF p8OnScreen(p8(0,0), p8(1,0));

    p.drawLine(p1OnScreen, p2OnScreen);
    p.drawLine(p1OnScreen, p3OnScreen);
    p.drawLine(p1OnScreen, p4OnScreen);

    p.drawLine(p2OnScreen, p7OnScreen);
    p.drawLine(p2OnScreen, p6OnScreen);

    p.drawLine(p4OnScreen, p5OnScreen);
    p.drawLine(p4OnScreen, p6OnScreen);

    p.drawLine(p3OnScreen, p7OnScreen);
    p.drawLine(p3OnScreen, p5OnScreen);

    p.drawLine(p7OnScreen, p8OnScreen);
    p.drawLine(p6OnScreen, p8OnScreen);
    p.drawLine(p5OnScreen, p8OnScreen);

}


void Box::normalize(double size, const Point3d& center){
    max -= center;
    max /= size;
    max(3, 0) = 1;
    min -= center;
    min /= size;
    min(3, 0) = 1;
}


Point3d Box::getMaxPoint() const{
    return max;
}


Point3d Box::getMinPoint() const{
    return min;
}

bool Box::isIntersect(const Point3d& p0, const Point3d& dp, Point3d& normal, Point3d& intersectPoint) const{
    double tNear = -INFINITY;
    double tFar = INFINITY;
    int whitchIsnear = -1; // 0 X 1 Y 2 Z
    double xMin = min(0,0);
    double yMin = min(1,0);
    double zMin = min(2,0);
    double xMax = max(0,0);
    double yMax = max(1,0);
    double zMax = max(3,0);

    if (dp(0,0) == 0.0){
        if (p0(0,0) < xMin || p0(0,0) > xMax){
            return false;
        }
    }
    else {
        double t1 = (xMin - p0(0,0)) / dp(0,0);
        double t2 = (xMax - p0(0,0)) / dp(0,0);
        if (t2 < t1){
            double t = t2;
            t2 = t1;
            t1 = t;
        }
        if (t1 > tNear){
            tNear = t1;
            whitchIsnear = 0;
        }
        if (t2 < tFar){
            tFar = t2;
        }
        if (tNear > tFar){
            return false;
        }
        if (tFar < 0){
            return false;
        }
    }

    if (dp(1,0) == 0.0){
        if (p0(1,0) < yMin || p0(1,0) > yMax){
            return false;
        }
    }
    else {
        double t1 = (yMin - p0(1,0)) / dp(1,0);
        double t2 = (yMax - p0(1,0)) / dp(1,0);
        if (t2 < t1){
            double t = t2;
            t2 = t1;
            t1 = t;
        }
        if (t1 > tNear){
            tNear = t1;
            whitchIsnear = 1;
        }
        if (t2 < tFar){
            tFar = t2;
        }
        if (tNear > tFar){
            return false;
        }
        if (tFar < 0){
            return false;
        }

    }

    if (dp(2,0) == 0.0){
        if (p0(2,0) < zMin || p0(2,0) > zMax){
            return false;
        }
    }
    else {
        double t1 = (zMin - p0(2,0)) / dp(2,0);
        double t2 = (zMax - p0(2,0)) / dp(2,0);
        if (t2 < t1){
            double t = t2;
            t2 = t1;
            t1 = t;
        }
        if (t1 > tNear){
            tNear = t1;
            whitchIsnear = 2;
        }
        if (t2 < tFar){
            tFar = t2;
        }
        if (tNear > tFar){
            return false;
        }
        if (tFar < 0){
            return false;
        }
    }

    if (whitchIsnear == 0){
        if (dp(0,0) > 0){
            normal(0,0) = -1;
        } else {
            normal(0,0) = 1;
        }
        normal(1,0) = 0;
        normal(2, 0) = 0;
        normal(3, 0) = 0;
    } else if (whitchIsnear == 1){
        if (dp(1,0) > 0){
            normal(1,0) = -1;
        } else {
            normal(1,0) = 1;
        }
        normal(0,0) = 0;
        normal(2, 0) = 0;
        normal(3, 0) = 0;
    } else if (whitchIsnear == 2){
        if (dp(2,0) > 0){
            normal(2,0) = -1;
        } else {
            normal(2,0) = 1;
        }
        normal(1,0) = 0;
        normal(0, 0) = 0;
        normal(3, 0) = 0;
    } else {
        return false;
    }
    intersectPoint = p0 + dp * tNear;
    return true;
}












