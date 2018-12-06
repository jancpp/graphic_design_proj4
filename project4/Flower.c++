// Flower.c++

#include "Flower.h"

Flower::Flower(ShaderIF *sIF, PhongMaterial &matlIn,
           cryph::AffPoint corner, cryph::AffVector u,
           double flowerHeigth,
           double topRadius, double stemRadius)
    : SceneElement(sIF), matl(matlIn)
{
    defineInitialGeometry(corner, u,
                          flowerHeigth,
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

Flower::~Flower()
{
    for (int i = 0; i < NUM_PIECES; i++)
    {
        if (pieces[i] != nullptr)
            delete pieces[i];
        if (piecesR[i] != nullptr)
            delete piecesR[i];
    }
}

void Flower::defineInitialGeometry(cryph::AffPoint corner, cryph::AffVector u,
                                 double flowerHeigth,
                                 double topRadius, double stemRadius)
{
    cryph::AffVector uu(u[0], u[1], 0.0);
    uu.normalize();
    cryph::AffVector ww(0, 0, 1);
    cryph::AffVector vv = ww.cross(uu);

    cryph::AffPoint top = corner + ww * flowerHeigth + uu * 2.0;
    const cryph::AffVector sZero = cryph::AffVector::xu;
    pieces[0] = BasicShape::makeSphere(top, topRadius, 20, 20);

    
}

void Flower::getMCBoundingBox(double *xyzLimits) const
{
    for (int i = 0; i < 6; i++)
        xyzLimits[i] = xyz[i];
}

void Flower::prepareForFace(void *caller, int faceIndex)
{
}

void Flower::render()
{
    // 1. Save current and establish new current shader program
    GLint pgm;
    glGetIntegerv(GL_CURRENT_PROGRAM, &pgm);
    glUseProgram(shaderIF->getShaderPgmID());

    glUniform1i(shaderIF->ppuLoc("drawingOpaqueObjects"), 1);
    establishView();
    establishLightingEnvironment();
    renderFlower();

    glUseProgram(pgm);
}

void Flower::renderFlower()
{
    establishMaterial(matl);
    for (currentlyDrawingPiece = 0; currentlyDrawingPiece < NUM_PIECES; currentlyDrawingPiece++)
        if (piecesR[currentlyDrawingPiece] != nullptr)
            piecesR[currentlyDrawingPiece]->renderShape(nullptr, nullptr);
}
