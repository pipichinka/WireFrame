#ifndef SPHERE_H
#define SPHERE_H

#include "applicationconfig.h"
#include "camera.h"
#include <QPainter>
#include "SceneObjectInterface.h"
#define NUM_LINES 10

class Sphere: public SceneObjectInterface
{
    double r;
    Point3d center;
    ObjectOpticParams opticParams;
public:
    Sphere() = default;
    Sphere(const SphereConfig& cfg);
    void virtual drowWireFrame(QPainter& p, const Camera& camera) const;
    virtual const ObjectOpticParams& getOpticParams() const {return opticParams;}
    virtual ~Sphere() = default;

    virtual void normalize(double size, const Point3d& center);
    virtual Point3d getMaxPoint() const;
    virtual Point3d getMinPoint() const;
    virtual bool isIntersect(const Point3d& p0, const Point3d& dp, Point3d& normal, Point3d& intersectPoint) const;
};
#endif // SPHERE_H
