#ifndef APPLICATIONCONFIG_H
#define APPLICATIONCONFIG_H

#include <QGenericMatrix>
#include <QPointF>
#include <fstream>
#include <QColor>
#include <math.h>
typedef QGenericMatrix<1, 4, double> Point3d;
typedef QGenericMatrix<1, 3, double> Point3D;


Point3d vectorMult(const Point3d& a, const Point3d& b);


QGenericMatrix<4, 4, double> makeRotationMatrix(const Point3d& vec, double phi);


double prod(const Point3d& a, const Point3d& b);


double norm(const Point3d& a);


double isIntersectPlane(const Point3d& p0, const Point3d& dp, Point3d& normal, Point3d& intersectPoint, double D);

struct RenderConfig{
    int bR;
    int bG;
    int bB;
    double gamma;
    int depth;
    std::string quality;
    double eyeX;
    double eyeY;
    double eyeZ;
    double viewX;
    double viewY;
    double viewZ;
    double upX;
    double upY;
    double upZ;
    double zN;
    double zF;
    double screenWidth;
    double screenHeight;

    RenderConfig() = default;
    RenderConfig(const std::string& fileName);
};


struct ObjectOpticParams{
    double KDr;
    double KDg;
    double KDb;
    double KSr;
    double KSg;
    double KSb;
    double power;

    void fromStreem(std::ifstream& ifs);
};


struct SphereConfig{
    Point3D center;
    double r;
    ObjectOpticParams opticParams;

    SphereConfig(std::ifstream& ifs);
};


struct BoxConfig{
    Point3D min;
    Point3D max;
    ObjectOpticParams opticParams;

    BoxConfig(std::ifstream& ifs);
};


struct TriangleConfig{
    Point3D p1;
    Point3D p2;
    Point3D p3;
    ObjectOpticParams opticParams;

    TriangleConfig(std::ifstream& ifs);
};


struct QuadrangleConfig{
    Point3D p1;
    Point3D p2;
    Point3D p3;
    Point3D p4;
    ObjectOpticParams opticParams;

    QuadrangleConfig(std::ifstream& ifs);
};


struct LightSourceConfig{
    Point3D p;
    int r;
    int g;
    int b;
    LightSourceConfig(std::ifstream& ifs);
};


struct SceneConfig{
    int r;
    int g;
    int b;
    std::vector<LightSourceConfig> lights;
    std::vector<SphereConfig> spheres;
    std::vector<BoxConfig> boxes;
    std::vector<TriangleConfig> triangles;
    std::vector<QuadrangleConfig> quadrangles;
    SceneConfig(const std::string& fileName);
};


struct ApplicationConfig
{
    int N;
    int M;
    int M1;
    QRgb pointsColor;
    QRgb splineColor;
    QPointF centerPoint;
    double zoomScale;
    QGenericMatrix<4,4, double> rotationMatrix;
    QGenericMatrix<4,4, double> cameraMatrix;
    std::vector<QPointF> points;


    ApplicationConfig() = default;
    ApplicationConfig(const std::string& fileName);
    void toFile(const std::string& fileName);
    ~ApplicationConfig();
};

#endif // APPLICATIONCONFIG_H
