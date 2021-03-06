// PhongMaterial.h - Utility struct with Phong model properties

#ifndef PHONGMATERIAL_H
#define PHONGMATERIAL_H

#include <stdlib.h>

// Feel free to add any other constructors that will help you.
// Always make sure "shininess" >= 1.

struct PhongMaterial
{
    float ka[3], kd[3], ks[3];
    float shininess, alpha;
    
    // Following also serves as the no-parameter constructor
    PhongMaterial(	float r=1.0, float g=1.0, float b=1.0,
                  float af=0.2, float df=0.2, float sf=0.8,
                  float m=25.0, float a=1.0);
    
    PhongMaterial(	float kaR, float kaG, float kaB,
                  float kdR, float kdG, float kdB,
                  float ksR, float ksG, float ksB, 
                  float m, float a);
    
    void copyColors(const float* rgb, float af, float df, float sf);
};

#endif
