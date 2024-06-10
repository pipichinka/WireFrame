#include "triangle.h"
#include <QPointF>
#include <QtDebug>
Triangle::Triangle(const TriangleConfig& cnf):opticParams(cnf.opticParams)
{
    p1(0,0) = cnf.p1(0,0);
    p1(1,0) = cnf.p1(1,0);
    p1(2,0) = cnf.p1(2,0);
    p1(3,0) = 1.0;

    p2(0,0) = cnf.p2(0,0);
    p2(1,0) = cnf.p2(1,0);
    p2(2,0) = cnf.p2(2,0);
    p2(3,0) = 1.0;

    p3(0,0) = cnf.p3(0,0);
    p3(1,0) = cnf.p3(1,0);
    p3(2,0) = cnf.p3(2,0);
    p3(3,0) = 1.0;
}


void Triangle::normalize(double size, const Point3d& center){
    p1 -= center;
    p1 /= size;
    p1(3,0) = 1.0;

    p2 -= center;
    p2 /= size;
    p2(3,0) = 1.0;

    p3 -= center;
    p3 /= size;
    p3(3,0) = 1.0;
    //qDebug() << "after normalize " << p1 << p2 << p3;
}


Point3d Triangle::getMaxPoint() const{
    double maxX = std::max(p1(0,0), std::max(p2(0,0), p3(0,0)));
    double maxY = std::max(p1(1,0), std::max(p2(1,0), p3(1,0)));
    double maxZ = std::max(p1(2,0), std::max(p2(2,0), p3(2,0)));
    double data[] = {maxX, maxY, maxZ, 1.0};
    return Point3d(data);
}


Point3d Triangle::getMinPoint() const{
    double minX = std::min(p1(0,0), std::min(p2(0,0), p3(0,0)));
    double minY = std::min(p1(1,0), std::min(p2(1,0), p3(1,0)));
    double minZ = std::min(p1(2,0), std::min(p2(2,0), p3(2,0)));
    double data[] = {minX, minY, minZ, 1.0};
    return Point3d(data);
}


bool Triangle::isIntersect(const Point3d& p0, const Point3d& dp, Point3d& normal, Point3d& intersectPoint) const{
    Point3d vec1 = p2 - p1;
    Point3d vec2 = p3 - p1;
    normal = vectorMult(vec1, vec2);
    normal /= std::sqrt(norm(normal));
    double D = -prod(normal, p1);
    bool res = isIntersectPlane(p0, dp, normal, intersectPoint, D);
    if (!res){
        return false;
    }
    QPointF diff;
    QPointF pf1;
    QPointF pf2;
    QPointF pf3;
    double max = std::max(std::max(std::abs(normal(0,0)), std::abs(normal(1,0))), std::abs(normal(2,0)));
    if (max == std::abs(normal(0,0))){
        diff = QPointF(intersectPoint(1,0), intersectPoint(2,0));
        pf1 = QPointF(p1(1,0), p1(2,0));
        pf2 = QPointF(p2(1,0), p2(2,0));
        pf3 = QPointF(p3(1,0), p3(2,0));
    } else if (max == std::abs(normal(1,0))){
        diff = QPointF(intersectPoint(0,0), intersectPoint(2,0));
        pf1 = QPointF(p1(0,0), p1(2,0));
        pf2 = QPointF(p2(0,0), p2(2,0));
        pf3 = QPointF(p3(0,0), p3(2,0));
    } else {
        diff = QPointF(intersectPoint(1,0), intersectPoint(0,0));
        pf1 = QPointF(p1(1,0), p1(0,0));
        pf2 = QPointF(p2(1,0), p2(0,0));
        pf3 = QPointF(p3(1,0), p3(0,0));
    }
    pf1 -= diff;
    pf2 -= diff;
    pf3 -= diff;
    int numCrossing = 0;
    int signHolder;
    if (pf1.y() < 0.0){
        signHolder = -1;
    } else {
        signHolder = 1;
    }
    int nextSignHolder;
    if (pf2.y() < 0.0){
        nextSignHolder = -1;
    } else {
        nextSignHolder = 1;
    }
    if (signHolder != nextSignHolder){
        if (pf1.x() > 0 && pf2.x() > 0){
            numCrossing++;
        } else {
            if (pf1.x() > 0 || pf2.x() > 0){
                double val = pf1.x() - (pf1.y()*(pf2.x() - pf1.x())) / (pf2.y() - pf1.y());
                if (val > 0){
                    numCrossing++;
                }
            }
        }
    }
    signHolder = nextSignHolder;

    if (pf3.y() < 0.0){
        nextSignHolder = -1;
    } else {
        nextSignHolder = 1;
    }
    if (signHolder != nextSignHolder){
        if (pf2.x() > 0 && pf3.x() > 0){
            numCrossing++;
        } else {
            if (pf2.x() > 0 || pf3.x() > 0){
                double val = pf2.x() - (pf2.y()*(pf3.x() - pf2.x())) / (pf3.y() - pf2.y());
                if (val > 0){
                    numCrossing++;
                }
            }
        }
    }
    signHolder = nextSignHolder;


    if (pf1.y() < 0.0){
        nextSignHolder = -1;
    } else {
        nextSignHolder = 1;
    }
    if (signHolder != nextSignHolder){
        if (pf3.x() > 0 && pf1.x() > 0){
            numCrossing++;
        } else {
            if (pf3.x() > 0 || pf1.x() > 0){
                double val = pf3.x() - (pf3.y()*(pf1.x() - pf3.x())) / (pf1.y() - pf3.y());
                if (val > 0){
                    numCrossing++;
                }
            }
        }
    }
    if (numCrossing % 2 == 1){
        return true;
    }
    return false;
}


void Triangle::drowWireFrame(QPainter& p, const Camera& camera) const {
    Point3d p1After = camera.getCameraMatrix() * p1;
    p1After /= p1After(3,0);
    p1After = camera.getProectionMatrix() * p1After;
    p1After /= p1After(3,0);
    p1After(0,0) = p1After(0,0) * camera.getWidth() / 2 + camera.getWidth() / 2;
    p1After(1,0) = -p1After(1,0) * camera.getHeight() / 2 + camera.getHeight() / 2;

    QPointF p1OnScreen(p1After(0,0), p1After(1,0));

    Point3d p2After = camera.getCameraMatrix() * p2;
    p2After /= p2After(3,0);
    p2After = camera.getProectionMatrix() * p2After;
    p2After /= p2After(3,0);
    p2After(0,0) = p2After(0,0) * camera.getWidth() / 2 + camera.getWidth() / 2;
    p2After(1,0) = -p2After(1,0) * camera.getHeight() / 2 + camera.getHeight() / 2;

    QPointF p2OnScreen(p2After(0,0), p2After(1,0));

    Point3d p3After = camera.getCameraMatrix() * p3;
    p3After /= p3After(3,0);
    p3After = camera.getProectionMatrix() * p3After;
    p3After /= p3After(3,0);
    p3After(0,0) = p3After(0,0) * camera.getWidth() / 2 + camera.getWidth() / 2;
    p3After(1,0) = -p3After(1,0) * camera.getHeight() / 2 + camera.getHeight() / 2;

    QPointF p3OnScreen(p3After(0,0), p3After(1,0));
//    qDebug() << p1OnScreen;
//    qDebug() << p2OnScreen;
//    qDebug() << p3OnScreen;
    p.drawLine(p1OnScreen, p2OnScreen);
    p.drawLine(p1OnScreen, p3OnScreen);
    p.drawLine(p2OnScreen, p3OnScreen);
}
