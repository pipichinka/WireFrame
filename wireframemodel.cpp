#include "wireframemodel.h"
#include <iostream>
#include <math.h>
#include <QPainter>
WireFrameModel::WireFrameModel(int width, int height): BSplineModel(width, height),
    view(nullptr),image(new QImage(width, height, QImage::Format::Format_ARGB32)),
    height(height), width(width), M(3), M1(1)
{
    double rotationMatrixData[] = {1,0,0,0,
                                   0,1,0,0,
                                   0,0,1,0,
                                   0,0,0,1};
    rotaitionMatrix = QGenericMatrix<4,4, double>(rotationMatrixData);

    double cameraMatrixData[] = {1,0,  0,  0,
                                 0,2000,0,  0,
                                 0,0, 2000,0,
                                 1,0,  0,  10};
    cameraMatrix = QGenericMatrix<4,4, double>(cameraMatrixData);

}


void WireFrameModel::setSize(int width, int height){
    this->width = width;
    this->height = height;
    delete image;
    image = new QImage(width, height, QImage::Format::Format_ARGB32);
    rePaint(false);
}


void WireFrameModel::setN(int n){
    BSplineModel::setN(n);
    rePaint(true);
}


void WireFrameModel::setM(int m){
    this->M = m;
    rePaint(true);
}


void WireFrameModel::setM1(int m1){
    this->M1 = m1;
    rePaint(true);
}


QPointF* WireFrameModel::movePickedPoint(int x, int y){
    QPointF* res = BSplineModel::movePickedPoint(x, y);
    if (res != nullptr){
        rePaint(true);
    }
    return res;
}


QPointF* WireFrameModel::movePickedPointDirect(double x, double y){
    QPointF* res = BSplineModel::movePickedPointDirect(x, y);
    if (res != nullptr){
        rePaint(true);
    }
    return res;
}


QPointF* WireFrameModel::addPoint(int x, int y){
    QPointF* res = BSplineModel::addPoint(x, y);
    if (res != nullptr){
        rePaint(true);
    }
    return res;
}


QPointF* WireFrameModel::removePickedPoint(){
    QPointF* res = BSplineModel::removePickedPoint();
    rePaint(true);
    return res;
}


void WireFrameModel::zoom(int value){
    cameraMatrix(1,1) *= 1 + ((double)value / 100.0);
    cameraMatrix(2,2) *= 1 + ((double)value / 100.0);
    rePaint(false);
}


void WireFrameModel::moveEngleByMouse(int x, int y){
//    if (x != 0){
//        double phiX = ((M_PI) / 100.0) * x;
//        rotaitionMatrix = makeRotationMatrix(true, phiX) * rotaitionMatrix;
//    }
//    if (y != 0){
//        double phiY = ((M_PI) / 100.0) * y;
//        rotaitionMatrix = makeRotationMatrix(false, phiY) * rotaitionMatrix;
//    }
//    rePaint(false);
}


void WireFrameModel::clearAngle(){
    double rotationMatrixData[] = {1,0,0,0,
                                   0,1,0,0,
                                   0,0,1,0,
                                   0,0,0,1};
    rotaitionMatrix = QGenericMatrix<4,4, double>(rotationMatrixData);
    rePaint(false);
}


void WireFrameModel::setSplineColor(QRgb color){
    BSplineModel::setSplineColor(color);
    rePaint(false);
}


QPointF WireFrameModel::calcOnePointOnScreen(QGenericMatrix<1, 4, double>& point, QPainter& painter){
    Point3d afterRotation = rotaitionMatrix * point;
    double scale = (-afterRotation(0,0)) * 0.35 + 0.65;
    if (scale > 1){
        scale = 1.0;
    } else  if (scale < 0){
        scale = 0.0;
    }
    QRgb color = this->getSplineColor();
    int colorRed = (color & 0xFF)*scale;
    int colorBlue = ((color & 0xFF00) >> 8) * scale;
    int colorGreen = ((color & 0xFF0000) >> 16) * scale;
    QRgb resultColor = (colorRed) | ((colorBlue) << 8) | ((colorGreen) << 16) | 0xFF000000;
    painter.setPen(QPen(QColor(resultColor), 1, Qt::PenStyle::SolidLine, Qt::PenCapStyle::RoundCap, Qt::PenJoinStyle::RoundJoin));
    QGenericMatrix<1, 4, double> p(cameraMatrix * (afterRotation));
    double d = p.constData()[3];
    QPointF res(p.constData()[1] / d, p.constData()[2] / d);
    res.setX(res.x() + width / 2.0);
    res.setY( -res.y() + height / 2.0);

    return res;
}


void normalize3dPoints(std::vector<std::vector<Point3d>>& bspline3DPoints, std::vector<std::vector<Point3d>>& circles){
    double maxX = bspline3DPoints[0][0].constData()[0];
    double minX = bspline3DPoints[0][0].constData()[0];
    double maxY = bspline3DPoints[0][0].constData()[1];
    double minY = bspline3DPoints[0][0].constData()[1];
    double maxZ = bspline3DPoints[0][0].constData()[2];
    double minZ = bspline3DPoints[0][0].constData()[2];
    for (size_t i = 0; i < bspline3DPoints.size(); ++i){
        for (size_t j = 0; j < bspline3DPoints[i].size(); ++j){
            double x = bspline3DPoints[i][j].constData()[0];
            double y = bspline3DPoints[i][j].constData()[1];
            double z = bspline3DPoints[i][j].constData()[2];
            if (x > maxX){
                maxX = x;
            }
            if (x < minX){
                minX = x;
            }
            if (y > maxY){
                maxY = y;
            }
            if (y < minY){
                minY = y;
            }
            if (z < minZ){
                minZ= z;
            }
            if (z > maxZ){
                maxZ = z;
            }
        }
    }

    for (size_t i = 0; i < circles.size(); ++i){
        for (size_t j = 0; j < circles[i].size(); ++j){
            double x = circles[i][j].constData()[0];
            double y = circles[i][j].constData()[1];
            double z = circles[i][j].constData()[2];
            if (x > maxX){
                maxX = x;
            }
            if (x < minX){
                minX = x;
            }
            if (y > maxY){
                maxY = y;
            }
            if (y < minY){
                minY = y;
            }
            if (z < minZ){
                minZ= z;
            }
            if (z > maxZ){
                maxZ = z;
            }
        }
    }
    double xSize = maxX - minX;
    double ySize = maxY - minY;
    double zSize = maxZ - minZ;
    double maxSize = std::max(xSize, std::max(ySize, zSize)) / 2;
    double xCenter = (maxX + minX) / 2.0;
    double yCenter = (maxY + minY) / 2.0;
    double zCenter = (maxZ + minZ) / 2.0;
    double centerPointData[] = {xCenter, yCenter, zCenter, 0};
    Point3d centerPoint(centerPointData);
    for (size_t i = 0; i < bspline3DPoints.size(); ++i){
        for (size_t j = 0; j < bspline3DPoints[i].size(); ++j){
            bspline3DPoints[i][j] -= centerPoint;
            bspline3DPoints[i][j] /= maxSize;
            bspline3DPoints[i][j](3, 0) = 1;
        }
    }
    for (size_t i = 0; i < circles.size(); ++i){
        for (size_t j = 0; j < circles[i].size(); ++j){
            circles[i][j] -= centerPoint;
            circles[i][j] /= maxSize;
            circles[i][j](3, 0) = 1;
        }
    }
}


void WireFrameModel::rePaint(bool isFull){
    image->fill(Qt::black);
    QPainter painter(image);
    drawXYZ(painter);
    if (isFull){
        bspline3DPoints.clear();
        circles.clear();
        std::vector<QPointF> bsplinePoints = this->getBSplinePoints();
        if (bsplinePoints.size() == 0){
            if (view != nullptr){
                view->onModelChange();
            }
            return;
        }

        //строим 3д каркас из бсплайнов
        double MEngle = (M_PI * 2) / (M);
        for (int i = 0; i < M; ++i){
            double currentEngle = MEngle * i;
            double currentSin = sin(currentEngle);
            double currentCos = cos(currentEngle);
            std::vector<Point3d> currentPoints;
            for (size_t j = 0; j < bsplinePoints.size(); ++j){
                double x = bsplinePoints[j].y() * currentCos; //мы крутим в 2д координатах воокруг x, но при реобразовании
                double y = bsplinePoints[j].y() * currentSin; // в 3д мы считаем, что х стал z, а х и у крутятся вокруг него
                double vector[] = {x, y, bsplinePoints[j].x(), 1}; // собираем 4-вектор
                currentPoints.emplace_back(vector);
            }
            bspline3DPoints.emplace_back(currentPoints);
        }

        //строим груги
        double M1Engle = (M_PI * 2) / (M * M1);
        for (int i = 0; i < bspline3DPoints[0].size(); i += this->getN()){
            QPointF currentPoint = bsplinePoints[i];
            std::vector<Point3d> currentPoints;
            for (int j = 0; j < M * M1; ++j){
                double currentEngle = M1Engle * j;
                double currentSin = sin(currentEngle);
                double currentCos = cos(currentEngle);
                double x = currentPoint.y() * currentCos; //мы крутим в 2д координатах воокруг x, но при реобразовании
                double y = currentPoint.y() * currentSin; // в 3д мы считаем, что х стал z, а х и у крутятся вокруг него
                double vector[] = {x, y, currentPoint.x(), 1}; // собираем 4-вектор
                currentPoints.emplace_back(vector);
            }
            circles.emplace_back(currentPoints);
        }
        normalize3dPoints(bspline3DPoints, circles);//вписывание фигуры в единичный крвадрат
    }

    //дальше банально рисуем всё на экран
    for (size_t i = 0; i < bspline3DPoints.size(); ++i){
        QPointF prevOnScreenPoint = calcOnePointOnScreen(bspline3DPoints[i][0], painter);
        for (size_t j = 1; j < bspline3DPoints[i].size(); ++j){
            QPointF currentOnScreenPoint = calcOnePointOnScreen(bspline3DPoints[i][j], painter);
            painter.drawLine(prevOnScreenPoint, currentOnScreenPoint);
            prevOnScreenPoint = currentOnScreenPoint;
        }
    }
    for (size_t i = 0; i < circles.size(); ++i){
        QPointF startPoint = calcOnePointOnScreen(circles[i][0], painter);
        QPointF prevOnScreenPoint = startPoint;
        for (size_t j = 1; j < circles[i].size(); ++j){
            QPointF currentOnScreenPoint = calcOnePointOnScreen(circles[i][j], painter);
            painter.drawLine(prevOnScreenPoint, currentOnScreenPoint);
            prevOnScreenPoint = currentOnScreenPoint;
        }
        painter.drawLine(prevOnScreenPoint, startPoint);
    }

    if (view != nullptr){
        view->onModelChange();
    }
}

void WireFrameModel::drawXYZ(QPainter& painter){
    double xyzCameraMatrixData[] = {
            1, 0, 0, 0,
            0, 200, 0, 0,
            0, 0, 200, 0,
            1, 0, 0, 10
    };
    QGenericMatrix<4,4,double> xyzCameraMatrix(xyzCameraMatrixData);
    double centerPointData[] = {0, 0, 0, 1};
    double xPointData[] = {1, 0, 0, 1};
    double yPointData[] = {0, 1, 0, 1};
    double zPointData[] = {0, 0, 1, 1};
    Point3d centerPoint(centerPointData);
    Point3d xPoint(xPointData);
    Point3d yPoint(yPointData);
    Point3d zPoint(zPointData);
    Point3d centerPointOnScreenMatrix(xyzCameraMatrix * (rotaitionMatrix * centerPoint));
    Point3d xPointOnScreenMatrix(xyzCameraMatrix * (rotaitionMatrix * xPoint));
    Point3d yPointOnScreenMatrix(xyzCameraMatrix * (rotaitionMatrix * yPoint));
    Point3d zPointOnScreenMatrix(xyzCameraMatrix * (rotaitionMatrix * zPoint));
    QPointF centerPointOnScreen(centerPointOnScreenMatrix.constData()[1] / centerPointOnScreenMatrix.constData()[3],
            centerPointOnScreenMatrix.constData()[2] / centerPointOnScreenMatrix.constData()[3]);
    centerPointOnScreen.setX(centerPointOnScreen.x() + 20);
    centerPointOnScreen.setY( -centerPointOnScreen.y() + 20);

    QPointF xPointOnScreen(xPointOnScreenMatrix.constData()[1] / xPointOnScreenMatrix.constData()[3],
            xPointOnScreenMatrix.constData()[2] / xPointOnScreenMatrix.constData()[3]);
    xPointOnScreen.setX(xPointOnScreen.x() + 20);
    xPointOnScreen.setY( -xPointOnScreen.y() + 20);

    QPointF yPointOnScreen(yPointOnScreenMatrix.constData()[1] / yPointOnScreenMatrix.constData()[3],
            yPointOnScreenMatrix.constData()[2] / yPointOnScreenMatrix.constData()[3]);
    yPointOnScreen.setX(yPointOnScreen.x() + 20);
    yPointOnScreen.setY( -yPointOnScreen.y() + 20);

    QPointF zPointOnScreen(zPointOnScreenMatrix.constData()[1] / zPointOnScreenMatrix.constData()[3],
            zPointOnScreenMatrix.constData()[2] / zPointOnScreenMatrix.constData()[3]);
    zPointOnScreen.setX(zPointOnScreen.x() + 20);
    zPointOnScreen.setY( -zPointOnScreen.y() + 20);

    painter.setPen(QPen(QColor(Qt::green), 1, Qt::PenStyle::SolidLine, Qt::PenCapStyle::RoundCap, Qt::PenJoinStyle::RoundJoin));
    painter.drawLine(centerPointOnScreen, xPointOnScreen);

    painter.setPen(QPen(QColor(Qt::yellow), 1, Qt::PenStyle::SolidLine, Qt::PenCapStyle::RoundCap, Qt::PenJoinStyle::RoundJoin));
    painter.drawLine(centerPointOnScreen, yPointOnScreen);

    painter.setPen(QPen(QColor(Qt::red), 1, Qt::PenStyle::SolidLine, Qt::PenCapStyle::RoundCap, Qt::PenJoinStyle::RoundJoin));
    painter.drawLine(centerPointOnScreen, zPointOnScreen);
}


void WireFrameModel::readConfig(const ApplicationConfig &config){
    BSplineModel::readConfig(config);
    M = config.M;
    M1 = config.M1;
    rotaitionMatrix = config.rotationMatrix;
    cameraMatrix = config.cameraMatrix;
    rePaint(true);
}


void WireFrameModel::recordDataToConfig(ApplicationConfig &config){
    BSplineModel::recordDataToConfig(config);
    config.M = M;
    config.M1 = M1;
    config.rotationMatrix = rotaitionMatrix;
    config.cameraMatrix = cameraMatrix;
}





