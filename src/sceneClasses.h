//
//  sceneClasses.h
//  
//
//  Created by Peter Zhang on 14-10-1.
//
//
#include <stdio.h>
#include "RTVector.h"

class RTRay {
public:
    RTVector direction;
    RTVector origin;
    double x, y, z;
    
public:
    RTRay(double xi, double yi, double zi, RTVector directioni);
};

class RTWindow {
public:
    double l, r, b, t;
    
public:
    RTWindow(double li, double ri, double bi, double ti);
    RTWindow(double w, double h);
    RTWindow();
};

class film_resolution {
public:
    int width, height;
    
public:
    film_resolution(int w, int h);
};

class ambient_light {
public:
    double r,g,b;
    
public:
    ambient_light(double ri, double gi, double bi);
};

class background {
public:
    double r,g,b;
    
public:
    background(double ri, double gi, double bi);
    background();
};

class camera {
    
public:
    double px, py, pz;
    double dx, dy, dz;
    double ux, uy, uz;
    double ha, haRadian;
    double focalLength;
    RTVector viewDirection, upDirection, U, V, W;
    RTWindow window;
    
public:
    //Basic camera constructor
    camera(double pxi, double pyi, double pzi,
           double dxi, double dyi, double dzi,
           double uxi, double uyi, double uzi,
           double hai, double ratio);
    RTRay getRay(double i, double j, film_resolution film);
};

class directional_light {
public:
    double r, g, b;
    double x, y, z;
    
public:
    directional_light(double ri, double gi, double bi, double xi, double yi, double zi);
};

class material {
public:
    double ar, ag, ab;
    double dr, dg, db;
    double sr, sg, sb;
    double ns;
    double tr, tg, tb;
    double ior;
    
public:
    material(double ari, double agi, double abi,
             double dri, double dgi, double dbi,
             double sri, double sgi, double sbi,
             double nsi,
             double tri, double tgi, double tbi,
             double iori);
    material();
};

class point_light {
public:
    double r, g, b;
    double x, y, z;
    
public:
    point_light(double ri, double gi, double bi, double xi, double yi, double zi);
    point_light();
};

class sphere {
public:
    double x,y,z,r;
    RTVector center;
    
public:
    sphere(double xi, double yi, double zi, double ri);
};

class spot_light {
public:
    double r, g, b;
    double px, py, pz;
    double dx, dy, dz;
    double angle1, angle2;
    
public:
    spot_light(double ri, double gi, double bi,
               double pxi, double pyi, double pzi,
               double dxi, double dyi, double dzi,
               double a1, double a2);
};

//Assignment 3
class vertex {
public:
    double x, y, z;
    
public:
    vertex();
    vertex(double _x, double _y, double _z);
};

class normal {
public:
    double x, y, z;
    
public:
    normal();
    normal(double _x, double _y, double _z);
};

class triangle {
public:
    vertex v1, v2, v3;
    RTVector normal;
    
public:
    triangle(vertex _v1, vertex _v2, vertex _v3);
};

class normalTriangle {
public:
    vertex v1, v2, v3;
    normal n1, n2, n3;
    
public:
    normalTriangle(vertex _v1, vertex _v2, vertex _v3, normal _n1, normal _n2, normal _n3);
};

class colorValue {
public:
    int r, g, b;
    
public:
    colorValue();
    colorValue(int _r, int _g, int _b);
};

class colorFactor {
public:
    double fR, fG, fB;
    
public:
    colorFactor();
    colorFactor(double _fR, double _fG, double _fB);
    void addFactor(colorFactor inputFactor);
};

class hitPoint {
public:
    RTVector incoming;
    RTVector normal;
    double xIntersect, yIntersect, zIntersect;
    int materialIndex;
public:
    //For triangles
    hitPoint(RTRay tempRay, double t, RTVector _normal, int _materialIndex);
    //For spheres
    hitPoint(RTRay tempRay, double t, sphere currentItem, int _materialIndex);
};