#include "applicationconfig.h"
#include <fstream>
#include <exception>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>
#include <iostream>
#include <string>


RenderConfig::RenderConfig(const std::string& fileName){
    std::ifstream ifs(fileName, std::ios::in);

    ifs >> bR >> bG >> bB;
    ifs >> gamma;
    ifs >> depth;
    ifs >> quality;
    ifs >> eyeX >> eyeY >> eyeZ;
    ifs >> viewX >> viewY >> viewZ;
    ifs >> upX >> upY >> upZ;
    ifs >> zN >> zF;
    ifs >> screenWidth >> screenHeight;

}

void ObjectOpticParams::fromStreem(std::ifstream& ifs){
    ifs >> KDr >> KDg >> KDb >> KSr >> KSg >> KSb >> power;
}


LightSourceConfig::LightSourceConfig(std::ifstream& ifs){
    double x;
    double y;
    double z;
    ifs >> x >> y >> z;
    p(0, 0) = x;
    p(1, 0) = y;
    p(2, 0) = z;
    ifs >> r >> g >> b;
}


SphereConfig::SphereConfig(std::ifstream& ifs){
    double x;
    double y;
    double z;
    ifs >> x >> y >> z;
    center(0, 0) = x;
    center(1, 0) = y;
    center(2, 0) = z;
    ifs >> r;
    opticParams.fromStreem(ifs);
}


BoxConfig::BoxConfig(std::ifstream& ifs){
    double x;
    double y;
    double z;
    ifs >> x >> y >> z;
    min(0, 0) = x;
    min(1, 0) = y;
    min(2, 0) = z;
    ifs >> x >> y >> z;
    max(0, 0) = x;
    max(1, 0) = y;
    max(2, 0) = z;
    opticParams.fromStreem(ifs);
}


TriangleConfig::TriangleConfig(std::ifstream& ifs){
    double x;
    double y;
    double z;
    ifs >> x >> y >> z;
    p1(0, 0) = x;
    p1(1, 0) = y;
    p1(2, 0) = z;
    ifs >> x >> y >> z;
    p2(0, 0) = x;
    p2(1, 0) = y;
    p2(2, 0) = z;
    ifs >> x >> y >> z;
    p3(0, 0) = x;
    p3(1, 0) = y;
    p3(2, 0) = z;
    opticParams.fromStreem(ifs);
}


QuadrangleConfig::QuadrangleConfig(std::ifstream& ifs){
    double x;
    double y;
    double z;
    ifs >> x >> y >> z;
    p1(0, 0) = x;
    p1(1, 0) = y;
    p1(2, 0) = z;
    ifs >> x >> y >> z;
    p2(0, 0) = x;
    p2(1, 0) = y;
    p2(2, 0) = z;
    ifs >> x >> y >> z;
    p3(0, 0) = x;
    p3(1, 0) = y;
    p3(2, 0) = z;
    ifs >> x >> y >> z;
    p4(0, 0) = x;
    p4(1, 0) = y;
    p4(2, 0) = z;
    opticParams.fromStreem(ifs);
}



SceneConfig::SceneConfig(const std::string& fileName){
    std::ifstream ifs(fileName, std::ios::in);
    ifs >> r >> g >> b;
    int n;
    ifs >> n;
    for (int i = 0; i < n; i++){
        lights.push_back(LightSourceConfig(ifs));
    }

    while (!ifs.eof()){
        std::string type;
        ifs >> type;
        if (type == "SPHERE"){
            spheres.push_back(SphereConfig(ifs));
        } else if (type == "BOX"){
            boxes.push_back(BoxConfig(ifs));
        } else if (type == "TRIANGLE"){
            triangles.push_back(TriangleConfig(ifs));
        } else if (type == "QUADRANGLE"){
            quadrangles.push_back(QuadrangleConfig(ifs));
        } else {
            continue;
        }
    }
}


Point3d vectorMult(const Point3d& a, const Point3d& b){
    Point3d res;
    res(0, 0) = a(1, 0) * b(2, 0) - b(1, 0) * a(2, 0);
    res(1, 0) = - a(0, 0) * b(2, 0) + b(0, 0) * a(2, 0);
    res(2, 0) = a(0, 0) * b(1, 0) - b(0, 0) * a(1, 0);
    double norm = std::sqrt(res(0, 0) * res(0, 0) + res(1, 0) * res(1, 0) + res(2, 0) * res(2, 0));
    res /= norm;
    res(3, 0) = 1;
    return res;
}


double prod(const Point3d& a, const Point3d& b){
    return a(0,0) * b(0,0) + a(1,0) * b(1,0) + a(2,0) * b(2,0);
}


double norm(const Point3d& a){
    return a(0,0) * a(0,0) + a(1,0) * a(1,0) + a(2,0) * a(2,0);
}


double isIntersectPlane(const Point3d& p0, const Point3d& dp, Point3d& normal, Point3d& intersectPoint, double D){
    double V = prod(normal, dp);
    if (V > 0){
        normal -= normal;
        V = -V;
    }

    double V0 = -prod(normal, p0) - D;
    double t = V0 / V;
    if (t < 0){
        return false;
    }
    intersectPoint = p0 + t * dp;
    return true;
}


QGenericMatrix<4, 4, double> makeRotationMatrix(const Point3d& vec, double phi){
    double sinPhi = sin(phi);
    double cosPhi = cos(phi);
    double x = vec(0,0);
    double y = vec(1,0);
    double z = vec(2,0);
    double matrixData[] =
        {cosPhi + (1 - cosPhi)*x*x, (1 - cosPhi)*x*y, (1-cosPhi)*x*z + sinPhi*y, 0,
         (1 - cosPhi)*y*x + sinPhi*z, cosPhi + (1-cosPhi) * y * y, (1-cosPhi)*y*z -sinPhi*x, 0,
         (1-cosPhi)*z*x - sinPhi*y, (1 - cosPhi)*z*y + sinPhi * x, cosPhi + (1-cosPhi)*z*z, 0,
         0, 0, 0, 1};
    return QGenericMatrix<4, 4, double>(matrixData);

}


std::vector<char> readFile2(const std::string &fileName)
{
    std::ifstream ifs(fileName, std::ios::in | std::ios::binary | std::ios::ate);
    if (!ifs){
        throw std::runtime_error("can't open file for readding");
    }
    std::ifstream::pos_type fileSize = ifs.tellg();
    ifs.seekg(0, std::ios::beg);

    std::vector<char> bytes(fileSize);
    ifs.read(bytes.data(), fileSize);
    return bytes;
}

ApplicationConfig::ApplicationConfig(const std::string& fileName)
{
    std::vector<char> fileData = readFile2(fileName);
    QByteArray bytes(fileData.data(), fileData.size());
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(bytes, &error);
    if (error.error != QJsonParseError::NoError){
        throw std::runtime_error(error.errorString().toStdString());
    }
    if (!doc.isObject()){
        throw std::runtime_error("invalid config format");
    }
    QJsonObject json = doc.object();
    QJsonObject::const_iterator object = json.constFind("N");
    if (object == json.constEnd()){
        throw std::runtime_error("no N field in config");
    }
    int res = object->toInt(-1);
    if (res < 1 || res > 20){
        throw std::runtime_error("invalid N field in config, should be an integer from 1 to 20");
    }
    N = res;

    object = json.constFind("M");
    if (object == json.constEnd()){
        throw std::runtime_error("no M field in config");
    }
    res = object->toInt(-1);
    if (res < 2 || res > 20){
        throw std::runtime_error("invalid M field in config, should be an integer from 2 to 20");
    }
    M = res;

    object = json.constFind("M1");
    if (object == json.constEnd()){
        throw std::runtime_error("no M1 field in config");
    }
    res = object->toInt(-1);
    if (res < 1 || res > 20){
        throw std::runtime_error("invalid M1 field in config, should be an integer from 1 to 20");
    }
    M1 = res;

    object = json.constFind("points_color");
    if (object == json.constEnd()){
        throw std::runtime_error("no points_color field in config");
    }
    if (!object->isString()){
        throw std::runtime_error("points_color should be a string that contains argb");
    }
    bool ok = true;
    pointsColor = object->toString().toUInt(&ok);
    if (!ok){
        throw std::runtime_error("points_color should be a string that contains argb");
    }

    object = json.constFind("spline_color");
    if (object == json.constEnd()){
        throw std::runtime_error("no spline_color field in config");
    }
    if (!object->isString()){
        throw std::runtime_error("spline_color should be a string that contains argb");
    }
    ok = true;
    splineColor = object->toString().toUInt(&ok);
    if (!ok){
        throw std::runtime_error("spline_color should be a string that contains argb");
    }


    object = json.constFind("center_point");
    if (object == json.constEnd()){
        throw std::runtime_error("no center_point field in config");
    }
    if (!object->isArray()){
        throw std::runtime_error("center_point field should contain 2 double values:x,y");
    }
    QJsonArray arr = object->toArray();
    if (arr.size() != 2 || !arr[0].isDouble() || !arr[1].isDouble()){
        throw std::runtime_error("center_point field should contain 2 double values:x,y");
    }
    centerPoint.setX(arr[0].toDouble());
    centerPoint.setY(arr[1].isDouble());

    object = json.constFind("zoom_scale");
    if (object == json.constEnd()){
        throw std::runtime_error("no zoom_scale field in config");
    }
    if (!object->isDouble()){
        throw std::runtime_error("zoom_scale field should contain double value");
    }
    zoomScale = object->toDouble();

    object = json.constFind("rotation_matrix");
    if (object == json.constEnd()){
        throw std::runtime_error("no rotation_matrix field in config");
    }
    if (!object->isArray()){
        throw std::runtime_error("rotation_matrix field should contain an array with 16 double values ");
    }
    arr = object->toArray();
    if (arr.size() != 16){
        throw std::runtime_error("rotation_matrix field should contain an array with 16 double values ");
    }
    for (int i = 0; i < 4; ++i){
        for (int j = 0; j < 4; ++j){
            if (!arr[i * 4 + j].isDouble()){
                throw std::runtime_error("rotation_matrix field should contain an array with 16 double values ");
            }
            double value = arr[i * 4 + j].toDouble();
            if (std::abs(value) > 1.0){
                throw std::runtime_error("rotation_matrix field should contain an array with values with absolute value below 1");
            }
            rotationMatrix(i,j) = arr[i * 4 + j].toDouble();
        }
    }

    object = json.constFind("camera_matrix");
    if (object == json.constEnd()){
        throw std::runtime_error("no camera_matrix field in config");
    }
    if (!object->isArray()){
        throw std::runtime_error("camera_matrix field should contain an array with 16 double values ");
    }
    arr = object->toArray();
    if (arr.size() != 16){
        throw std::runtime_error("camera_matrix field should contain an array with 16 double values ");
    }
    for (int i = 0; i < 4; ++i){
        for (int j = 0; j < 4; ++j){
            if (!arr[i * 4 + j].isDouble()){
                throw std::runtime_error("camera_matrix field should contain an array with 16 double values ");
            }
            cameraMatrix(i,j) = arr[i * 4 + j].toDouble();
        }
    }


    object = json.constFind("points");
    if (object == json.constEnd()){
        throw std::runtime_error("no points field in config");
    }
    if (!object->isArray()){
        throw std::runtime_error("points field should contain an array with values that are array of 2 double values: x, y");
    }
    arr = object->toArray();
    for (size_t i = 0; i < arr.size(); ++i){
        if (!arr[i].isArray()){
            throw std::runtime_error("points field should contain an array with values that are array of 2 double values: x, y");
        }
        QJsonArray arrPoints = arr[i].toArray();
        if (arrPoints.size() != 2 || !arrPoints[0].isDouble() || !arrPoints[1].isDouble()){
            throw std::runtime_error("points field should contain an array with values that are array of 2 double values: x, y");
        }
        points.emplace_back(arrPoints[0].toDouble(), arrPoints[1].toDouble());
    }
}


ApplicationConfig::~ApplicationConfig(){
}


void ApplicationConfig::toFile(const std::string& fileName){
    std::ofstream file(fileName);
    if (!file){
        throw std::runtime_error("can't open file for writing");
    }
    QJsonObject json;
    json.insert("N", N);
    json.insert("M", M);
    json.insert("M1", M1);
    json.insert("points_color", QString::number(pointsColor));
    json.insert("spline_color", QString::number(splineColor));
    json.insert("center_point", QJsonArray({centerPoint.x(), centerPoint.y()}));
    json.insert("zoom_scale", zoomScale);
    QJsonArray rotationMatrixArray;
    QJsonArray cameraMatrixArray;
    for (int i = 0; i < 4; ++i){
        for (int j = 0; j < 4; ++j){
            rotationMatrixArray.append(rotationMatrix(i,j));
            cameraMatrixArray.append(cameraMatrix(i,j));
        }
    }
    json.insert("rotation_matrix", rotationMatrixArray);
    json.insert("camera_matrix", cameraMatrixArray);
    QJsonArray pointsArray;
    for (size_t i = 0; i < points.size(); ++i){
        pointsArray.append(QJsonArray({points[i].x(), points[i].y()}));
    }
    json.insert("points", pointsArray);
    QJsonDocument doc(json);
    QByteArray array = doc.toJson();
    file.write(array.constData(), array.size());
    file.flush();
    if (file.fail()){
        throw std::runtime_error("can't write all data to file");
    }
}
