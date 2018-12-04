// SceneElement.h - a base class that factors out many common data and
//                  method implementations to support a Phong local
//                  lighting model.

#ifndef SCENEELEMENT_H
#define SCENEELEMENT_H

#ifdef __APPLE_CC__
#include "GLFW/glfw3.h"
#else
#include <GL/gl.h>
#endif

#include "ModelView.h"
#include "PhongMaterial.h"
#include "ShaderIF.h"
#include "ExtendedController.h"

static const int MAX_NUM_LIGHTS = 4; // MUST BE KEPT SAME AS IN the shader program
typedef float vec2[2];
typedef float vec3[3];

class SceneElement : public ModelView
{
public:
	SceneElement(ShaderIF* sIF);
	virtual ~SceneElement();

	static GLuint readTextureImage(const std::string& imgFileName);

protected:
	ShaderIF* shaderIF;

	void establishLightingEnvironment(/* ... parameters? ... */);
	void establishMaterial(const PhongMaterial &matl);
	void establishTexture(/* ... parameters? ... */);
	void establishView();
	GLuint texID = 0;

	// lighting environment
	static float lightPos[4 * MAX_NUM_LIGHTS];		   // (x,y,z,w) for each light
	static bool posInModelCoordinates[MAX_NUM_LIGHTS]; // pos is in MC or EC?
	static float lightStrength[3*MAX_NUM_LIGHTS]; // (r,g,b) for each light
	static float globalAmbient[3]; // (r,g,b) for ambient term, A
};

#endif
