// couch.c++

#include "Couch.h"

Couch::Couch(ShaderIF *sIF, PhongMaterial &couchMatlIn,
             cryph::AffPoint corner, cryph::AffVector u,
             double legHeight, double legRadius, double armRadius,
             double couchWidth, double couchHeight, double couchDepth)
: SceneElement(sIF), couchMatl(couchMatlIn)
{
    defineInitialGeometry(corner, u,
                          legHeight, legRadius, armRadius,
                          couchWidth, couchHeight, couchDepth);
    xyz[0] = 1.0;
    xyz[1] = 0.0;
    for (int i = 0; i < 10; i++)
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

Couch::~Couch()
{
    for (int i = 0; i < 10; i++)
    {
        if (pieces[i] != nullptr)
            delete pieces[i];
        if (piecesR[i] != nullptr)
            delete piecesR[i];
    }
}

void Couch::defineInitialGeometry(cryph::AffPoint corner, cryph::AffVector u,
                                  double legHeight, double legRadius, double armRadius,
                                  double couchWidth, double couchHeight, double couchDepth)
{
    // we assume the Couch is parallel to xy-plane, hence:
    cryph::AffVector uu(u[0], u[1], 0.0);
    cryph::AffVector ww(0, 0, 1);
    uu.normalize();
    cryph::AffVector vv = ww.cross(uu);
    corner = corner + ww * 0.01 + vv * 0.01; // to avoid bleading in to wall and floor
    cryph::AffPoint frontCornerOfBase = corner +
    uu * armRadius +
    ww * legHeight;
    double couchBaseWidth = couchWidth - 2 * armRadius;
    // sit on block
    pieces[0] = BasicShape::makeBlock(frontCornerOfBase,
                                      uu, couchBaseWidth,
                                      ww, couchHeight / 2,
                                      vv, couchDepth);
    
    cryph::AffPoint front = corner + uu * armRadius - vv*couchDepth + ww * (couchHeight / 2 + armRadius) ;
    cryph::AffPoint back = front + vv * couchDepth;
    // left arm
    pieces[1] = BasicShape::makeBoundedCylinder(front, back,
                                                armRadius, 20, 2, BasicShape::CAP_AT_BOTH);
    
    front += couchBaseWidth *  uu;
    back = front + couchDepth * vv;
    //right arm
    pieces[2] = BasicShape::makeBoundedCylinder(front, back,
                                                armRadius, 20, 2, BasicShape::CAP_AT_BOTH);
    front += (couchHeight * armRadius) * uu;
    back = front + couchHeight * vv;
    
    cryph::AffPoint cornerBackBlock = corner + uu * armRadius + ww * couchHeight / 2;
    // back block
    pieces[3] = BasicShape::makeBlock(cornerBackBlock,
                                      uu, couchBaseWidth,
                                      ww, couchHeight / 2,
                                      vv, couchDepth / 4);
    
    cryph::AffPoint bottom = corner + vv * (legRadius - couchDepth) + uu * (legRadius + armRadius);
    cryph::AffPoint top = bottom + ww * legHeight;
    // 6 legs
    for (int i = 0; i < 4; i += 3)
    {
        cryph::AffPoint currBottom = bottom;
        cryph::AffPoint currTop = top;
        pieces[i + 4] = BasicShape::makeBoundedCylinder(currTop, currBottom,
                                                        legRadius, 20, 2, BasicShape::CAP_AT_BOTH);
        currBottom = currBottom + uu * (couchBaseWidth / 2 - legRadius);
        currTop = currBottom + ww * legHeight;
        pieces[i + 5] = BasicShape::makeBoundedCylinder(currTop, currBottom,
                                                        legRadius, 20, 2, BasicShape::CAP_AT_BOTH);
        
        currBottom = currBottom + uu * (couchBaseWidth / 2 - legRadius);
        currTop = currBottom + ww * legHeight;
        pieces[i + 6] = BasicShape::makeBoundedCylinder(currTop, currBottom,
                                                        legRadius, 20, 2, BasicShape::CAP_AT_BOTH);
        
        bottom += vv * couchDepth - 2 * vv * legRadius;
        top = bottom + ww * legHeight;
    }
}

void Couch::getMCBoundingBox(double *xyzLimits) const
{
    for (int i = 0; i < 6; i++)
        xyzLimits[i] = xyz[i];
}

void Couch::prepareForFace(void *caller, int faceIndex)
{
}

void Couch::render()
{
    GLint pgm;
    glGetIntegerv(GL_CURRENT_PROGRAM, &pgm);
    glUseProgram(shaderIF->getShaderPgmID());
    establishView();
    
    establishLightingEnvironment();
    
    renderCouch();
    glUseProgram(pgm);
}

void Couch::renderCouch()
{
    establishMaterial(couchMatl);
    for (currentlyDrawingPiece = 0; currentlyDrawingPiece < 10; currentlyDrawingPiece++)
        if (piecesR[currentlyDrawingPiece] != nullptr)
            piecesR[currentlyDrawingPiece]->renderShape(prepareForFace, this);
}
