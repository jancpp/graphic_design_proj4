// Tree.h

#ifndef Tree_H
#define Tree_H

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

class Tree : public SceneElement
{
public:
    Tree(ShaderIF *sIF, PhongMaterial &matlIn,
         cryph::AffPoint corner, cryph::AffVector u,
         double treeHeigth,
         double topRadius, double midRadius, double baseRadius);
    virtual ~Tree();
    
    void getMCBoundingBox(double *xyzLimits) const; // {xmin, xmax, ymin, ymax, zmin, zmax}
    void render();
    void renderTree();
    
private:
    static const int NUM_PIECES = 5;
    BasicShape *pieces[NUM_PIECES];
    BasicShapeRenderer *piecesR[NUM_PIECES];
    int currentlyDrawingPiece; // used only in context of "prepareForFace"
    PhongMaterial matl;
    double xyz[6];
    
    void defineInitialGeometry(cryph::AffPoint corner, cryph::AffVector u,
                               double treeHeigth,
                               double topRadius, double midRadius, double baseRadius);
    
    static void prepareForFace(void *caller, int faceIndex);
};

#endif
