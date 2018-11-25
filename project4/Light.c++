// Light.c++

#include "Light.h"

Light::Light(ShaderIF *sIF, PhongMaterial &topMatlIn, PhongMaterial &bottomMatlIn,
             cryph::AffPoint center, cryph::AffVector u,
             double heigth,
             double radius)
: SceneElement(sIF), topMatl(topMatlIn), bottomMatl(bottomMatlIn)
{
    defineInitialGeometry(center, u, heigth, radius);
    xyz[0] = 1.0;
    xyz[1] = 0.0;
    for (int i = 0; i < 3; i++)
        if (pieces[i] == nullptr)
            piecesR[i] = nullptr;
        else
        {
            piecesR[i] = new BasicShapeRenderer(sIF, pieces[i]);
            // avvumulate bounding box
            if (xyz[0] > xyz[1]) // not yet initialized
                pieces[i]->getMCBoundingBox(xyz);
            else
            {
                double thisxyz[6];
                pieces[i]->getMCBoundingBox(thisxyz);
                for (int j = 0; j < 3; j++)
                {
                    if (thisxyz[2 * j] < xyz[2 * j])
                        xyz[2 * j] = thisxyz[2 * j];
                    if (thisxyz[2 * j + 1] > xyz[2 * j + 1])
                        xyz[2 * j + 1] = thisxyz[2 * j + 1];
                }
            }
        }
}

Light::~Light()
{
    for (int i = 0; i < 3; i++)
    {
        if (pieces[i] != nullptr)
            delete pieces[i];
        if (piecesR[i] != nullptr)
            delete piecesR[i];
    }
}

void Light::defineInitialGeometry(cryph::AffPoint center, cryph::AffVector u,
                                  double heigth, double radius)
{
    cryph::AffVector uu(u[0], u[1], 0.0);
    uu.normalize();
    cryph::AffVector ww(0, 0, 1);
    cryph::AffVector vv = ww.cross(uu);
    
    cryph::AffPoint top = center - 0.01 * vv;
    cryph::AffPoint bottom = top - ww * heigth;
    // rim
    pieces[0] = BasicShape::makeBoundedCylinder(top, top - ww * heigth / 10,
                                                radius * 1.05, 20, 2, BasicShape::CAP_AT_BOTH);
    // light
    pieces[1] = BasicShape::makeBoundedCylinder(top, bottom, radius, 20, 2, BasicShape::CAP_AT_BOTH);
    // screw
    pieces[2] = BasicShape::makeBoundedCylinder(bottom, bottom - ww * heigth / 10,
                                                radius/10, 20, 2, BasicShape::CAP_AT_BOTH);
}

void Light::getMCBoundingBox(double *xyzLimits) const
{
    for (int i = 0; i < 6; i++)
        xyzLimits[i] = xyz[i];
}

void Light::prepareForFace(void *caller, int faceIndex)
{
}

void Light::render()
{
    // 1. Save current and establish new current shader program
    GLint pgm;
    glGetIntegerv(GL_CURRENT_PROGRAM, &pgm);
    glUseProgram(shaderIF->getShaderPgmID());
    
    // 2. Establish "mc_ec" and "ec_lds" matrices
    establishView();
    
    // 3. Establish Lighting environment
    //    complete the implementation of SceneElement::establishLightingEnvironment
    //    and then call it here.
    establishLightingEnvironment();
    
    renderLight();
    
    // 6. Reestablish previous shader program
    glUseProgram(pgm);
}

void Light::renderLight()
{
    establishMaterial(topMatl);
    if (piecesR[0] != nullptr)
        piecesR[0]->renderShape(prepareForFace, this);
    establishMaterial(bottomMatl);
    if (piecesR[1] != nullptr)
        piecesR[1]->renderShape(prepareForFace, this);
    establishMaterial(topMatl);
    if (piecesR[2] != nullptr)
        piecesR[2]->renderShape(prepareForFace, this);
    
    establishTexture();
}
