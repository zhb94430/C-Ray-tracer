//
//  RTVector.h
//  
//
//  Created by Peter Zhang on 14/10/18.
//
//

#include <stdio.h>


class RTVector {
public:
    double x, y, z;
    
public:
    RTVector(double xi, double yi, double zi);
    RTVector();
    RTVector RTNormalize();
};

double RTDot(RTVector v1, RTVector v2);
RTVector RTMultiC(RTVector v, double c);
RTVector RTNormalize(RTVector v, double m);
RTVector RTAdd(RTVector v1, RTVector v2);
RTVector RTMinus(RTVector v1, RTVector v2);
RTVector RTCross(RTVector v1, RTVector v2);
bool RTEqual(RTVector v1, RTVector v2);
double RTDistance(RTVector v1, RTVector v2);
double RTLength(RTVector v);