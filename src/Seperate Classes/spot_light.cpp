//
//  spot_light.cpp
//  
//
//  Created by Peter Zhang on 14-9-30.
//
//

#include "spot_light.h"

class spot_light {
    int r, g, b;
    double px, py, pz;
    double dx, dy, dz;
    double angle1, angle2;
    
    spot_light::spot_light(int ri, int gi, int bi,
               double pxi, double pyi, double pzi,
               double dxi, double dyi, double dzi,
               double a1, double a2){
        r = ri;
        g = gi;
        b = bi;
        px = pxi;
        py = pyi;
        pz = pzi;
        dx = dxi;
        dy = dyi;
        dz = dzi;
        angle1 = a1;
        angle2 = a2;
    }
};