// present.c++

#include "Present.h"

Present::Present(ShaderIF *sIF, const char *presentTexImageSource, PhongMaterial &pMatlIn,
              cryph::AffPoint corner, cryph::AffVector u,
              double presentWidth, double presentHeight, double presentDepth)
: SceneElement(sIF), pMatl(pMatlIn)
{
    defineInitialGeometry(corner, u,
                          presentWidth, presentHeight, presentDepth);
    texID = readTextureImage(presentTexImageSource);
    xyz[0] = 1.0;
    xyz[1] = 0.0;
    for (int i = 0; i < 1; i++)
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

Present::~Present()
{
    for (int i = 0; i < 1; i++)
    {
        if (pieces[i] != nullptr)
            delete pieces[i];
        if (piecesR[i] != nullptr)
            delete piecesR[i];
    }
}

void Present::defineInitialGeometry(cryph::AffPoint corner, cryph::AffVector u,
                                  double presentWidth, double presentHeight, double presentDepth)
{
    // we assume the present is parallel to xy-plane, hence:
    cryph::AffVector uu(u[0], u[1], 0.0);
    cryph::AffVector ww(0, 0, 1);
    uu.normalize();
    cryph::AffVector vv = ww.cross(uu);
    corner = corner + ww * 0.01 + vv * 0.01; // to avoid bleading in to wall and floor

    pieces[0] = BasicShape::makeBlock(corner,
                                      uu, presentWidth,
                                      ww, presentHeight,
                                      vv, presentDepth);

}

void Present::getMCBoundingBox(double *xyzLimits) const
{
    for (int i = 0; i < 6; i++)
        xyzLimits[i] = xyz[i];
}

void Present::prepareForFace(void *caller, int faceIndex)
{
        if (caller != nullptr)
        {
            Present *p = reinterpret_cast<Present *>(caller);
            
                p->piecesR[0]->setTexCoordsForBlock(faceIndex);
                p->establishTexture();

        }
}

void Present::render()
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

    renderPresent();

    // 6. Reestablish previous shader program
    glUseProgram(pgm);
}

void Present::renderPresent()
{
    establishMaterial(pMatl);
    for (currentlyDrawingPiece = 0; currentlyDrawingPiece < 1; currentlyDrawingPiece++)
        if (piecesR[currentlyDrawingPiece] != nullptr)
            piecesR[currentlyDrawingPiece]->renderShape(prepareForFace, this);
}
