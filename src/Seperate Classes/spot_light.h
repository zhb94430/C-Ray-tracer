//
//  spot_light.h
//  
//
//  Created by Peter Zhang on 14-9-30.
//
//
#include <stdio.h>

class spot_light {
public:
    int r, g, b;
    double px, py, pz;
    double dx, dy, dz;
    double angle1, angle2;
    
public:
    spot_light(int ri, int gi, int bi,
               double pxi, double pyi, double pzi,
               double dxi, double dyi, double dzi,
               double a1, double a2);
};