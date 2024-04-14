#include "bsplinemodel.h"
#include<QPainter>
#include <iostream>
#include <QGenericMatrix>
BSplineModel::BSplineModel(int width, int height):
    view(nullptr),
    image(new QImage(width, height, QImage::Format::Format_ARGB32)),
    points(), N(1), centerPoint(0, 0), zoomScale(1.0),
    splineColor(QColor(Qt::blue).rgba()),
    pointsColor(QColor(Qt::black).rgba()),
    width(width), height(height)
{
    rePaint();
}


void BSplineModel::rePaint(){
    image->fill(Qt::white);
    QPainter painter(image);
    painter.setPen(QPen(QColor(Qt::black), 1, Qt::PenStyle::SolidLine, Qt::PenCapStyle::RoundCap, Qt::PenJoinStyle::RoundJoin));

    //рисование координатныых осей (идея в том, чтобы определить, где
    QPointF centerOnScreen = {0.0,0.0};
    centerOnScreen -= centerPoint;//переходим в координаты центра
    centerOnScreen *= zoomScale;// учитываем пиближение
    centerOnScreen.setX(centerOnScreen.x() + width / 2.0); // переводим в координаты экрана
    centerOnScreen.setY(-centerOnScreen.y() + height / 2.0);
    painter.drawText(centerOnScreen, std::to_string(0).data());
    QPointF prevCoordPoint = centerOnScreen;
    // дальше просто ходим в четырёх напралениях из точки текущего центра
    while (prevCoordPoint.x() < width){
        QPointF currentPoint = prevCoordPoint + QPointF(50, 0);
        double onScreenX = (currentPoint.x() - width / 2.0) / zoomScale + centerPoint.x(); // считаем реальную координату а не ту которая на картинке
        painter.drawText(currentPoint, std::to_string((int) onScreenX).data());
        painter.drawLine(prevCoordPoint, currentPoint);
        prevCoordPoint = currentPoint;
    }
    prevCoordPoint = centerOnScreen;
    while (prevCoordPoint.x() > 0){
        QPointF currentPoint = prevCoordPoint + QPointF(-50, 0);
        double onScreenX = (currentPoint.x() - width / 2.0) / zoomScale + centerPoint.x();// считаем реальную координату а не ту которая на картинке
        painter.drawText(currentPoint, std::to_string((int) onScreenX).data());
        painter.drawLine(prevCoordPoint, currentPoint);
        prevCoordPoint = currentPoint;
    }

    prevCoordPoint = centerOnScreen;
    while (prevCoordPoint.y() < height){
        QPointF currentPoint = prevCoordPoint + QPointF(0, 50);
        double onScreenY = (-currentPoint.y() + height / 2.0) / zoomScale + centerPoint.y();// считаем реальную координату а не ту которая на картинке
        painter.drawText(currentPoint, std::to_string( (int) onScreenY).data());
        painter.drawLine(prevCoordPoint, currentPoint);
        prevCoordPoint = currentPoint;
    }

    prevCoordPoint = centerOnScreen;
    while (prevCoordPoint.y() > 0){
        QPointF currentPoint = prevCoordPoint + QPointF(0, -50);
        double onScreenY = (-currentPoint.y() + height / 2.0) / zoomScale + centerPoint.y();// считаем реальную координату а не ту которая на картинке
        painter.drawText(currentPoint, std::to_string((int) onScreenY).data());
        painter.drawLine(prevCoordPoint, currentPoint);
        prevCoordPoint = currentPoint;
    }

    if (points.size() == 0){
        if (view != nullptr){
            view->onModelChange();
        }
        return;
    }


    painter.setPen(QPen(QColor(pointsColor), 1, Qt::PenStyle::SolidLine, Qt::PenCapStyle::RoundCap, Qt::PenJoinStyle::RoundJoin));
    // рисование линий образующих бсплайна
    QPointF prevPoint(points[0]->x(), points[0]->y());
    prevPoint -= centerPoint;//переходим в координаты центра
    prevPoint *= zoomScale;// учитываем пиближение
    prevPoint.setX(prevPoint.x() + width / 2.0); // переводим в координаты экрана
    prevPoint.setY(-prevPoint.y() + height / 2.0);// переводим в координаты экрана
    painter.drawEllipse(prevPoint, 5.0, 5.0);
    if (points.size() == 1){
        if (view != nullptr){
            view->onModelChange();
        }
        return;
    }
    for (size_t i = 1; i < points.size(); ++i){
       QPointF currentPoint (*points[i]);
       currentPoint -= centerPoint;
       currentPoint *= zoomScale;
       currentPoint.setX(currentPoint.x() + width / 2.0);
       currentPoint.setY(-currentPoint.y() + height / 2.0);
       painter.drawLine(prevPoint, currentPoint);
       painter.drawEllipse(currentPoint, 10.0, 10.0);
       prevPoint = currentPoint;
    }


    //рисование самого бсплайна
    if (points.size() < 4){
        if (view != nullptr){
            view->onModelChange();
        }
        return;
    }
    painter.setPen(QPen(QColor(splineColor), 1, Qt::PenStyle::SolidLine, Qt::PenCapStyle::RoundCap, Qt::PenJoinStyle::RoundJoin));
    int start = 0;
    double MsplineValues[] = {-1.0, 3.0, -3.0, 1.0,
                     3.0, -6.0, 3.0, 0.0,
                     -3.0, 0.0, 3.0, 0.0,
                     1.0, 4.0, 1.0, 0.0};
    QGenericMatrix<4, 4, double> Mspline(MsplineValues); // просто матрица из задания
    Mspline *= (1.0 / 6.0);

    double TMult = 1.0 / (double) N; // шаг с которым мы берём t в формулах

    while (start < points.size() - 3){
        double pointsValues[] = {points[start]->x(), points[start]->y(),
                                 points[start + 1]->x(), points[start + 1]->y(),
                                 points[start + 2]->x(), points[start + 2]->y(),
                                 points[start + 3]->x(), points[start + 3]->y(),
                                };
        QGenericMatrix<2, 4, double> pointsMatrix(pointsValues); // матрица из координат

        pointsMatrix = Mspline * pointsMatrix; // получили вектор точек для текущего участка

        double TmatrixValues[] = {0.0, 0.0, 0.0, 1.0}; // матрица для t равного 0

        QGenericMatrix<4, 1, double> TMatrix(TmatrixValues);
        QGenericMatrix<2, 1, double> prevMatrixPoint = TMatrix * pointsMatrix; //

        QPointF prevPoint(prevMatrixPoint.constData()[0], prevMatrixPoint.constData()[1]);
        prevPoint -= centerPoint;//переходим в координаты центра
        prevPoint *= zoomScale;// учитываем пиближение
        prevPoint.setX(prevPoint.x() + width / 2.0); // переводим в координаты экрана
        prevPoint.setY(-prevPoint.y() + height / 2.0);// переводим в координаты экрана

        for (int i = 1; i <= N; i++){
            double t = (i * TMult);
            double currentTValues[] = {
                t * t * t,
                t * t,
                t,
                1
            };
            QGenericMatrix<4, 1, double> currentTMatrix(currentTValues);
            QGenericMatrix<2, 1, double> currentMatrixPoint = currentTMatrix * pointsMatrix;

            QPointF currentPoint(currentMatrixPoint.constData()[0], currentMatrixPoint.constData()[1]);
            currentPoint -= centerPoint;//переходим в координаты центра
            currentPoint *= zoomScale;// учитываем пиближение
            currentPoint.setX(currentPoint.x() + width / 2.0); // переводим в координаты экрана
            currentPoint.setY(-currentPoint.y() + height / 2.0);// переводим в координаты экрана
            painter.drawLine(prevPoint, currentPoint);
            prevPoint = currentPoint;
        }
        start++;

    }


    if (view != nullptr){
        view->onModelChange();
    }
}


/*
 * мы хотим найти точки прямогугольника в кторый впишутся все наши точки
 * это будет банальный поиск минимума и максимума
 * для поиска нужного приближения или отдаления мы смотрим насколько нужно приблизить
 * чтобы вписать этот прямоугольник в экран
 */


void BSplineModel::normalize(){
    if (points.size() == 0){
        centerPoint.setX(0.0);
        centerPoint.setY(0.0);
        rePaint();
        return;
    }
    else if (points.size() == 1){
        centerPoint = *points[0];
        rePaint();
        return;
    }
    double maxX = points[0]->x();
    double minX = points[0]->x();
    double maxY = points[0]->y();
    double minY = points[0]->y();
    for (size_t i = 1; i < points.size(); ++i){
        double x = points[i]->x();
        double y = points[i]->y();
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
    }
    double rectWidth = maxX - minX + 20;
    double rectHeight = maxY - minY + 20; // 20 добавляется, чтобы не было деления на ноль и
    centerPoint.setX((maxX + minX) / 2.0); // чтобы точки немного отходили от краёв
    centerPoint.setY((maxY + minY) / 2.0);
    zoomScale = 1.0 / std::max(rectWidth / (double) width, rectHeight / (double) height);
    rePaint();

}


void BSplineModel::addPoint(int x, int y){
    double xCenter = (double) x - (double) width/2.0; // переводим в реальные координаты
    double yCenter = (double) -y + (double) height/2.0; // переводим в реальные координаты
    double xScale = xCenter * (1 / zoomScale); // учитываем приближение (обратное)
    double yScale = yCenter * (1 / zoomScale); // учитываем приближение (обратное)
    double xDiff = xScale + centerPoint.x(); //добавляем координаты центра
    double yDiff = yScale + centerPoint.y(); // добавляем координаты центра
    points.emplace_back(new QPointF(xDiff, yDiff));
    rePaint();
}


void BSplineModel::zoomIn(){
    if (zoomScale < 5.0){
        zoomScale += 0.1;
        rePaint();
    }
}


void BSplineModel::zoomOut(){
    if (zoomScale > 0.2){
        zoomScale -=0.1;
        rePaint();
    }
}

//я смещаюсь на рассояние, такое чтобы оно бяло одинаковое на экране
void BSplineModel::stepLeft(){
    centerPoint += QPointF(- STEP * (1.0 / zoomScale), 0);
    rePaint();
}

//я смещаюсь на рассояние, такое чтобы оно бяло одинаковое на экране
void BSplineModel::stepRight(){
    centerPoint += QPointF(STEP * (1.0 / zoomScale), 0);
    rePaint();
}

//я смещаюсь на рассояние, такое чтобы оно бяло одинаковое на экране
void BSplineModel::stepUp(){
    centerPoint += QPointF(0, STEP * (1.0 / zoomScale));
    rePaint();
}

//я смещаюсь на рассояние, такое чтобы оно бяло одинаковое на экране
void BSplineModel::stepDown(){
    centerPoint += QPointF(0, -STEP * (1.0 / zoomScale));
    rePaint();
}




