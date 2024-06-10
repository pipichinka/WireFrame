#ifndef BOX_H
#define BOX_H

#include <QPainter>
#include "applicationconfig.h"
#include "camera.h"
#include "SceneObjectInterface.h"
class Box:  public SceneObjectInterface
{
    Point3d min;
    Point3d max;
    ObjectOpticParams opticParams;
public:
    Box() = default;
    Box(const BoxConfig& cfg);
    virtual void drowWireFrame(QPainter& p, const Camera& camera) const;
    virtual const ObjectOpticParams& getOpticParams() const { return opticParams;}
    virtual ~Box() = default;

    virtual void normalize(double size, const Point3d& center);
    virtual Point3d getMaxPoint() const;
    virtual Point3d getMinPoint() const;
    virtual bool isIntersect(const Point3d& p0, const Point3d& dp, Point3d& normal, Point3d& intersectPoint) const;
};

#endif // BOX_H
