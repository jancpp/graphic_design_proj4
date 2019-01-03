// Vase.h

#ifndef Vase_H
#define Vase_H

#ifdef __APPLE_CC__
#include "GLFW/glfw3.h"
#else
#include <GL/gl.h>
#endif

#include "SceneElement.h"
#include "ShaderIF.h"
#include "AffPoint.h"
#include "AffVector.h"
#include "BasicShapeRenderer.h"

class Vase : public SceneElement
{
public:
    Vase(ShaderIF *sIF, PhongMaterial &vaseMatlIn,
         cryph::AffPoint corner, cryph::AffVector u,
         double VaseHeigth,
         double topRadius,
         double baseRadius);
    virtual ~Vase();
    
    void getMCBoundingBox(double *xyzLimits) const; // {xmin, xmax, ymin, ymax, zmin, zmax}
    void render();
    void renderVase();
    
private:
    static const int NUM_PIECES = 1;
    BasicShape *pieces[NUM_PIECES];
    BasicShapeRenderer *piecesR[NUM_PIECES];
    int currentlyDrawingPiece; // used only in context of "prepareForFace"
    PhongMaterial vaseMatl;
    double xyz[6];
    
    void defineInitialGeometry(cryph::AffPoint corner, cryph::AffVector u,
                               double VaseHeigth,
                               double topRadius,
                               double baseRadius);
    
    static void prepareForFace(void *caller, int faceIndex);
};

#endif
