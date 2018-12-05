// Tree.c++

#include "Tree.h"

Tree::Tree(ShaderIF *sIF, PhongMaterial &matlIn,
           cryph::AffPoint corner, cryph::AffVector u,
           double treeHeigth,
           double topRadius, double midRadius, double baseRadius)
    : SceneElement(sIF), matl(matlIn)
{
    defineInitialGeometry(corner, u,
                          treeHeigth,
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

Tree::~Tree()
{
    for (int i = 0; i < NUM_PIECES; i++)
    {
        if (pieces[i] != nullptr)
            delete pieces[i];
        if (piecesR[i] != nullptr)
            delete piecesR[i];
    }
}

void Tree::defineInitialGeometry(cryph::AffPoint corner, cryph::AffVector u,
                                 double treeHeigth,
                                 double topRadius, double midRadius, double baseRadius)
{
    cryph::AffVector uu(u[0], u[1], 0.0);
    uu.normalize();
    cryph::AffVector ww(0, 0, 1);
    cryph::AffVector vv = ww.cross(uu);

    cryph::AffPoint bottom = corner;
    cryph::AffPoint top = bottom + ww * treeHeigth;
    const cryph::AffVector sZero = cryph::AffVector::xu;
    pieces[0] = BasicShape::makeBoundedCone(bottom + ww * treeHeigth * 4/6, bottom + ww * treeHeigth,
                                            baseRadius*0.6, 0.5, 20, 20, BasicShape::CAP_AT_NEITHER);

    pieces[1] = BasicShape::makeBoundedCone(bottom + ww * treeHeigth * 3 / 6, bottom + ww * treeHeigth * 5 / 6,
                                            baseRadius * 0.7, 0.5, 20, 20, BasicShape::CAP_AT_NEITHER);

    pieces[2] = BasicShape::makeBoundedCone(bottom + ww * treeHeigth * 2 / 6, bottom + ww * treeHeigth * 4 / 6,
                                            baseRadius * 0.8, 0.5, 20, 20, BasicShape::CAP_AT_NEITHER);

    pieces[3] = BasicShape::makeBoundedCone(bottom + ww * treeHeigth * 1 / 6, bottom + ww * treeHeigth * 3 / 6,
                                            baseRadius * 0.9, 0.5, 20, 20, BasicShape::CAP_AT_NEITHER);

    pieces[4] = BasicShape::makeBoundedCone(bottom, bottom + ww * treeHeigth * 2 / 6,
                                            baseRadius, 0.5, 20, 20, BasicShape::CAP_AT_NEITHER);
}

void Tree::getMCBoundingBox(double *xyzLimits) const
{
    for (int i = 0; i < 6; i++)
        xyzLimits[i] = xyz[i];
}

void Tree::prepareForFace(void *caller, int faceIndex)
{
}

void Tree::render()
{
    // 1. Save current and establish new current shader program
    GLint pgm;
    glGetIntegerv(GL_CURRENT_PROGRAM, &pgm);
    glUseProgram(shaderIF->getShaderPgmID());

    glUniform1i(shaderIF->ppuLoc("drawingOpaqueObjects"), 1);
    establishView();
    establishLightingEnvironment();
    renderTree();

    glUseProgram(pgm);
}

void Tree::renderTree()
{
    establishMaterial(matl);
    for (currentlyDrawingPiece = 0; currentlyDrawingPiece < NUM_PIECES; currentlyDrawingPiece++)
        if (piecesR[currentlyDrawingPiece] != nullptr)
            piecesR[currentlyDrawingPiece]->renderShape(nullptr, nullptr);

    // establishTexture();
}
