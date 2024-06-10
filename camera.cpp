#include "camera.h"

Camera::Camera(const RenderConfig& conf, int height, int width):
    zf(conf.zF),
    zb(conf.zN),
    height(height),
    width(width),
    sw(conf.screenWidth),
    sh(conf.screenHeight)
{
    pos(0, 0) = conf.eyeX;
    pos(1, 0) = conf.eyeY;
    pos(2, 0) = conf.eyeZ;
    pos(3, 0) = 1.0;

    up(0, 0) = conf.upX;
    up(1, 0) = conf.upY;
    up(2, 0) = conf.upZ;
    up(3, 0) = 1.0;

    forward(0, 0) = conf.viewX;
    forward(1, 0) = conf.viewY;
    forward(2, 0) = conf.viewZ;
    forward(3, 0) = 1.0;

    forward = forward - pos;
    double forwardNorm = std::sqrt(forward(0,0) * forward(0, 0) + forward(1,0) * forward(1, 0) + forward(2,0) * forward(2, 0));
    forward /= forwardNorm;
    forward(3, 0) = 1.0;
    right = vectorMult(up, forward);
    up = vectorMult(forward, right);
    cameraMatrix = makeRotMatrix() * makeMoveMatrix();
    proectionMatrix = makeProectionMatrix();
}

void Camera::zoom(double m){
    zf *= m;
    proectionMatrix = makeProectionMatrix();
}


QGenericMatrix<4,4, double> Camera::makeRotMatrix(){

    double data[] = {
        right(0, 0), right(1, 0), right(2, 0), 1,
        up(0, 0), up(1, 0), up(2, 0), 1.0,
        forward(0, 0), forward(1, 0), forward(2, 0), 1,
        0, 0, 0, 1
    };
    return QGenericMatrix<4,4, double>(data);
}

QGenericMatrix<4,4, double> Camera::makeMoveMatrix(){
    double data[] = {
        1, 0, 0, -pos(0,0),
        0, 1, 0, -pos(1,0),
        0, 0, 1, -pos(2, 0),
        0, 0, 0, 1
    };
    return QGenericMatrix<4,4, double>(data);
}


QGenericMatrix<4,4, double> Camera::makeProectionMatrix(){
    double data[] = {
        (2.0 / sw) * zf , 0, 0, 0,
        0, (2.0 / sh) * zf, 0, 0,
        0, 0, zb / (zb - zf), -zf * zb / (zb - zf),
        0, 0, 1, 0
    };
    return QGenericMatrix<4,4, double>(data);
}


void Camera::moveRight(double t){
    pos += right * t;
    cameraMatrix = makeRotMatrix() * makeMoveMatrix();
}


void Camera::moveLeft(double t){
    pos -= right * t;
    cameraMatrix = makeRotMatrix() * makeMoveMatrix();
}


void Camera::moveForward(double t){
    pos += forward * t;
    cameraMatrix = makeRotMatrix() * makeMoveMatrix();
}


void Camera::moveBack(double t){
    pos -= forward * t;
    cameraMatrix = makeRotMatrix() * makeMoveMatrix();
}


void Camera::rotate(double phi, bool isX){
    if (isX){
        right = makeRotationMatrix(up, phi) * right;
        double rightNorm = std::sqrt(right(0,0) * right(0,0) + right(1,0) * right(1,0) + right(2,0) * right(2,0));
        right /= rightNorm;
        right(3,0) = 1.0;
    } else {
        up = makeRotationMatrix(right, phi) * up;
        double rightNorm = std::sqrt(up(0,0) * up(0,0) + up(1,0) * up(1,0) + up(2,0) * up(2,0));
        up /= rightNorm;
        up(3,0) = 1.0;
    }
    forward = vectorMult(right, up);
    cameraMatrix = makeRotMatrix() * makeMoveMatrix();
}










