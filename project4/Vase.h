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
  Vase(ShaderIF *sIF, PhongMaterial &topMatlIn, PhongMaterial &poleMatlIn, PhongMaterial &bulbMatlIn,
       cryph::AffPoint corner, cryph::AffVector u,
       double VaseHeigth,
       double topRadius, double midRadius,
       double poleHeight, double poleRadius,
       double baseRadius);
  virtual ~Vase();

  void getMCBoundingBox(double *xyzLimits) const; // {xmin, xmax, ymin, ymax, zmin, zmax}
  void render();
  void renderVase();

private:
  BasicShape *pieces[3];
  BasicShapeRenderer *piecesR[3];
  int currentlyDrawingPiece; // used only in context of "prepareForFace"
  PhongMaterial topMatl, poleMatl, bulbMatl;
  double xyz[6];

  void defineInitialGeometry(cryph::AffPoint corner, cryph::AffVector u,
                             double legHeight, double legRadius, double armRadius,
                             double VaseWidth, double VaseHeight, double VaseDepth);

  static void prepareForFace(void *caller, int faceIndex);
};

#endif
