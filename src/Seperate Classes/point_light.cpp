//
//  point_light.cpp
//  
//
//  Created by Peter Zhang on 14-9-30.
//
//

#include "point_light.h"

class point_light {
    int r, g, b;
    double x, y, z;
    
    point_light::point_light(int ri, int gi, int bi, double xi, double yi, double zi) {
        r = ri;
        g = gi;
        b = bi;
        x = xi;
        y = yi;
        z = zi;
    }
};