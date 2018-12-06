// TVSet.c++

#include "TVSet.h"

TVSet::TVSet(ShaderIF *sIF, const char *tvTexImageSource, PhongMaterial &tvMatlIn,
             cryph::AffPoint corner, cryph::AffVector u,
             double tvWidth, double tvHeight, double tvDepth)
    : SceneElement(sIF), tvMatl(tvMatlIn)
// , wrapS(GL_CLAMP_TO_BORDER), wrapT(GL_CLAMP_TO_BORDER)
{
    defineInitialGeometry(corner, u,
                          tvWidth, tvHeight, tvDepth);
    texID = readTextureImage(tvTexImageSource);

    xyz[0] = 1.0;
    xyz[1] = 0.0;
    for (int i = 0; i < 3; i++)
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

TVSet::~TVSet()
{
    for (int i = 0; i < 3; i++)
    {
        if (pieces[i] != nullptr)
            delete pieces[i];
        if (piecesR[i] != nullptr)
            delete piecesR[i];
    }
}

void TVSet::defineInitialGeometry(cryph::AffPoint corner, cryph::AffVector u,
                                  double tvWidth, double tvHeight, double tvDepth)
{
    // we assume the tv is parallel to xy-plane, hence:
    cryph::AffVector uu(u[0], u[1], 0.0);
    cryph::AffVector ww(0, 0, 1);
    uu.normalize();
    cryph::AffVector vv = ww.cross(uu);
    corner = corner + ww * 0.01 + vv * 0.01; // to avoid bleading in to wall and floor
    cryph::AffPoint screenCorner = corner + ww * tvHeight/10;
    // tv block
    pieces[0] = BasicShape::makeBlock(screenCorner,
                                      uu, tvWidth,
                                      ww, 9 * tvHeight / 10,
                                      vv, tvDepth);
    // base - cylinder part
    double radius = tvDepth - 0.01;
    cryph::AffPoint bottom = corner + vv * (-tvDepth + radius) + uu * tvWidth/2;
    cryph::AffPoint top = bottom + ww* tvHeight/3;
    pieces[1] = BasicShape::makeBoundedCylinder(top, bottom, radius, 20, 2, BasicShape::CAP_AT_BOTH);
    cryph::AffPoint cornertvBase = bottom + vv * tvDepth * 5 / 2 - uu * tvWidth / 3/2;
    // tv base
    pieces[2] = BasicShape::makeBlock(cornertvBase,
                                      uu, tvWidth/3,
                                      ww, 0.1,
                                      vv, tvDepth * 5);
}

void TVSet::getMCBoundingBox(double *xyzLimits) const
{
    for (int i = 0; i < 6; i++)
        xyzLimits[i] = xyz[i];
}

void TVSet::prepareForFace(void *caller, int faceIndex)
{
    // check if texure is active
    // std::cout << "caller: " << caller << ", faceIndex: " << faceIndex << "\n";

    if (caller != nullptr) {
        TVSet *tv = reinterpret_cast<TVSet*>(caller);
        if (faceIndex == 5) {
            PhongMaterial screenMatl(1.0, 1.0, 1.0, 0.5, 0.1, 0.5, 2.0, 1.0);
            tv->establishMaterial(screenMatl); // ligth
            tv->piecesR[0]->setTexCoordsForBlock(faceIndex);
            tv->establishTexture();
        } else {
            glUniform1i(tv->shaderIF->ppuLoc("usingTextureMap"), 0);
        }
    }
}

void TVSet::render()
{
    // 1. Save current and establish new current shader program
    GLint pgm;
    glGetIntegerv(GL_CURRENT_PROGRAM, &pgm);
    glUseProgram(shaderIF->getShaderPgmID());

    glUniform1i(shaderIF->ppuLoc("drawingOpaqueObjects"), 0);
    establishView();
    establishLightingEnvironment();
    renderTVSet();
    glUseProgram(pgm);
}

void TVSet::renderTVSet()
{

    establishMaterial(tvMatl); // black

    if (piecesR[0] != nullptr) 
        piecesR[0]->renderShape(prepareForFace, this);
    glUniform1i(shaderIF->ppuLoc("usingTextureMap"), 0);
    establishMaterial(tvMatl); // black
    if (piecesR[1] != nullptr)
        piecesR[1]->renderShape(nullptr, nullptr);
    if (piecesR[2] != nullptr)
        piecesR[2]->renderShape(nullptr, nullptr);
}
