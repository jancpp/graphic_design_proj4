// Table.h

#ifndef Table_H
#define Table_H

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

class Table : public SceneElement
{
  public:
	Table(ShaderIF *sIF, PhongMaterial &tableMatlIn,
		  cryph::AffPoint corner, cryph::AffVector u,
		  double topHeight, double legRadius,
		  double tableWidth, double tableHeight, double tableDepth);
	virtual ~Table();

	void getMCBoundingBox(double *xyzLimits) const; // {xmin, xmax, ymin, ymax, zmin, zmax}
	void render();
	void renderTable();

  private : 
  	BasicShape *pieces[5];
	BasicShapeRenderer *piecesR[5];
	int currentlyDrawingPiece; // used only in context of "prepareForFace"
	PhongMaterial tableMatl;
	double xyz[6];

	void defineInitialGeometry(cryph::AffPoint corner, cryph::AffVector u,
							   double topHeight, double legRadius,
							   double tableWidth, double tableHeight, double tableDepth);

	static void prepareForFace(void *caller, int faceIndex);
};

#endif
