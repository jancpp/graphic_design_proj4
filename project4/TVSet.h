// TVSet.h

#ifndef TVSet_H
#define TVSet_H

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

class TVSet : public SceneElement
{
public:
  TVSet(ShaderIF *sIF, const char *tvTexImageSource, PhongMaterial &TVSetMatlIn,
		cryph::AffPoint corner, cryph::AffVector u,
		double tvWidth, double tvHeight, double tvDepth);
  virtual ~TVSet();

  void getMCBoundingBox(double *xyzLimits) const; // {xmin, xmax, ymin, ymax, zmin, zmax}
  void render();
  void renderTVSet();

private:
	BasicShape* pieces[3]; 
	BasicShapeRenderer* piecesR[3];
	int currentlyDrawingPiece; // used only in context of "prepareForFace"
	PhongMaterial tvMatl;
	double xyz[6];

	void defineInitialGeometry(cryph::AffPoint corner, cryph::AffVector u,
							   double tvWidth, double tvHeight, double tvDepth);

	static void prepareForFace(void *caller, int faceIndex);
};

#endif
