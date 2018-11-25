// ECenter.c++

#include "ECenter.h"

ECenter::ECenter(ShaderIF *sIF, const PhongMaterial &ecmatlIn,
                 cryph::AffPoint corner, cryph::AffVector w,
                 double width, double height, double depth) : SceneElement(sIF), ecmatl(ecmatlIn)
{
    defineInitialGeometry(corner, w,
                          width, height, depth);
    xyz[0] = 1.0;
    xyz[1] = 0.0;
    for (int i=0 ; i<5 ; i++)
        if (pieces[i] == nullptr)
            piecesR[i] = nullptr;
        else
        {
            piecesR[i] = new BasicShapeRenderer(sIF, pieces[i]);
            // accumulate bounding box
            if (xyz[0] > xyz[1]) // not yet initialized
                pieces[i]->getMCBoundingBox(xyz);
            else
            {
                double thisxyz[6];
                pieces[i]->getMCBoundingBox(thisxyz);
                for (int j=0 ; j<3 ; j++)
                {
                    if (thisxyz[2*j] < xyz[2*j])
                        xyz[2*j] = thisxyz[2*j];
                    if (thisxyz[2*j+1] > xyz[2*j+1])
                        xyz[2*j+1] = thisxyz[2*j+1];
                }
            }
        }
}

ECenter::~ECenter()
{
    for (int i=0 ; i<5 ; i++)
    {
        if (pieces[i] != nullptr)
            delete pieces[i];
        if (piecesR[i] != nullptr)
            delete piecesR[i];
    }
}

void ECenter::defineInitialGeometry(cryph::AffPoint corner, cryph::AffVector u,
                                    double width, double height, double depth)
{
    cryph::AffVector uu(u[0], u[1], 0.0);
    cryph::AffVector ww(0, 0, 1);
    uu.normalize();
    cryph::AffVector vv = ww.cross(uu);
    
    // main block
    pieces[0] = BasicShape::makeBlock(corner,
                                      uu, width,
                                      ww, height,
                                      vv, depth);
    // bottom block
    double jump = 0.01 * width;
    pieces[1] = BasicShape::makeBlock(corner - uu * jump,
                                      uu, width + 2 * jump,
                                      ww, height / 7,
                                      vv, depth + jump);
    // top block
    pieces[2] = BasicShape::makeBlock(corner - uu * jump + ww * 9 * height / 10,
                                      uu, width + (2 * jump),
                                      ww, height / 10,
                                      vv, 2 * jump + depth);
    // left door
    pieces[3] = BasicShape::makeBlock(corner + uu * jump + ww * (jump + height / 7),
                                      uu, width/2 - (2 * jump),
                                      ww, height / 10 * 7,
                                      vv, 2 * jump + depth);
    // right door
    pieces[4] = BasicShape::makeBlock(corner + uu * (width / 2 + jump) + ww * (jump + height / 7),
                                      uu, width / 2 - (2 * jump),
                                      ww, height / 10 * 7,
                                      vv, 2 * jump + depth);
    
}

// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
void ECenter::getMCBoundingBox(double* xyzLimits) const
{
    for (int i=0 ; i<6 ; i++)
        xyzLimits[i] = xyz[i];
}

void ECenter::prepareForFace(void *caller, int faceIndex)
{
}

void ECenter::render()
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
    
    renderECenter();
    
    // 6. Reestablish previous shader program
    glUseProgram(pgm);
}

void ECenter::renderECenter()
{
    // 4. Establish material property parameters.
    //    complete the implementation of SceneElement::establishMaterial
    //    and then call it here.
    establishMaterial(ecmatl);
    for (currentlyDrawingPiece = 0; currentlyDrawingPiece < 5; currentlyDrawingPiece++)
        if (piecesR[currentlyDrawingPiece] != nullptr)
            piecesR[currentlyDrawingPiece]->renderShape(prepareForFace, this);
    
    
    // 5. Establish any other attributes and make one or more calls to
    //    glDrawArrays and/or glDrawElements
    //    If all or part of this model involves texture mapping (project 4
    //    only), complete the implementation of SceneElement::establishTexture
    //    and call it from here as needed immediately before any glDrawArrays
    //    and/or glDrawElements calls to which texture is to be applied.
    establishTexture();
}
