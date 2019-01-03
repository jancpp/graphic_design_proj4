// Couch.h

#ifndef Couch_H
#define Couch_H

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

class Couch : public SceneElement
{
public:
    Couch(ShaderIF *sIF, PhongMaterial &couchMatlIn,
          cryph::AffPoint corner, cryph::AffVector u,
          double legHeight, double legRadius, double armRadius,
          double CouchWidth, double CouchHeight, double CouchDepth);
    virtual ~Couch();
    
    void getMCBoundingBox(double *xyzLimits) const; // {xmin, xmax, ymin, ymax, zmin, zmax}
    void render();
    void renderCouch();
    
private:
    BasicShape* pieces[10];
    BasicShapeRenderer* piecesR[10];
    int currentlyDrawingPiece; // used only in context of "prepareForFace"
    PhongMaterial couchMatl;
    double xyz[6];
    
    void defineInitialGeometry(cryph::AffPoint corner, cryph::AffVector u,
                               double legHeight, double legRadius, double armRadius,
                               double CouchWidth, double CouchHeight, double CouchDepth);
    
    static void prepareForFace(void *caller, int faceIndex);
};

#endif
