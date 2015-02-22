//
//  material.h
//  
//
//  Created by Peter Zhang on 14-9-30.
//
//
#include <stdio.h>

class material {
public:
    int ar, ag, ab;
    int dr, dg, db;
    int sr, sg, sb;
    double ns;
    int tr, tg, tb;
    double ior;
    
public:
    material(int ari, int agi, int abi,
             int dri, int dgi, int dbi,
             int sri, int sgi, int sbi,
             double nsi,
             int tri, int tgi, int tbi,
             double iori)
};