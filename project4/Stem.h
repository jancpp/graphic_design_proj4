// Stem.h

#ifndef Stem_H
#define Stem_H

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

class Stem : public SceneElement
{
public:
  Stem(ShaderIF *sIF, PhongMaterial &matlIn,
         cryph::AffPoint corner, cryph::AffVector u,
         double stemHeigth,
         double topRadius, double stemRadius);
  virtual ~Stem();

  void getMCBoundingBox(double *xyzLimits) const; // {xmin, xmax, ymin, ymax, zmin, zmax}
  void render();
  void renderStem();

private:
  static const int NUM_PIECES = 1;
  BasicShape *pieces[NUM_PIECES];
  BasicShapeRenderer *piecesR[NUM_PIECES];
  int currentlyDrawingPiece; // used only in context of "prepareForFace"
  PhongMaterial matl;
  double xyz[6];

  void defineInitialGeometry(cryph::AffPoint corner, cryph::AffVector u,
           double stemHeigth,
           double topRadius, double stemRadius);

  static void prepareForFace(void *caller, int faceIndex);
};

#endif
