// ECenter.h

#ifndef ECenter_H
#define ECenter_H

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

class ECenter : public SceneElement
{
public:
    ECenter(ShaderIF *sIF, const PhongMaterial &ecmatlIn,
            cryph::AffPoint corner, cryph::AffVector u,
            double width, double height, double depth);
    virtual ~ECenter();
    
    // xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
    void getMCBoundingBox(double *xyzLimitsF) const;
    void render();
    void renderECenter();
private:
    PhongMaterial ecmatl;
    BasicShape *pieces[5];
    BasicShapeRenderer *piecesR[5];
    double xyz[6];
    int currentlyDrawingPiece;
    
    void defineInitialGeometry(cryph::AffPoint corner, cryph::AffVector u,
                               double width, double height, double depth);
    static void prepareForFace(void *caller, int faceIndex);
};

#endif
