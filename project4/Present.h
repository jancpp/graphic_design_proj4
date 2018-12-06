// Present.h

#ifndef Present_H
#define Present_H

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

class Present : public SceneElement
{
public:
  Present(ShaderIF *sIF, const char *presentTexImageSource, const PhongMaterial &rmatlIn,
          cryph::AffPoint corner, cryph::AffVector u,
          double presentWidth, double presentHeight, double presentDepth);
  virtual ~Present();

  // xyzLimits: {mcXmin, mcXmax, mcYmin, mcYmax, mcZmin, mcZmax}
  void getMCBoundingBox(double *xyzLimitsF) const;
  void render();
  void renderPresent();
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
                             double presentWidth, double presentHeight, double presentDepth);
  static void prepareForFace(void *caller, int faceIndex);
};

#endif
