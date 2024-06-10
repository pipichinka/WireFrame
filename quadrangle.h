#ifndef QUADRANGLE_H
#define QUADRANGLE_H

#include <QPainter>
#include "applicationconfig.h"
#include "camera.h"
#include "SceneObjectInterface.h"
class Quadrangle: public SceneObjectInterface
{
    Point3d p1;
    Point3d p2;
    Point3d p3;
    Point3d p4;
    ObjectOpticParams opticParams;
public:
    Quadrangle() = default;
    Quadrangle(const QuadrangleConfig& cnf);
    void virtual drowWireFrame(QPainter& p, const Camera& camera) const;
    virtual const ObjectOpticParams& getOpticParams() const { return opticParams;}
    virtual ~Quadrangle() = default;

    virtual void normalize(double size, const Point3d& center);
    virtual Point3d getMaxPoint() const;
    virtual Point3d getMinPoint() const;
    virtual bool isIntersect(const Point3d& p0, const Point3d& dp, Point3d& normal, Point3d& intersectPoint) const;
};

#endif // QUADRANGLE_H
