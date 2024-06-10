#include "sphere.h"
#include <QPointF>
Sphere::Sphere(const SphereConfig& cfg):
    r(cfg.r), opticParams(cfg.opticParams)
{
    center(0, 0) = cfg.center(0, 0);
    center(1, 0) = cfg.center(1, 0);
    center(2, 0) = cfg.center(2, 0);
    center(3, 0) = 1;

}


void Sphere::normalize(double size, const Point3d& center){
    r /= size;
    this->center -= center;
    this->center /= size;
    this->center(3,0) = 1.0;
}


Point3d Sphere::getMaxPoint() const{
    double data[] = {center(0,0) + r, center(1,0) + r, center(2,0) + r, 1};
    return Point3d(data);

}


Point3d Sphere::getMinPoint() const{
    double data[] = {center(0,0) - r, center(1,0) - r, center(2,0) - r, 1};
    return Point3d(data);
}


bool Sphere::isIntersect(const Point3d& p0, const Point3d& dp, Point3d& normal, Point3d& intersectPoint) const{
    double B = prod(dp, p0 - center);
    double C = norm(p0 - center) - r * r;
    double D = B * B - C;
    if (D < 0.0){
        return false;
    }
    double t0 = -B - std::sqrt(D);
    if (t0 > 0.0){
        intersectPoint = dp * t0 + p0;
        normal = (intersectPoint - center) / r;
        return true;
    }

    double t1 = -B + std::sqrt(D);
    if (t1 < 0.0){
        return false;
    }
    intersectPoint = dp * t1 + p0;
    normal = (intersectPoint - center) / r;
    return true;
}


void Sphere::drowWireFrame(QPainter& p, const Camera& camera) const{
    Point3d dotOnSphere = center;
    dotOnSphere(0,0) += r;

    Point3d afterCameraCenter = camera.getCameraMatrix() * center;
    afterCameraCenter /= afterCameraCenter(3,0);
    Point3d afterProgectionCenter = camera.getProectionMatrix() * afterCameraCenter;
    afterProgectionCenter /= afterProgectionCenter(3,0);

    dotOnSphere = camera.getCameraMatrix() * dotOnSphere ;
    dotOnSphere  /= dotOnSphere (3,0);
    dotOnSphere  = camera.getProectionMatrix() * dotOnSphere ;
    dotOnSphere  /= dotOnSphere (3,0);
    dotOnSphere -= afterProgectionCenter;
    double afterProgectionR = std::sqrt(dotOnSphere(0,0) * dotOnSphere(0, 0) + dotOnSphere(1,0) * dotOnSphere(1, 0) + dotOnSphere(2,0) * dotOnSphere(2, 0));
    Point3d vertices[NUM_LINES][NUM_LINES];
    for (int i = 0; i<NUM_LINES; ++i)
            for (int j = 0; j < NUM_LINES; ++j)
            {
                float alpha = j*2.0*M_PI / NUM_LINES;
                float beta = i*2.0*M_PI / NUM_LINES;
                //сферич. ==> декарт.
                vertices[i][j](1,0) = afterProgectionR*cos(alpha);
                vertices[i][j](0,0) = afterProgectionR*sin(alpha)*sin(beta);
                vertices[i][j](2,0) = afterProgectionR*sin(alpha)*cos(beta);
                //поворот вокруг x
                const float Dalpha = M_PI / 4;
                Point3d newp;
                newp(0,0) = vertices[i][j](0,0);
                newp(1,0) = vertices[i][j](1,0)*cos(Dalpha) - vertices[i][j](2,0)*sin(Dalpha);
                newp(2,0) = vertices[i][j](1,0)*sin(Dalpha) + vertices[i][j](2,0)*cos(Dalpha);
                vertices[i][j] = newp;
                //поворот вокруг z
                const float Dbeta = M_PI / 4;
                newp(0,0) = vertices[i][j](0,0)*cos(Dbeta) - vertices[i][j](1,0)*sin(Dbeta);
                newp(1,0) = vertices[i][j](0,0)*sin(Dbeta) + vertices[i][j](1,0)*cos(Dbeta);
                newp(2,0) = vertices[i][j](2,0);
                vertices[i][j] = newp + center;
                //перенос на центр экрана

                vertices[i][j](3,0) = 1.0;

                vertices[i][j] = (camera.getCameraMatrix() * vertices[i][j]);
                vertices[i][j] /= vertices[i][j](3,0);

                vertices[i][j] = camera.getProectionMatrix() * vertices[i][j];
                vertices[i][j] /= vertices[i][j](3,0);

                vertices[i][j](0,0) = vertices[i][j](0,0) * camera.getWidth() / 2 + camera.getWidth() / 2;
                vertices[i][j](1,0) = -vertices[i][j](1,0) * camera.getHeight() / 2 + camera.getHeight() / 2;
            }
    for (int parallel = 0; parallel < NUM_LINES; ++parallel) {
        QPointF prev(vertices[parallel][NUM_LINES - 1](0,0), vertices[parallel][NUM_LINES - 1](1,0));
            for (int j = 0; j < NUM_LINES; ++j){
                QPointF cur(vertices[parallel][j](0,0), vertices[parallel][j](1,0));
                p.drawLine(prev, cur);
                prev = cur;
            }
        }
        for (int meridian = 0; meridian < NUM_LINES; ++meridian) {
            QPointF prev(vertices[NUM_LINES - 1][meridian](0,0), vertices[NUM_LINES - 1][meridian](1,0));
            for (int j = 0; j < NUM_LINES; ++j){
                QPointF cur(vertices[j][meridian](0,0), vertices[j][meridian](1,0));
                p.drawLine(prev, cur);
                prev = cur;

            }
        }

}






