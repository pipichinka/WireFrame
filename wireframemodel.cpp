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
    rePaint(false);
}


void WireFrameModel::setM1(int m1){
    this->M1 = m1;
    rePaint(false);
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
    std::cout << "zoom in " << std::endl;
    cameraMatrix(1,1) *= 1 + ((double)value / 100.0);
    cameraMatrix(2,2) *= 1 + ((double)value / 100.0);
    rePaint(false);
}


QGenericMatrix<4, 4, double> makeRotationMatrix(bool isX, double phi){
    double sinPhi = sin(phi);
    double cosPhi = cos(phi);
    if (isX){
        double matrixData[] =
        {1, 0, 0, 0,
         0, cosPhi, -sinPhi, 0,
         0, sinPhi, cosPhi, 0,
         0, 0, 0, 1};
        return QGenericMatrix<4, 4, double>(matrixData);
    }
    else {//y
        double matrixData[] =
        {cosPhi, 0, sinPhi, 0,
        0, 1, 0, 0,
        -sinPhi, 0, cosPhi, 0,
        0,0,0,1};
        return QGenericMatrix<4, 4, double>(matrixData);
    }
}


void WireFrameModel::moveEngleByMouse(int x, int y){
    if (x != 0){
        double phiX = ((M_PI) / 100.0) * x;
        rotaitionMatrix = makeRotationMatrix(true, phiX) * rotaitionMatrix;
    }
    if (y != 0){
        double phiY = ((M_PI) / 100.0) * y;
        rotaitionMatrix = makeRotationMatrix(false, phiY) * rotaitionMatrix;
    }
    rePaint(false);
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
    double scale = (afterRotation(0,0)) * 0.5 + 0.5;
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
    painter.setPen(QPen(QColor(this->getSplineColor()), 1, Qt::PenStyle::SolidLine, Qt::PenCapStyle::RoundCap, Qt::PenJoinStyle::RoundJoin));
    std::vector<QPointF> bsplinePoints = this->getBSplinePoints();
    if (bsplinePoints.size() == 0){
        if (view != nullptr){
            view->onModelChange();
        }
        return;
    }

    //строим 3д каркас из бсплайнов
    std::vector<std::vector<Point3d>> bspline3DPoints;
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
    std::vector<std::vector<Point3d>> circles;
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











