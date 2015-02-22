//
//  camera.h
//  
//
//  Created by Peter Zhang on 14-9-30.
//
//
#include <stdio.h>

class camera {
    
public:
    double px, py, pz;
    double dx, dy, dz;
    double ux, uy, uz;
    double ha;
    
public:
    //Basic camera constructor
    camera(double pxi, double pyi, double pzi,
           double dxi, double dyi, double dzi,
           double uxi, double uyi, double uzi,
           double hai);
};