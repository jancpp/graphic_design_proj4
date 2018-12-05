// Rug.h

#ifndef Rug_H
#define Rug_H

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

class Rug : public SceneElement
{
public:
  Rug(ShaderIF *sIF, const char *rugTexImageSource, const PhongMaterial &rmatlIn,
       cryph::AffPoint corner, cryph::AffVector u,
       double rugWidth, double rugDepth,
       double width, double height, double depth);
  virtual ~Rug();

  // xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
  void getMCBoundingBox(double *xyzLimitsF) const;
  void render();
  void renderRug();
private:
	// IMPORTANT NOTE:
	// The ShaderIF will be stored with the SceneElement piece of
	// this object instance. You only need add instance variables here
	// that are unique to the new subclass you are creating.
  PhongMaterial wmatl, cmatl, fmatl, rmatl;
  BasicShape *pieces[1];
  BasicShapeRenderer *piecesR[1];
  double xyz[6];
  int currentlyDrawingPiece;

  void defineInitialGeometry(cryph::AffPoint corner, cryph::AffVector u,
							 double rugWidth, double rugDepth,
							 double width, double height, double depth);
  static void prepareForFace(void *caller, int faceIndex);
};

#endif
