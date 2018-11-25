// Light.h

#ifndef Light_H
#define Light_H

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

class Light : public SceneElement
{
public:
  Light(ShaderIF *sIF, PhongMaterial &topMatlIn, PhongMaterial &bottomMatlIn,
	   cryph::AffPoint corner, cryph::AffVector u,
	   double heigth,
	   double radius);
  virtual ~Light();

  void getMCBoundingBox(double *xyzLimits) const; // {xmin, xmax, ymin, ymax, zmin, zmax}
  void render();
  void renderLight();

	  private : BasicShape *pieces[3];
  BasicShapeRenderer *piecesR[3];
  int currentlyDrawingPiece; // used only in context of "prepareForFace"
  PhongMaterial topMatl, bottomMatl;
  double xyz[6];

  void defineInitialGeometry(cryph::AffPoint corner, cryph::AffVector u,
							 double height, double radius);

  static void prepareForFace(void *caller, int faceIndex);
};

#endif
