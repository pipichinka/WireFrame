#ifndef LIGHTSOURCE_H
#define LIGHTSOURCE_H

#include "applicationconfig.h"
class LightSource
{
    double r;
    double g;
    double b;
    Point3d original;
    Point3d current;
public:
    LightSource() = default;

    LightSource(const LightSourceConfig& conf);
    void clean(){current = original;}
    void applyMatrix(QGenericMatrix<4,4,double> matrix){
        current = matrix * current;
    }
    const Point3d& getPoint() const { return current;}
    double getR() const {return r;}
    double getG() const {return g;}
    double getB() const {return b;}
};

#endif // LIGHTSOURCE_H
