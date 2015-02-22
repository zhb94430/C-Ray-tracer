//
//  point_light.h
//
//
//  Created by Peter Zhang on 14-9-30.
//
//
#include <stdio.h>

class point_light {
public:
    int r, g, b;
    double x, y, z;
    
public:
    point_light(int ri, int gi, int bi, double xi, double yi, double zi);
};