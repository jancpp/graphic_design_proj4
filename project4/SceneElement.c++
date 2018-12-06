// SceneElement.c++

#include "SceneElement.h"
#include "OpenGLImageReader.h"

float SceneElement::lightPos[4*MAX_NUM_LIGHTS] =
{
    120.0, 72.0, 115.0, 0.0,
    55.0, 132.5, 43.0, 1.0,
    185.0, 132.5, 43.0, 1.0
};

// Are coordinates in "lightPos" stored in MC or EC? (You usually want them
// defined in MC, unless you are trying to simulate something like the viewer
// holding a flashlight while moving around the scene.)
bool SceneElement::posInModelCoordinates[MAX_NUM_LIGHTS] =
{ true, true, true };
// The following is the buffer actually sent to GLSL. It will contain a copy of
// the (x,y,z,w) for light sources defined in EC; it will contain the coordinates
// after transformation to EC if the position was originally specified in MC.
float posToGLSL[4*MAX_NUM_LIGHTS];

float SceneElement::lightStrength[3*MAX_NUM_LIGHTS] =
{
    0.5, 0.5, 0.5,
    0.6, 0.6, 0.6,
    0.6, 0.6, 0.6
};

float SceneElement::globalAmbient[] = { 0.7, 0.7, 0.7 };

SceneElement::SceneElement(ShaderIF *sIF) : shaderIF(sIF), texID(0)
{
}

SceneElement::~SceneElement()
{
}

void SceneElement::establishLightingEnvironment()
{
    // This should set all light source parameters, including:
    // "actualNumLights", "ecLightPosition", "lightStrength", "globalAmbient"
    // When setting "ecLightPosition", be sure to transform the coordinates
    // from MC to EC using the current mc_ec matrix.
    int actualNumLights = 3;
    cryph::Matrix4x4 mc_ec;
    cryph::Matrix4x4 ec_lds;
    ModelView::getMatrices(mc_ec, ec_lds);
    cryph::AffPoint inEC;
    float ecLightPosition[4 * MAX_NUM_LIGHTS];
    for (int i = 0; i < actualNumLights; i++) {
        for (int j = 4*i; j < 4*i+4; j++) {
            posToGLSL[j] = lightPos[j];
        }
        if (posInModelCoordinates[i]) {
            if (lightPos[4*i+3] == 1) {
                cryph::AffPoint inMC( lightPos[4*i], lightPos[4*i+1], lightPos[4*i+2]);
                inEC = mc_ec * inMC;
            } else {
                cryph::AffVector trans( lightPos[4*i], lightPos[4*i+1], lightPos[4*i+2]);
                inEC = mc_ec * trans;
            }
            posToGLSL[4*i]     = inEC[0];
            posToGLSL[4*i + 1] = inEC[1];
            posToGLSL[4*i + 2] = inEC[2];
        }
    }

    glUniform1i(shaderIF->ppuLoc("actualNumLights"), actualNumLights);
    glUniform4fv(shaderIF->ppuLoc("p_ecLightPos"), actualNumLights, posToGLSL);
    glUniform3fv(shaderIF->ppuLoc("lightStrength"), actualNumLights, lightStrength);
    glUniform3fv(shaderIF->ppuLoc("La"), 1, globalAmbient);
}

void SceneElement::establishMaterial(const PhongMaterial &matl)
{
    // This should set all material property parameters, including:
    // "ka", "kd", "ks", "shininess", and (for project 4) "alpha"
    glUniform3fv(shaderIF->ppuLoc("ka"), 1, matl.ka);
    glUniform3fv(shaderIF->ppuLoc("kd"), 1, matl.kd);
    glUniform3fv(shaderIF->ppuLoc("ks"), 1, matl.ks);
    glUniform1f(shaderIF->ppuLoc("m"), matl.shininess);
    glUniform1f(shaderIF->ppuLoc("alpha"), matl.alpha);
}

// NOTE: You may want to modify the interface to this method so that you
//       can pass parameters to help you specify some of the parameters
//       mentioned in the comments that follow.
void SceneElement::establishTexture( /* ... parameters? ... */ )
{
     glUniform1i(shaderIF->ppuLoc("usingTextureMap"), 1);
     glUniform1i(shaderIF->ppuLoc("textureMap"), 0);
     glActiveTexture(GL_TEXTURE0);
     glBindTexture(GL_TEXTURE_2D, texID);
}

void SceneElement::establishView()
{
    // Line of sight, dynamic view controls, 3D-2D projection, & mapping to LDS:
    cryph::Matrix4x4 mc_ec, ec_lds;
    ModelView::getMatrices(mc_ec, ec_lds);
    float m[16];
    glUniformMatrix4fv(shaderIF->ppuLoc("mc_ec"), 1, false, mc_ec.extractColMajor(m));
    glUniformMatrix4fv(shaderIF->ppuLoc("ec_lds"), 1, false, ec_lds.extractColMajor(m));

    ExtendedController *ec = dynamic_cast<ExtendedController *>(Controller::getCurrentController());
    glUniform1i(shaderIF->ppuLoc("sceneHasTranslucentObjects"), 1);
    if (ec->drawingOpaque())
        glUniform1i(shaderIF->ppuLoc("drawingOpaqueObjects"), 1);
    else
        glUniform1i(shaderIF->ppuLoc("drawingOpaqueObjects"), 0);
}

GLuint SceneElement::readTextureImage(const std::string& imgFileName)
{
    OpenGLImageReader* oglIR = OpenGLImageReader::create(imgFileName.c_str());
    if (oglIR == nullptr)
    {
        std::cerr << "Could not open '" << imgFileName << "' for texture map.\n";
        return 0;
    }
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    GLuint texID[1];
    glGenTextures(1, texID);
    glBindTexture(GL_TEXTURE_2D, texID[0]);
    float white[] = { 1.0, 1.0, 1.0, 1.0 };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, white);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    GLint level = 0;
    int pw = oglIR->getWidth(), ph = oglIR->getHeight();
    GLint iFormat = oglIR->getInternalFormat();
    GLenum format = oglIR->getFormat();
    GLenum type = oglIR->getType();
    const GLint border = 0; // must be zero (only present for backwards compatibility)
    const void* pixelData = oglIR->getTexture();
    glTexImage2D(GL_TEXTURE_2D, level, iFormat, pw, ph, border, format, type, pixelData);
    delete oglIR;
    return texID[0];
}
