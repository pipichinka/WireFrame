#ifndef WIREFRAMERMODEL_H
#define WIREFRAMERMODEL_H

#include <QImage>
#include <vector>
#include <QPoint>
#include <QPointF>
#include <QColor>
#include "viewinterface.h"
#include "applicationconfig.h"
#include "sphere.h"
#include "quadrangle.h"
#include "triangle.h"
#include "box.h"
#include "lightsource.h"
#include <QProgressBar>

#define KA 0.5

class WireFrameRModel
{
    ViewInterface* view;
    QImage* image;
    int height;
    int width;
    std::vector<SceneObjectInterface*> objects;
    std::vector<LightSource> lights;
    Camera camera;
    double aR;
    double aG;
    double aB;
    double bR;
    double bG;
    double bB;
    int depth;
    double gamma;

    void normalizeObjects();
    void rePaint();
    bool checkBlockPoint(const Point3d& p, const Point3d& dp);
    Point3d calcOneInternalRay(const Point3d& p0, const Point3d& dp, int d, double& distance);
    Point3d calcOneFrameRay(const Point3d& p0, const Point3d& dp, double& distance);
public:
    WireFrameRModel(int height, int width, const RenderConfig& render, const SceneConfig& scene);
    QImage* getImage(){return image;}
    void setView(ViewInterface* view){this->view = view;}
    void setConfigs(RenderConfig render, SceneConfig scene);
    void moveEngleByMouse(int x, int y);
    void setSize(int height, int width);
    void zoom(int val);
    void stepForward();
    void stepBack();
    void stepLeft();
    void stepRight();
    void calcReyTracing(QImage* image, QProgressBar* progress);
};

#endif // WIREFRAMERMODEL_H
