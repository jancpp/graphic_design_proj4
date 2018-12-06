// Vase.c++

#include "Vase.h"

Vase::Vase(ShaderIF *sIF, PhongMaterial &vaseMatlIn,
           cryph::AffPoint corner, cryph::AffVector u,
           double VaseHeigth, double topRadius, double baseRadius)
    : SceneElement(sIF), vaseMatl(vaseMatlIn)
{
    defineInitialGeometry(corner, u,
                          VaseHeigth,
                          topRadius,
                          baseRadius);
    xyz[0] = 1.0;
    xyz[1] = 0.0;
    for (int i = 0; i < NUM_PIECES; i++)
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

Vase::~Vase()
{
    for (int i = 0; i < NUM_PIECES; i++)
    {
        if (pieces[i] != nullptr)
            delete pieces[i];
        if (piecesR[i] != nullptr)
            delete piecesR[i];
    }
}

void Vase::defineInitialGeometry(cryph::AffPoint corner, cryph::AffVector u,
                double VaseHeigth, double topRadius, double baseRadius)
{
    cryph::AffVector uu(u[0], u[1], 0.0);
    uu.normalize();
    cryph::AffVector ww(0, 0, 1);
    cryph::AffVector vv = ww.cross(uu);

    cryph::AffPoint bottom = corner;
    cryph::AffPoint top = bottom +
                          ww * VaseHeigth;

     // top part / cone
    const cryph::AffVector sZero = cryph::AffVector::xu;
    pieces[0] = BasicShape::makeBoundedCone(bottom, bottom + ww * VaseHeigth,
                                            baseRadius, topRadius, 20, 20, BasicShape::CAP_AT_BOTTOM);
}

void Vase::getMCBoundingBox(double *xyzLimits) const
{
    for (int i = 0; i < 6; i++)
        xyzLimits[i] = xyz[i];
}

void Vase::prepareForFace(void *caller, int faceIndex)
{
}

void Vase::render()
{
    // 1. Save current and establish new current shader program
    GLint pgm;
    glGetIntegerv(GL_CURRENT_PROGRAM, &pgm);
    glUseProgram(shaderIF->getShaderPgmID());
    
    establishView();
    establishLightingEnvironment();
    renderVase();
    glUseProgram(pgm);
}

void Vase::renderVase()
{
    establishMaterial(vaseMatl);
    if (piecesR[0] != nullptr)
        piecesR[0]->renderShape(nullptr, nullptr);
}
