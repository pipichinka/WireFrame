#include "applicationconfig.h"
#include <fstream>
#include <exception>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonParseError>
#include <iostream>

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
    if (res < 1 && res > 20){
        throw std::runtime_error("invalid N field in config, should be an integer from 1 to 20");
    }
    N = res;

    object = json.constFind("M");
    if (object == json.constEnd()){
        throw std::runtime_error("no M field in config");
    }
    res = object->toInt(-1);
    if (res < 2 && res > 20){
        throw std::runtime_error("invalid M field in config, should be an integer from 2 to 20");
    }
    M = res;

    object = json.constFind("M1");
    if (object == json.constEnd()){
        throw std::runtime_error("no M1 field in config");
    }
    res = object->toInt(-1);
    if (res < 2 && res > 20){
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
