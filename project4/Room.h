// Room.h

#ifndef Room_H
#define Room_H

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

typedef float vec2[2];
typedef float vec3[3];
const int NUM_PPA_ATTRIBUTES = 4;

class Room : public SceneElement
{
public:
  Room(ShaderIF *sIF, const char *floorTexImageSource, const float d, const PhongMaterial &fmatlIn,
       const char *rugTexImageSource, const PhongMaterial &rmatlIn, float rugWidth, float rugDepth,
       const PhongMaterial &cmatlIn, const PhongMaterial &wmatlIn,
       cryph::AffPoint corner, cryph::AffVector u,
       double width, double height, double depth);
  virtual ~Room();

  void getMCBoundingBox(double *xyzLimitsF) const;
  void render();
  void renderFloor();
  void renderBackWall();
  void renderRightWall();
  void renderRug();
  void renderCeiling();

private:
  PhongMaterial fmatl, rmatl, cmatl, wmatl;
  GLenum wrapS, wrapT;
  float xmin, xmax, ymin, ymax, zmin, zmax, d;
  GLuint vao[NUM_PPA_ATTRIBUTES];
  GLuint vboFloor[1];
  GLuint vboBackWall[1];
  GLuint vboRightWall[1];
  GLuint vboRug[1];
  GLuint vboCeiling[1];

  void defineFloorGeometry(const char *floorTexImageSource, const float d);
  void defineRugGeometry(const char *rugTexImageSource, float rugWidthIn, float rugDepthIn);
  void defineBackWallGeometry();
  void defineRightWallGeometry();
  void defineCeilingGeometry();
};

#endif
