// lamp.h

#ifndef lamp_H
#define lamp_H

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

class Lamp : public SceneElement
{
public:
    Lamp(ShaderIF *sIF, PhongMaterial &topMatlIn, PhongMaterial &poleMatlIn, PhongMaterial &bulbMatlIn,
         cryph::AffPoint corner, cryph::AffVector u,
         double lampHeigth,
         double topRadius, double midRadius,
         double poleHeight, double poleRadius,
         double baseRadius);
    virtual ~Lamp();
    
    void getMCBoundingBox(double *xyzLimits) const; // {xmin, xmax, ymin, ymax, zmin, zmax}
    void render();
    void renderLamp();
    
private: BasicShape *pieces[3];
    BasicShapeRenderer *piecesR[3];
    int currentlyDrawingPiece; // used only in context of "prepareForFace"
    PhongMaterial topMatl, poleMatl, bulbMatl;
    double xyz[6];
    
    void defineInitialGeometry(cryph::AffPoint corner, cryph::AffVector u,
                               double legHeight, double legRadius, double armRadius,
                               double lampWidth, double lampHeight, double lampDepth);
    
    static void prepareForFace(void *caller, int faceIndex);
};

#endif
