//
//  camera.cpp
//  
//
//  Created by Peter Zhang on 14-9-30.
//
//

#include "camera.h"

class camera {
    double px, py, pz;
    double dx, dy, dz;
    double ux, uy, uz;
    double ha;
    
    camera::camera(double pxi, double pyi, double pzi,
                   double dxi, double dyi, double dzi,
                   double uxi, double uyi, double uzi,
                   double hai) {
        px = pxi;
        py = pyi;
        pz = pzi;
        dx = dxi;
        dy = dyi;
        dz = dzi;
        ha = hai;
    }
};