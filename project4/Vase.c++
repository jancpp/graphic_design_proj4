// Vase.c++

#include "Vase.h"

Vase::Vase(ShaderIF *sIF, PhongMaterial &topMatlIn, PhongMaterial &poleMatlIn, PhongMaterial &bulbMatlIn,
           cryph::AffPoint corner, cryph::AffVector u,
           double VaseHeigth,
           double topRadius, double midRadius,
           double poleHeight, double poleRadius,
           double baseRadius)
    : SceneElement(sIF), topMatl(topMatlIn), poleMatl(poleMatlIn), bulbMatl(bulbMatlIn)
{
    defineInitialGeometry(corner, u,
                          VaseHeigth,
                          topRadius, midRadius,
                          poleHeight, poleRadius,
                          baseRadius);
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

Vase::~Vase()
{
    for (int i = 0; i < 3; i++)
    {
        if (pieces[i] != nullptr)
            delete pieces[i];
        if (piecesR[i] != nullptr)
            delete piecesR[i];
    }
}

void Vase::defineInitialGeometry(cryph::AffPoint corner, cryph::AffVector u,
                                 double VaseHeigth,
                                 double topRadius, double midRadius,
                                 double poleHeight, double poleRadius,
                                 double baseRadius)
{
    cryph::AffVector uu(u[0], u[1], 0.0);
    uu.normalize();
    cryph::AffVector ww(0, 0, 1);
    cryph::AffVector vv = ww.cross(uu);

    double baseHeigth = 2 * poleRadius;
    cryph::AffPoint bottom = corner;
    cryph::AffPoint top = bottom +
                          ww * VaseHeigth;

    pieces[0] = BasicShape::makeBoundedCylinder(bottom, bottom + ww * baseHeigth,
                                                baseRadius, 20, 2, BasicShape::CAP_AT_BOTH);

    pieces[1] = BasicShape::makeBoundedCylinder(bottom, bottom + ww * poleHeight,
                                                poleRadius, 20, 2, BasicShape::CAP_AT_BOTH);
    // top part / cone
    const cryph::AffVector sZero = cryph::AffVector::xu;
    pieces[2] = BasicShape::makeBoundedCone(bottom + ww * VaseHeigth / 2.5, bottom + ww * VaseHeigth,
                                            midRadius, topRadius, 20, 20, BasicShape::CAP_AT_NEITHER);
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

    // 2. Establish "mc_ec" and "ec_lds" matrices
    establishView();

    // 3. Establish Lighting environment
    //    complete the implementation of SceneElement::establishLightingEnvironment
    //    and then call it here.
    establishLightingEnvironment();

    renderVase();

    // 6. Reestablish previous shader program
    glUseProgram(pgm);
}

void Vase::renderVase()
{
    establishMaterial(poleMatl);
    if (piecesR[0] != nullptr)
        piecesR[0]->renderShape(nullptr, nullptr);
    if (piecesR[1] != nullptr)
        piecesR[1]->renderShape(nullptr, nullptr);
    establishMaterial(topMatl);
    if (piecesR[2] != nullptr)
        piecesR[2]->renderShape(nullptr, nullptr);

    // establishTexture();
}
