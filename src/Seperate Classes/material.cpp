//
//  material.cpp
//  
//
//  Created by Peter Zhang on 14-9-30.
//
//

#include "material.h"

class material {
    int ar, ag, ab;
    int dr, dg, db;
    int sr, sg, sb;
    double ns;
    int tr, tg, tb;
    double ior;
    
    material::material(int ari, int agi, int abi,
             int dri, int dgi, int dbi,
             int sri, int sgi, int sbi,
             double nsi,
             int tri, int tgi, int tbi,
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
};