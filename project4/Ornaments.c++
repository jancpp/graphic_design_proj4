// Ornaments.c++

#include "Ornaments.h"

Ornaments::Ornaments(ShaderIF *sIF, PhongMaterial &matlIn,
                     cryph::AffPoint corner, cryph::AffVector u,
                     double heigth,
                     double topRadius, double midRadius, double baseRadius, int points)
    : SceneElement(sIF), matl(matlIn), pt(points)
{
    defineInitialGeometry(corner, u,
                          heigth,
                          topRadius, midRadius, baseRadius);
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

Ornaments::~Ornaments()
{
    for (int i = 0; i < NUM_PIECES; i++)
    {
        if (pieces[i] != nullptr)
            delete pieces[i];
        if (piecesR[i] != nullptr)
            delete piecesR[i];
    }
}

void Ornaments::defineInitialGeometry(cryph::AffPoint corner, cryph::AffVector u,
                                 double heigth,
                                 double topRadius, double midRadius, double baseRadius)
{
    cryph::AffVector uu(u[0], u[1], 0.0);
    uu.normalize();
    cryph::AffVector ww(0, 0, 1);
    cryph::AffVector vv = ww.cross(uu);

    cryph::AffPoint bottom = corner;
    cryph::AffPoint top = bottom + ww * heigth;
    const cryph::AffVector sZero = cryph::AffVector::xu;
    pieces[0] = BasicShape::makeBoundedCone(bottom + ww * heigth * 4/6, bottom + ww * heigth,
                                            baseRadius*0.6, 2.0, pt, pt, BasicShape::CAP_AT_NEITHER);

    pieces[1] = BasicShape::makeBoundedCone(bottom + ww * heigth * 3 / 6, bottom + ww * heigth * 5 / 6,
                                            baseRadius * 0.7, 2.0, pt + 1, pt + 1, BasicShape::CAP_AT_NEITHER);

    pieces[2] = BasicShape::makeBoundedCone(bottom + ww * heigth * 2 / 6, bottom + ww * heigth * 4 / 6,
                                            baseRadius * 0.8, 2.0, pt + 2, pt + 2, BasicShape::CAP_AT_NEITHER);

    pieces[3] = BasicShape::makeBoundedCone(bottom + ww * heigth * 1 / 6, bottom + ww * heigth * 3 / 6,
                                            baseRadius * 0.9, 2.0, pt + 3, pt + 3, BasicShape::CAP_AT_NEITHER);

    pieces[4] = BasicShape::makeBoundedCone(bottom, bottom + ww * heigth * 2 / 6,
                                            baseRadius, 2.0, pt + 4, pt + 4, BasicShape::CAP_AT_NEITHER);
}

void Ornaments::getMCBoundingBox(double *xyzLimits) const
{
    for (int i = 0; i < 6; i++)
        xyzLimits[i] = xyz[i];
}

void Ornaments::prepareForFace(void *caller, int faceIndex)
{
}

void Ornaments::render()
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

    renderOrnaments();

    // 6. Reestablish previous shader program
    glUseProgram(pgm);
}

void Ornaments::renderOrnaments()
{
    establishMaterial(matl);
    for (currentlyDrawingPiece = 0; currentlyDrawingPiece < NUM_PIECES; currentlyDrawingPiece++)
        if (piecesR[currentlyDrawingPiece] != nullptr)
            piecesR[currentlyDrawingPiece]->renderShape(prepareForFace, this);

    establishTexture();
}
