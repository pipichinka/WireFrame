#include "wireframermodel.h"
#include <iostream>
#include <QPair>
WireFrameRModel::WireFrameRModel(int height, int width, const RenderConfig& render, const SceneConfig& scene):
    view(nullptr),image(new QImage(width, height, QImage::Format::Format_ARGB32)),
    camera(render, height, width),
    aR(scene.r / 255.0), aG(scene.g / 255.0), aB(scene.b / 255.0),
    bR(render.bR / 255.0), bG(render.bG / 255.0), bB(render.bB / 255.0),
    depth(render.depth), gamma(render.gamma)
{
    for (const auto& o: scene.boxes){
        objects.push_back(new Box(o));
    }
    for (const auto& o: scene.spheres){
        qDebug() << o.center;
        objects.push_back(new Sphere(o));
    }
    for (const auto& o: scene.triangles){
        objects.push_back(new Triangle(o));
    }
    for (const auto& o: scene.quadrangles){
        objects.push_back(new Quadrangle(o));
    }

    for (const auto& l: scene.lights){
        lights.push_back(LightSource(l));
    }

    normalizeObjects();
    rePaint();
}

void WireFrameRModel::setConfigs(RenderConfig render, SceneConfig scene){
    for (SceneObjectInterface* o: objects){
        delete o;
    }
    aR = scene.r / 255.0;
    aG = scene.g / 255.0;
    aB = scene.b / 255.0;
    bR = render.bR / 255.0;
    bG = render.bG / 255.0;
    bB = render.bB / 255.0;
    depth = render.depth;
    gamma = render.gamma;
    objects.clear();
    camera = Camera(render, camera.getHeight(), camera.getWidth());
    for (const auto& o: scene.boxes){
        objects.push_back(new Box(o));
    }
    for (const auto& o: scene.spheres){
                qDebug() << o.center;
        objects.push_back(new Sphere(o));

    }
    for (const auto& o: scene.triangles){
        objects.push_back(new Triangle(o));
    }
    for (const auto& o: scene.quadrangles){
        objects.push_back(new Quadrangle(o));
    }
    lights.clear();
    for (const auto& l: scene.lights){
        lights.push_back(LightSource(l));
    }
    normalizeObjects();
    rePaint();
}


void WireFrameRModel::zoom(int val){
    camera.zoom(1 + ((double)val / 100.0));
    rePaint();
}


void WireFrameRModel::rePaint(){
    image->fill(Qt::white);
    if (objects.size() == 0){
        return;
    }
    QPainter p(image);

    p.setPen(QPen(QColor(Qt::black), 2, Qt::PenStyle::SolidLine, Qt::PenCapStyle::RoundCap, Qt::PenJoinStyle::RoundJoin));
    for (size_t i = 0; i < objects.size(); i++){
        objects[i]->drowWireFrame(p, camera);
    }
    if (view != NULL){
        view->onModelChange();
    }
    p.end();
}


void WireFrameRModel::moveEngleByMouse(int x, int y){
    if (x != 0){
        double phiX = ((M_PI) / 1000.0) * x;
        camera.rotate(-phiX, true);
    }
    if (y != 0){
        double phiY = ((M_PI) / 1000.0) * y;
        camera.rotate(-phiY, false);
    }
    rePaint();
}


void WireFrameRModel::stepForward(){
    camera.moveForward(0.1);
    rePaint();
}


void WireFrameRModel::stepBack(){
    camera.moveBack(0.1);
    rePaint();
}


void WireFrameRModel::stepLeft(){
    camera.moveLeft(0.1);
    rePaint();
}


void WireFrameRModel::stepRight(){
    camera.moveRight(0.1);
    rePaint();
}


void WireFrameRModel::setSize(int height, int width){
    camera.setHeight(height);
    camera.setWidth(width);
    delete image;
    image = new QImage(width, height, QImage::Format::Format_ARGB32);
    rePaint();
}


bool WireFrameRModel::checkBlockPoint(const Point3d& p, const Point3d& dp){
    Point3d normal;
    Point3d intersectPoint;
    for (size_t i = 0; i < objects.size(); i++){
        Point3d curNormal;
        Point3d curIntersectPoint;
        if (objects[i]->isIntersect(p, dp, curNormal, curIntersectPoint)){
            return false;
        }
    }
    return true;
}


Point3d WireFrameRModel::calcOneInternalRay(const Point3d& p0, const Point3d& dp, int d, double& distance){
    SceneObjectInterface* o = nullptr;
    double tMin = INFINITY;
    Point3d normal;
    Point3d intersectPoint;
    for (size_t i = 0; i < objects.size(); i++){
        Point3d curNormal;
        Point3d curIntersectPoint;
        if (objects[i]->isIntersect(p0, dp, curNormal, curIntersectPoint)){
            double t = (curIntersectPoint(0,0) - p0(0,0)) / dp(0,0);
            if (t < tMin){
                tMin = t;
                normal = curNormal;
                intersectPoint = curIntersectPoint;
                o = objects[i];
            }
        }
    }

    if (o == nullptr){
        double data[] = {0.0, 0.0, 0.0, 1.0};
        distance = 0.0;
        return Point3d(data);
    }
    distance = std::sqrt(norm(intersectPoint - p0));
    Point3d Ir;
    if (d < depth){
        Point3d reflectVec = 2 * prod(normal, -dp) * normal + dp;
        reflectVec /= std::sqrt(norm(reflectVec));
        double fAtt;
        Ir = calcOneInternalRay(intersectPoint, reflectVec, d + 1, fAtt);
        fAtt = 1.0 / (1.0 + fAtt);
        Ir *= fAtt;
    } else {
        Ir.fill(0.0);
        Ir(3, 0) = 1;
    }
    Point3d I;
    I.fill(0.0);
    for (const auto l: lights){
        Point3d ld = l.getPoint() - intersectPoint;
        Point3d ldN = ld / std::sqrt(norm(ld));
        if (!checkBlockPoint(intersectPoint, ldN)){
            continue;
        }
        double fAtt = 1.0 / (1.0 + std::sqrt(norm(ld)));
        double NL = prod(ldN, normal);
        Point3d reflectVec = 2 * prod(normal, ldN) * normal - ldN;
        reflectVec /= std::sqrt(norm(reflectVec));
        double RV = std::pow(prod(reflectVec, -dp), o->getOpticParams().power);
        I(0,0) += fAtt * l.getR() * (o->getOpticParams().KDr * NL + o->getOpticParams().KSr * RV);
        I(1,0) += fAtt * l.getG() * (o->getOpticParams().KDg * NL + o->getOpticParams().KSg * RV);
        I(2,0) += fAtt * l.getB() * (o->getOpticParams().KDb * NL + o->getOpticParams().KSb * RV);
    }
    I(0,0) += aR * KA + Ir(0,0) * o->getOpticParams().KSr;
    I(1,0) += aG * KA + Ir(1,0) * o->getOpticParams().KSg;
    I(2,0) += aB * KA + Ir(2,0) * o->getOpticParams().KSb;
    return I;
}


Point3d WireFrameRModel::calcOneFrameRay(const Point3d& p0, const Point3d& dp,  double& distance){
    SceneObjectInterface* o = nullptr;
    double tMin = INFINITY;
    Point3d normal;
    Point3d intersectPoint;
    for (size_t i = 0; i < objects.size(); i++){
        Point3d curNormal;
        Point3d curIntersectPoint;
        if (objects[i]->isIntersect(p0, dp, curNormal, curIntersectPoint)){
            double t = (curIntersectPoint(0,0) - p0(0,0)) / dp(0,0);
            if (t < tMin){
                tMin = t;
                normal = curNormal;
                intersectPoint = curIntersectPoint;
                o = objects[i];
            }
        }
    }

    if (o == nullptr){
        double data[] = {bR,bG,bB, 1.0};
        distance = 0.0;
        return Point3d(data);
    }
    distance = std::sqrt(norm(intersectPoint - p0));
    Point3d reflectVec = 2 * prod(normal, -dp) * normal + dp;
    reflectVec /= std::sqrt(norm(reflectVec));
    double fAtt;
    Point3d Ir = calcOneInternalRay(intersectPoint, reflectVec, 1, fAtt);
    fAtt = 1.0 / (1.0 + fAtt);
    Ir *= fAtt;
    Point3d I;
    I.fill(0.0);
    for (const auto l: lights){
        Point3d ld = l.getPoint() - intersectPoint;
        Point3d ldN = ld / std::sqrt(norm(ld));
        if (!checkBlockPoint(intersectPoint, ldN)){
            continue;
        }
        double fAtt = 1.0 / (1.0 + std::sqrt(norm(ld)));
        double NL = prod(ldN, normal);
        Point3d reflectVec = 2 * prod(normal, ldN) * normal - ldN;
        reflectVec /= std::sqrt(norm(reflectVec));
        double RV = std::pow(prod(reflectVec, -dp), o->getOpticParams().power);
        if (NL < 0 || RV < 0){
            continue;
        }
        I(0,0) += fAtt * l.getR() * (o->getOpticParams().KDr * NL + o->getOpticParams().KSr * RV);
        I(1,0) += fAtt * l.getG() * (o->getOpticParams().KDg * NL + o->getOpticParams().KSg * RV);
        I(2,0) += fAtt * l.getB() * (o->getOpticParams().KDb * NL + o->getOpticParams().KSb * RV);
    }
    I(0,0) += aR * KA + Ir(0,0) * o->getOpticParams().KSr;
    I(1,0) += aG * KA + Ir(1,0) * o->getOpticParams().KSg;
    I(2,0) += aB * KA + Ir(2,0) * o->getOpticParams().KSb;
    return I;
}


void applyGammaOnI(const Point3d& I, QImage* image, double gamma, int x, int y){
    int red = std::min((int)(std::pow(I(0,0), gamma) * 255.0), 255);
    int green = std::min((int)(std::pow(I(1,0), gamma) * 255.0), 255);
    int blue = std::min((int)(std::pow(I(2,0), gamma) * 255.0), 255);
    int resultColor = (red) | ((blue) << 8) | ((green) << 16) | 0xFF000000;
    image->setPixel(x, y, resultColor);
}





void WireFrameRModel::calcReyTracing(QImage* image, QProgressBar* progress){
    int height = image->height();
    int width = image->width();
    int p = 0;
    int Pstep = height * width / 100;
    progress->setValue(0);
    double dx = camera.getSW() / (double) width;
    double dy = camera.getSH() / (double) height;
    Point3d* Iimage = new Point3d[height * width];
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            Point3d x = dx *(j - width) * camera.getRight();
            Point3d y = dy *(-i) * camera.getUp();

            Point3d Isum;
            Isum.fill(0.0);
            for (int k = 0; k < 4; k++){
                double randomX = ((double) (rand() % 10) / 10.0) - 0.5;
                double randomY = ((double) (rand() % 10) / 10.0) - 0.5;
                Point3d xr = x + dx * camera.getRight() * randomX;
                Point3d yr = y + dy * camera.getUp() * randomY;
                Point3d vec = xr+ yr + camera.getFrameCenter() - camera.getPos();
                vec /= std::sqrt(norm(vec));
                vec(3,0) = 1;
                double d; // unused
                Isum += calcOneFrameRay(camera.getPos(), vec, d);
            }
            Iimage[i * width + j] = Isum / 4.0;
            if (i * width + j == Pstep * p){
                progress->setValue(p);
                p++;
            }
        }
    }
    double max= 1e-10;
    for (int i = 0; i < height * width; i++){
            if (Iimage[i](0,0) > max){
                max = Iimage[i](0,0);
            }
            if (Iimage[i](1,0) > max){
                max = Iimage[i](1,0);
            }
            if (Iimage[i](2,0) > max){
                max = Iimage[i](2,0);
            }
    }
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            Iimage[i * width + j](0,0) /= max;
            Iimage[i * width + j](1,0) /= max;
            Iimage[i * width + j](2,0) /= max;
            applyGammaOnI(Iimage[i * width + j], image, gamma, j ,i);
        }
    }
    delete [] Iimage;
    progress->setValue(100);
}


void WireFrameRModel::normalizeObjects(){
    Point3d startMax = objects[0]->getMaxPoint();
    Point3d startMin = objects[0]->getMinPoint();

    double maxX = startMax(0,0);
    double minX = startMin(0,0);
    double maxY = startMax(1,0);
    double minY = startMin(1,0);
    double maxZ = startMax(2,0);
    double minZ = startMin(2,0);
    for (size_t i = 0; i < objects.size(); ++i){
        Point3d curMax = objects[i]->getMaxPoint();
        Point3d curMin = objects[i]->getMinPoint();
        if (curMax(0,0) > maxX){
            maxX = curMax(0,0);
        }
        if (curMin(0,0) < minX){
            minX = curMin(0,0);
        }
        if (curMax(1,0) > maxY){
            maxY = curMax(1,0);
        }
        if (curMin(1,0) < minY){
            minY = curMin(1,0);
        }
        if (curMin(2,0) < minZ){
            minZ= curMin(2,0);
        }
        if (curMax(2,0) > maxZ){
            maxZ = curMax(2,0);
        }
    }
    double xSize = maxX - minX;
    double ySize = maxY - minY;
    double zSize = maxZ - minZ;
    double maxSize = (std::max(xSize, std::max(ySize, zSize)) / 2) * 1.05;
    double xCenter = (maxX + minX) / 2.0;
    double yCenter = (maxY + minY) / 2.0;
    double zCenter = (maxZ + minZ) / 2.0;
    double centerPointData[] = {xCenter, yCenter, zCenter, 0};
    Point3d centerPoint(centerPointData);
    for (size_t i = 0; i < objects.size(); i++){
        objects[i]->normalize(maxSize, centerPoint);
    }
}
