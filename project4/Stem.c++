// Stem.c++

#include "Stem.h"

Stem::Stem(ShaderIF *sIF, PhongMaterial &matlIn,
           cryph::AffPoint corner, cryph::AffVector u,
           double stemHeigth,
           double topRadius, double stemRadius)
: SceneElement(sIF), matl(matlIn)
{
    defineInitialGeometry(corner, u,
                          stemHeigth,
                          topRadius, stemRadius);
    xyz[0] = 1.0;
    xyz[1] = 0.0;
    for (int i = 0; i < NUM_PIECES; i++)
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

Stem::~Stem()
{
    for (int i = 0; i < NUM_PIECES; i++)
    {
        if (pieces[i] != nullptr)
            delete pieces[i];
        if (piecesR[i] != nullptr)
            delete piecesR[i];
    }
}

void Stem::defineInitialGeometry(cryph::AffPoint corner, cryph::AffVector u,
                                 double stemHeigth,
                                 double topRadius, double stemRadius)
{
    cryph::AffVector uu(u[0], u[1], 0.0);
    uu.normalize();
    cryph::AffVector ww(0, 0, 1);
    cryph::AffVector vv = ww.cross(uu);
    
    cryph::AffPoint bottom = corner;
    cryph::AffPoint top = bottom + ww * stemHeigth + uu * 2.0;
    const cryph::AffVector sZero = cryph::AffVector::xu;
    pieces[0] = BasicShape::makeBoundedCylinder(bottom, top,
                                                stemRadius, 20, 2, BasicShape::CAP_AT_BOTH);
    
}

void Stem::getMCBoundingBox(double *xyzLimits) const
{
    for (int i = 0; i < 6; i++)
        xyzLimits[i] = xyz[i];
}

void Stem::prepareForFace(void *caller, int faceIndex)
{
}

void Stem::render()
{
    GLint pgm;
    glGetIntegerv(GL_CURRENT_PROGRAM, &pgm);
    glUseProgram(shaderIF->getShaderPgmID());
    
    glUniform1i(shaderIF->ppuLoc("drawingOpaqueObjects"), 1);
    establishView();
    establishLightingEnvironment();
    renderStem();
    
    glUseProgram(pgm);
}

void Stem::renderStem()
{
    establishMaterial(matl);
    for (currentlyDrawingPiece = 0; currentlyDrawingPiece < NUM_PIECES; currentlyDrawingPiece++)
        if (piecesR[currentlyDrawingPiece] != nullptr)
            piecesR[currentlyDrawingPiece]->renderShape(nullptr, nullptr);
}
