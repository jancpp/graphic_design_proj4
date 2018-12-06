// Rug.c++

#include "Rug.h"

Rug::Rug(ShaderIF *sIF, const char *rugTexImageSource, const PhongMaterial &rmatlIn,
           cryph::AffPoint corner, cryph::AffVector u,
           double rugWidth, double rugDepth,
           double width, double height, double depth) : SceneElement(sIF), rmatl(rmatlIn)
{
    defineInitialGeometry(corner, u,
                          rugWidth, rugDepth,
                          width, height, depth);
    texID = readTextureImage(rugTexImageSource);

    xyz[0] = 1.0;
    xyz[1] = 0.0;
    for (int i=0 ; i<1 ; i++)
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

Rug::~Rug()
{
    for (int i=0 ; i<1 ; i++)
    {
        if (pieces[i] != nullptr)
            delete pieces[i];
        if (piecesR[i] != nullptr)
            delete piecesR[i];
    }
}

void Rug::defineInitialGeometry(cryph::AffPoint corner, cryph::AffVector u,
                                 double rugWidth, double rugDepth,
                                 double width, double height, double depth)
{

    cryph::AffVector uu(u[0], u[1], 0.0);
    uu.normalize();
    cryph::AffVector vv(0, 1, 0);
    cryph::AffVector ww = vv.cross(uu);


    // rug
    pieces[0] = BasicShape::makeBlock(corner + uu * (width - rugWidth) / 2 +
                                      vv * (depth - rugDepth) / 2 -
                                      ww * 0.01,
                                      uu, rugWidth,
                                      vv, rugDepth,
                                      ww, 0.0);
}

// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
void Rug::getMCBoundingBox(double* xyzLimits) const
{
    for (int i=0 ; i<6 ; i++)
        xyzLimits[i] = xyz[i];
}

void Rug::prepareForFace(void *caller, int faceIndex)
{
    if (caller != nullptr)
    {
        Rug *rug = reinterpret_cast<Rug *>(caller);
        if (faceIndex == 5)
        {
            rug->piecesR[0]->setTexCoordsForBlock(faceIndex);
            rug->establishTexture();
        }
        else
        {
            glUniform1i(rug->shaderIF->ppuLoc("usingTextureMap"), 0);
        }
    }
}

void Rug::render()
{
    // 1. Save current and establish new current shader program
    GLint pgm;
    glGetIntegerv(GL_CURRENT_PROGRAM, &pgm);
    glUseProgram(shaderIF->getShaderPgmID());

    glUniform1i(shaderIF->ppuLoc("drawingOpaqueObjects"), 1);
    establishView();
    establishLightingEnvironment();

    renderRug();

    // 6. Reestablish previous shader program
    glUseProgram(pgm);
}

void Rug::renderRug()
{
    establishMaterial(rmatl); // walls
    if (piecesR[0] != nullptr)
        piecesR[0]->renderShape(prepareForFace, this);
    establishTexture();
    glUniform1i(shaderIF->ppuLoc("usingTextureMap"), 0);
}
