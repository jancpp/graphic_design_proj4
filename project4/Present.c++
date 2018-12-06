// Present.c++

#include "Present.h"

Present::Present(ShaderIF *sIF, const char *presentTexImageSource, const PhongMaterial &rmatlIn,
                 cryph::AffPoint corner, cryph::AffVector u,
                 double presentWidth, double presentHeight, double presentDepth) : SceneElement(sIF), rmatl(rmatlIn)
{
    defineInitialGeometry(corner, u,
                          presentWidth, presentHeight, presentDepth);
    texID = readTextureImage(presentTexImageSource);
    
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

Present::~Present()
{
    for (int i=0 ; i<1 ; i++)
    {
        if (pieces[i] != nullptr)
            delete pieces[i];
        if (piecesR[i] != nullptr)
            delete piecesR[i];
    }
}

void Present::defineInitialGeometry(cryph::AffPoint corner, cryph::AffVector u,
                                    double presentWidth, double presentHeigth, double presentDepth)
{
    
    cryph::AffVector uu(u[0], u[1], 0.0);
    uu.normalize();
    cryph::AffVector vv(0, 1, 0);
    cryph::AffVector ww = vv.cross(uu);
    
    
    // Present
    pieces[0] = BasicShape::makeBlock(corner,
                                      uu, presentWidth,
                                      vv, presentDepth,
                                      ww, presentHeigth);
}

// xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
void Present::getMCBoundingBox(double* xyzLimits) const
{
    for (int i=0 ; i<6 ; i++)
        xyzLimits[i] = xyz[i];
}

void Present::prepareForFace(void *caller, int faceIndex)
{
    if (caller != nullptr)
    {
        Present *p = reinterpret_cast<Present *>(caller);
        if (faceIndex == 5)
        {
            p->piecesR[0]->setTexCoordsForBlock(faceIndex);
            p->establishTexture();
        }
        else
        {
            glUniform1i(p->shaderIF->ppuLoc("usingTextureMap"), 0);
        }
    }
}

void Present::render()
{
    // 1. Save current and establish new current shader program
    GLint pgm;
    glGetIntegerv(GL_CURRENT_PROGRAM, &pgm);
    glUseProgram(shaderIF->getShaderPgmID());

    glUniform1i(shaderIF->ppuLoc("drawingOpaqueObjects"), 1);
    establishView();
    establishLightingEnvironment();
    
    renderPresent();
    
    // 6. Reestablish previous shader program
    glUseProgram(pgm);
}

void Present::renderPresent()
{
    // 3. Establish material property parameters.
    //    complete the implementation of SceneElement::establishMaterial
    //    and then call it here.
    // for (currentlyDrawingPiece = 0; currentlyDrawingPiece < 4; currentlyDrawingPiece++)
    // 	if (piecesR[currentlyDrawingPiece] != nullptr)
    // 		piecesR[currentlyDrawingPiece]->renderShape(prepareForFace, this);
    establishMaterial(rmatl); // walls
    if (piecesR[0] != nullptr)
        piecesR[0]->renderShape(prepareForFace, this);

        // 4. Establish any other attributes and make one or more calls to
    //    glDrawArrays and/or glDrawElements
    //    If all or part of this model involves texture mapping (project 3
    //    only), complete the implementation of SceneElement::establishTexture
    //    and call it from here as needed immediately before any glDrawArrays
    //    and/or glDrawElements calls to which texture is to be applied.
    establishTexture();
    glUniform1i(shaderIF->ppuLoc("usingTextureMap"), 0);
}
