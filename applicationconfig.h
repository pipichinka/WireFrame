#ifndef APPLICATIONCONFIG_H
#define APPLICATIONCONFIG_H

#include <QGenericMatrix>
#include <QPointF>
#include <QColor>
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
