//
//  sceneClasses.cpp
//  
//
//  Created by Peter Zhang on 14-10-1.
//
//
#include "sceneClasses.h"
#include <math.h>
#include <stdlib.h>
#include <iostream>

using namespace std;


RTRay::RTRay(double xi, double yi, double zi, RTVector directioni) {
    x = xi;
    y = yi;
    z = zi;
    RTVector origini(x, y, z);
    direction = directioni.RTNormalize();
    origin = origini;
}

RTWindow::RTWindow(double li, double ri, double bi, double ti) {
    l = li;
    r = ri;
    b = bi;
    t = ti;
}

RTWindow::RTWindow(double w, double h) {
    l = -(w/2);
    r = h/2;
    b = -(w/2);
    t = h/2;
}

RTWindow::RTWindow() {
    l = 0;
    r = 0;
    b = 0;
    t = 0;
}

film_resolution::film_resolution(int w, int h) {
    width = w;
    height = h;
}

ambient_light::ambient_light(double ri, double gi, double bi){
    r = ri;
    g = gi;
    b = bi;
}

background::background(double ri, double gi, double bi) {
    r = ri;
    g = gi;
    b = bi;
}

background::background() {
    r = 0;
    g = 0;
    b = 0;
}

camera::camera(double pxi, double pyi, double pzi,
               double dxi, double dyi, double dzi,
               double uxi, double uyi, double uzi,
               double hai, double ratio) {
    px = pxi;
    py = pyi;
    pz = pzi;
    RTVector inputViewDirection(dxi, dyi, dzi);
    viewDirection = inputViewDirection;
    dx = dxi;
    dy = dyi;
    dz = dzi;
    ux = uxi;
    uy = uyi;
    uz = uzi;
    upDirection = RTVector(ux, uy, uz);
    ha = hai;
    haRadian = ha * M_PI / 180;
    
    //Set the focalLength
//    focalLength = dz;
    
    focalLength = sqrt(pow((dx - px), 2) + pow((dy - py), 2) + pow((dz - pz), 2));
    
    cout << "focalLength after computation is " << focalLength << endl;
    cout << "dz is " << dz << endl;
    
    //Calculate the bottom and top based on focalLength;
    double t = focalLength * tan(haRadian);
    double b = -t;
    
    //Calculate the width from the ratio
    double r = t*ratio;
    double l = -r;
    
    //Make the window
    RTWindow inputWindow(l, r, b, t);
    window = inputWindow;
    
    //Establish the coord sys
    W = RTVector(dx, dy, dz);
    double magW = sqrt(pow(dx, 2) + pow(dy, 2) + pow(dz, 2));
    if (magW != 0) {
        W = RTNormalize(W, magW);
    }
    W = RTMultiC(W, -1);
    
    U = RTCross(RTVector(ux, uy, uz), W);
    
    double magU = sqrt(pow(U.x, 2) + pow(U.y, 2) + pow(U.z, 2));
    if (magU != 0) {
        U = RTNormalize(U, magU);
    }
    V = RTCross(W, U);
    
    cout << "The lrbt of window" << '\n';
    cout << l << endl;
    cout << r << endl;
    cout << b << endl;
    cout << t << endl;
}

RTRay camera::getRay(double i, double j, film_resolution film) {
    double u = -(window.l + (window.r - window.l)*(i + 0.5)/film.width);
    double v = -(window.b + (window.t - window.b)*(j + 0.5)/film.height);
    double d = focalLength;
    
    //Ray direction computation
    RTVector rayDirection = RTAdd(RTAdd(RTMultiC(W, -d), RTMultiC(U, u)), RTMultiC(V, v));
    
    RTRay returnRay(px, py, pz, rayDirection);
    return returnRay;
}

directional_light::directional_light(double ri, double gi, double bi, double xi, double yi, double zi) {
    r = ri;
    g = gi;
    b = bi;
    x = xi;
    y = yi;
    z = zi;
}

material::material(double ari, double agi, double abi,
                   double dri, double dgi, double dbi,
                   double sri, double sgi, double sbi,
                   double nsi,
                   double tri, double tgi, double tbi,
                   double iori) {
    ar = ari;
    ag = agi;
    ab = abi;
    dr = dri;
    dg = dgi;
    db = dbi;
    sr = sri;
    sg = sgi;
    sb = sbi;
    ns = nsi;
    tr = tri;
    tg = tgi;
    tb = tbi;
    ior = iori;
}

material::material() {
    ar = 0;
    ag = 0;
    ab = 0;
    dr = 1;
    dg = 1;
    db = 1;
    sr = 0;
    sg = 0;
    sb = 0;
    ns = 5;
    tr = 0;
    tg = 0;
    tb = 0;
    ior = 1;
}

point_light::point_light(double ri, double gi, double bi, double xi, double yi, double zi) {
    r = ri;
    g = gi;
    b = bi;
    x = xi;
    y = yi;
    z = zi;
}

point_light::point_light() {
    r = 0;
    g = 0;
    b = 0;
    x = 0;
    y = 0;
    z = 0;
}

sphere::sphere(double xi, double yi, double zi, double ri) {
    x = xi;
    y = yi;
    z = zi;
    r = ri;
    RTVector centeri(x, y, z);
    center = centeri;
}

spot_light::spot_light(double ri, double gi, double bi,
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

//Assignment 3
vertex::vertex() {
    x = 0;
    y = 0;
    z = 0;
}

vertex::vertex(double _x, double _y, double _z) {
    x = _x;
    y = _y;
    z = _z;
}

normal::normal() {
    x = 0;
    y = 0;
    z = 0;
}

normal::normal(double _x, double _y, double _z) {
    x = _x;
    y = _y;
    z = _z;
}

triangle::triangle(vertex _v1, vertex _v2, vertex _v3) {
    v1 = _v1;
    v2 = _v2;
    v3 = _v3;
    
//    double nx, ny, nz;
//    nx = (v2.y - v1.y)*(v3.z - v1.z) - (v2.z - v1.z)*(v3.y - v1.y);
//    ny = (v2.z - v1.z)*(v3.x - v1.x) - (v2.x - v1.x)*(v3.z - v1.z);
//    nz = (v2.x - v1.x)*(v3.y - v1.y) - (v2.y - v1.y)*(v3.x - v1.x);
//    
//    normal = RTVector(nx, ny, nz);
    
    normal = RTCross(RTMinus(RTVector(v3.x, v3.y, v3.z), RTVector(v1.x, v1.y, v1.z)), RTMinus(RTVector(v2.x, v2.y, v2.z), RTVector(v1.x, v1.y, v1.z)));
    
    normal = normal.RTNormalize();
    
    cout << "Normal for triangle" << endl;
    cout << normal.x << endl;
    cout << normal.y << endl;
    cout << normal.z << endl;
}

normalTriangle::normalTriangle(vertex _v1, vertex _v2, vertex _v3, normal _n1, normal _n2, normal _n3) {
    v1 = _v1;
    v2 = _v2;
    v3 = _v3;
    n1 = _n1;
    n2 = _n2;
    n3 = _n3;
}

colorValue::colorValue() {
    r = 0;
    g = 0;
    b = 0;
}

colorValue::colorValue(int _r, int _g, int _b) {
    r = _r;
    g = _g;
    b = _b;
}

colorFactor::colorFactor() {
    fR = 0;
    fG = 0;
    fB = 0;
}

colorFactor::colorFactor(double _fR, double _fG, double _fB) {
    fR = _fR;
    fG = _fG;
    fB = _fB;
}

void colorFactor::addFactor(colorFactor inputFactor) {
    fR += inputFactor.fR;
    fG += inputFactor.fG;
    fB += inputFactor.fB;
}

hitPoint::hitPoint(RTRay tempRay, double t, RTVector _normal, int _materialIndex) {
    materialIndex = _materialIndex;
    xIntersect = tempRay.x + t * tempRay.direction.x;
    yIntersect = tempRay.y + t * tempRay.direction.y;
    zIntersect = tempRay.z + t * tempRay.direction.z;
    incoming = RTVector(xIntersect, yIntersect, zIntersect);
    normal = _normal;
    RTVector incomingN = incoming.RTNormalize();
    double angle = acos(RTDot(incomingN, normal));
//    cout << "The arc cosine is " << angle * 180.0 / M_PI << endl;
//    cout << "angle between two vectors" << endl;
//    cout << angle << endl;
    if ((angle >= -(M_PI/2)) && (angle <= (M_PI/2))) {
        normal = RTMultiC(normal, -1);
    }
}

hitPoint::hitPoint(RTRay tempRay, double t, sphere currentItem, int _materialIndex) {
    materialIndex = _materialIndex;
    xIntersect = tempRay.x + t * tempRay.direction.x;
    yIntersect = tempRay.y + t * tempRay.direction.y;
    zIntersect = tempRay.z + t * tempRay.direction.z;
    incoming = RTVector(xIntersect, yIntersect, zIntersect);
    normal = RTVector((xIntersect - currentItem.x)/currentItem.r, (yIntersect - currentItem.y)/currentItem.r, (zIntersect - currentItem.z)/currentItem.r);
}
