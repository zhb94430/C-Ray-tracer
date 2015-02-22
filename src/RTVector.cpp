//
//  RTVector.cpp
//  
//
//  Created by Peter Zhang on 14/10/18.
//
//

#include "RTVector.h"
#include <math.h>

RTVector::RTVector(double xi, double yi, double zi) {
    x = xi;
    y = yi;
    z = zi;
}

RTVector::RTVector() {
    x = 0;
    y = 0;
    z = 0;
}

RTVector RTVector::RTNormalize() {
    double m = sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
    RTVector value(x / m, y / m, z / m);
    return value;
}

double RTDot(RTVector v1, RTVector v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

RTVector RTMultiC(RTVector v, double c) {
    RTVector value(v.x * c, v.y * c, v.z * c);
    return value;
}

RTVector RTNormalize(RTVector v, double m) {
    RTVector value(v.x / m, v.y / m, v.z / m);
    return value;
}



//Calculate the sum of two vectors
RTVector RTAdd(RTVector v1, RTVector v2) {
    RTVector value(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
    return value;
}

//Calculate the subtraction of two vectors
RTVector RTMinus(RTVector v1, RTVector v2) {
    RTVector value(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
    return value;
}

RTVector RTCross(RTVector v1, RTVector v2) {
    double rx = v1.y * v2.z - v1.z * v2.y;
    double ry = v1.z * v2.x - v1.x * v2.z;
    double rz = v1.x * v2.y - v1.y * v2.x;
    RTVector value(rx, ry, rz);
//    std::cout << "In RTCross" << std::endl;
//    std::cout << rx << std::endl;
//    std::cout << ry << std::endl;
//    std::cout << rz << std::endl;
    return value;
}

bool RTEqual(RTVector v1, RTVector v2) {
    if (v1.x == v2.x) {
        if (v1.y == v2.y) {
            if (v1.z == v2.z) {
                return true;
            }
        }
    }
    
    return false;
}

double RTDistance(RTVector v1, RTVector v2) {
    return sqrt(pow((v1.x - v2.x), 2) + pow((v1.y - v2.y), 2) + pow((v1.z - v2.z), 2));
}

double RTLength(RTVector v) {
    return sqrt(pow(v.x, 2) + pow(v.y, 2) + pow(v.z, 2));
}