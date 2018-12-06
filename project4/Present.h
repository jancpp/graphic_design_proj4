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
  Present(ShaderIF *sIF, const char *presentTexImageSource, PhongMaterial &pMatlIn,
		cryph::AffPoint corner, cryph::AffVector u,
		double presentWidth, double presentHeight, double presentDepth);
  virtual ~Present();

  void getMCBoundingBox(double *xyzLimits) const; // {xmin, xmax, ymin, ymax, zmin, zmax}
  void render();
  void renderPresent();

private:
	BasicShape* pieces[1];
	BasicShapeRenderer* piecesR[1];
	int currentlyDrawingPiece; // used only in context of "prepareForFace"
	PhongMaterial pMatl;
	double xyz[6];

	void defineInitialGeometry(cryph::AffPoint corner, cryph::AffVector u,
							   double presentWidth, double presentHeight, double presentDepth);

	static void prepareForFace(void *caller, int faceIndex);
};

#endif
