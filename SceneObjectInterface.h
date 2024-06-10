#ifndef SCENEOBJECTINTERFACE_H
#define SCENEOBJECTINTERFACE_H

#include "camera.h"
#include <QPainter>
class SceneObjectInterface{
public:
    virtual void drowWireFrame(QPainter& p, const Camera& camera) const = 0;
    virtual const ObjectOpticParams& getOpticParams() const = 0;
    virtual ~SceneObjectInterface() = default;
    virtual void normalize(double size, const Point3d& center) = 0;
    virtual Point3d getMaxPoint() const = 0;
    virtual Point3d getMinPoint() const = 0;
    virtual bool isIntersect(const Point3d& p0, const Point3d& dp, Point3d& normal, Point3d& intersectPoint) const = 0;
};

#endif // SCENEOBJECTINTERFACE_H
