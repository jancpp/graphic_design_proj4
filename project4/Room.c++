// Room.c++

#include "Room.h"
Room::Room(ShaderIF *sIF, const char *floorTexImageSource, const float d, const PhongMaterial &fmatlIn,
           const char *rugTexImageSource, const PhongMaterial &rmatlIn, float rugWidth, float rugDepth,
           const PhongMaterial &cmatlIn, const PhongMaterial &wmatlIn,
           cryph::AffPoint corner, cryph::AffVector u,
           double width, double height, double depth)
    : SceneElement(sIF), fmatl(fmatlIn), rmatl(rmatlIn), wmatl(wmatlIn), cmatl(cmatlIn), wrapS(GL_CLAMP_TO_BORDER), wrapT(GL_CLAMP_TO_BORDER)
{
    width = width + 0.1; // to avoid furniture color bleading into walls
    depth = depth + 0.1;
    height = height + 0.1;

    xmin = corner.x;
    xmax = corner.x + width;
    ymin = corner.y;
    ymax = corner.y + depth;
    zmin = corner.z;
    zmax = corner.z + height;

    glGenVertexArrays(NUM_PPA_ATTRIBUTES, vao);
    defineFloorGeometry(floorTexImageSource, d);
    defineBackWallGeometry();
    defineRightWallGeometry();
    // defineRugGeometry(rugTexImageSource, rugWidth, rugDepth);
    // defineCeilingGeometry();
}

Room::~Room()
{
    glDeleteBuffers(1, vboBackWall);
    glDeleteBuffers(1, vboRightWall);
    // glDeleteBuffers(1, vboCeiling);
    glDeleteBuffers(1, vboFloor);
    // glDeleteBuffers(1, vboRug);
    glDeleteVertexArrays(NUM_PPA_ATTRIBUTES, vao);
}

void Room::defineFloorGeometry(const char *floorTexImageSource, const float d)
{


    texID = readTextureImage(floorTexImageSource);

    vec3 *normals = new vec3[4];
    vec3 *mcPosition = new vec3[4];
    vec2 *texCoords = new vec2[4];

    vec3 normalsValues[4] = {
        {0.0, 0.0, 1.0},
        {0.0, 0.0, 1.0},
        {0.0, 0.0, 1.0},
        {0.0, 0.0, 1.0}};
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            normals[i][j] = normalsValues[i][j];
        }
    }

    vec3 mcPositionValues[4] = {
        {xmin, ymin, 0.0},
        {xmin, ymax, 0.0},
        {xmax, ymin, 0.0},
        {xmax, ymax, 0.0}};

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            mcPosition[i][j] = mcPositionValues[i][j];
        }
    }
    vec2 texCoordsValues[4] = {
        {xmin / d, ymin / d},
        {xmin / d, ymax / d},
        {xmax / d, ymin / d},
        {xmax / d, ymax / d}};

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 2; j++)
        {
            texCoords[i][j] = texCoordsValues[i][j];
        }
    }

    glBindVertexArray(vao[0]);

    glGenBuffers(3, vboFloor);

    glBindBuffer(GL_ARRAY_BUFFER, vboFloor[0]);
    glBufferData(GL_ARRAY_BUFFER, 4 * 3 * sizeof(float), mcPosition, GL_STATIC_DRAW);
    glVertexAttribPointer(shaderIF->pvaLoc("mcPosition"), 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(shaderIF->pvaLoc("mcPosition"));

    glBindBuffer(GL_ARRAY_BUFFER, vboFloor[1]);
    glBufferData(GL_ARRAY_BUFFER, 4 * 3 * sizeof(vec3), normals, GL_STATIC_DRAW);
    glVertexAttribPointer(shaderIF->pvaLoc("mcNormal"), 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(shaderIF->pvaLoc("mcNormal"));

    glBindBuffer(GL_ARRAY_BUFFER, vboFloor[2]);
    glBufferData(GL_ARRAY_BUFFER, 4 * 3 * sizeof(vec2), texCoords, GL_STATIC_DRAW);
    glVertexAttribPointer(shaderIF->pvaLoc("texCoords"), 2, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(shaderIF->pvaLoc("texCoords"));

    delete[] normals;
    delete[] mcPosition;
    delete[] texCoords;
    }

    void Room::defineBackWallGeometry()
    {
        vec3 *normals = new vec3[4];
        vec3 *mcPosition = new vec3[4];

        vec3 normalsValues[4] = {
            {0.0, -1.0, 0.0},
            {0.0, -1.0, 0.0},
            {0.0, -1.0, 0.0},
            {0.0, -1.0, 0.0}};
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                normals[i][j] = normalsValues[i][j];
            }
        }

        vec3 mcPositionValues[4] = {
            {xmin, ymax, zmin},
            {xmin, ymax, zmax},
            {xmax, ymax, zmin},
            {xmax, ymax, zmax}};

        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                mcPosition[i][j] = mcPositionValues[i][j];
            }
        }
          glBindVertexArray(vao[1]);

        glGenBuffers(2, vboBackWall);
        glDisableVertexAttribArray(shaderIF->pvaLoc("mcNormal"));

        glBindBuffer(GL_ARRAY_BUFFER, vboBackWall[0]);
        glBufferData(GL_ARRAY_BUFFER, 4 * 3 * sizeof(float), mcPosition, GL_STATIC_DRAW);
        glVertexAttribPointer(shaderIF->pvaLoc("mcPosition"), 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(shaderIF->pvaLoc("mcPosition"));

        glBindBuffer(GL_ARRAY_BUFFER, vboBackWall[1]);
        glBufferData(GL_ARRAY_BUFFER, 4 * 3 * sizeof(vec3), normals, GL_STATIC_DRAW);
        glVertexAttribPointer(shaderIF->pvaLoc("mcNormal"), 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(shaderIF->pvaLoc("mcNormal"));

        delete[] normals;
        delete[] mcPosition;
    }

    void Room::defineRightWallGeometry()
    {
        vec3 *normals = new vec3[4];
        vec3 *mcPosition = new vec3[4];

        vec3 normalsValues[4] = {
            {-1.0, 0.0, 0.0},
            {-1.0, 0.0, 0.0},
            {-1.0, 0.0, 0.0},
            {-1.0, 0.0, 0.0}};

        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                normals[i][j] = normalsValues[i][j];
            }
        }

        vec3 mcPositionValues[4] = {
            {xmax, ymin, zmin},
            {xmax, ymin, zmax},
            {xmax, ymax, zmin},
            {xmax, ymax, zmax}};

        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                mcPosition[i][j] = mcPositionValues[i][j];
            }
        }

        glBindVertexArray(vao[2]);

        glGenBuffers(2, vboRightWall);

        glBindBuffer(GL_ARRAY_BUFFER, vboRightWall[0]);
        glBufferData(GL_ARRAY_BUFFER, 4 * 3 * sizeof(float), mcPosition, GL_STATIC_DRAW);
        glVertexAttribPointer(shaderIF->pvaLoc("mcPosition"), 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(shaderIF->pvaLoc("mcPosition"));

        glBindBuffer(GL_ARRAY_BUFFER, vboRightWall[1]);
        glBufferData(GL_ARRAY_BUFFER, 4 * 3 * sizeof(vec3), normals, GL_STATIC_DRAW);
        glVertexAttribPointer(shaderIF->pvaLoc("mcNormal"), 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(shaderIF->pvaLoc("mcNormal"));

        delete[] normals;
        delete[] mcPosition;
    }



    void Room::defineRugGeometry(const char *rugTexImageSource,
                                 float rugWidthIn, float rugDepthIn)
    {

        texID = readTextureImage(rugTexImageSource);

        vec3 *normals = new vec3[4];
        vec3 *mcPosition = new vec3[4];
        vec2 *texCoords = new vec2[4];

        vec3 normalsValues[4] = {
            {0.0, 0.0, 1.0},
            {0.0, 0.0, 1.0},
            {0.0, 0.0, 1.0},
            {0.0, 0.0, 1.0}};
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                normals[i][j] = normalsValues[i][j];
            }
        }

        vec3 mcPositionValues[4] = {
            {(xmax - xmin - rugWidthIn) / 2, (ymax - ymin - rugDepthIn) / 2, 0.01},
            {(xmax - xmin - rugWidthIn) / 2, (ymax - ymin + rugDepthIn) / 2, 0.01},
            {(xmax - xmin + rugWidthIn) / 2, (ymax - ymin - rugDepthIn) / 2, 0.01},
            {(xmax - xmin + rugWidthIn) / 2, (ymax - ymin + rugDepthIn) / 2, 0.01}};

        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                mcPosition[i][j] = mcPositionValues[i][j];
            }
        }
        vec2 texCoordsValues[4] = {
            {(xmax - xmin - rugWidthIn) / 2, (ymax - ymin - rugDepthIn) / 2},
            {(xmax - xmin - rugWidthIn) / 2, (ymax - ymin + rugDepthIn) / 2},
            {(xmax - xmin + rugWidthIn) / 2, (ymax - ymin - rugDepthIn) / 2},
            {(xmax - xmin + rugWidthIn) / 2, (ymax - ymin + rugDepthIn) / 2}};

        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 2; j++)
            {
                texCoords[i][j] = texCoordsValues[i][j];
            }
        }

        glBindVertexArray(vao[3]);

        glGenBuffers(3, vboFloor);

        glBindBuffer(GL_ARRAY_BUFFER, vboFloor[0]);
        glBufferData(GL_ARRAY_BUFFER, 4 * 3 * sizeof(float), mcPosition, GL_STATIC_DRAW);
        glVertexAttribPointer(shaderIF->pvaLoc("mcPosition"), 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(shaderIF->pvaLoc("mcPosition"));

        glBindBuffer(GL_ARRAY_BUFFER, vboFloor[1]);
        glBufferData(GL_ARRAY_BUFFER, 4 * 3 * sizeof(vec3), normals, GL_STATIC_DRAW);
        glDisableVertexAttribArray(shaderIF->pvaLoc("mcNormal"));
        glVertexAttribPointer(shaderIF->pvaLoc("mcNormal"), 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(shaderIF->pvaLoc("mcNormal"));

        glBindBuffer(GL_ARRAY_BUFFER, vboFloor[2]);
        glBufferData(GL_ARRAY_BUFFER, 4 * 3 * sizeof(vec2), texCoords, GL_STATIC_DRAW);
        glVertexAttribPointer(shaderIF->pvaLoc("texCoords"), 2, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(shaderIF->pvaLoc("texCoords"));

        delete[] normals;
        delete[] mcPosition;
        delete[] texCoords;
    }

    void Room::defineCeilingGeometry()
    {
        vec3 *normals = new vec3[4];
        vec3 *mcPosition = new vec3[4];

        vec3 normalsValues[4] = {
            {0.0, 0.0, -1.0},
            {0.0, 0.0, -1.0},
            {0.0, 0.0, -1.0},
            {0.0, 0.0, -1.0}};

        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                normals[i][j] = normalsValues[i][j];
            }
        }

        vec3 mcPositionValues[4] = {
            {xmin, ymin, zmax},
            {xmax, ymin, zmax},
            {xmin, ymax, zmax},
            {xmax, ymax, zmax}};

        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 3; j++)
            {
                mcPosition[i][j] = mcPositionValues[i][j];
            }
        }

        glBindVertexArray(vao[4]);

        glGenBuffers(2, vboCeiling);

        glBindBuffer(GL_ARRAY_BUFFER, vboCeiling[0]);
        glBufferData(GL_ARRAY_BUFFER, 4 * 3 * sizeof(float), mcPosition, GL_STATIC_DRAW);
        glVertexAttribPointer(shaderIF->pvaLoc("mcPosition"), 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(shaderIF->pvaLoc("mcPosition"));

        glBindBuffer(GL_ARRAY_BUFFER, vboCeiling[1]);
        glBufferData(GL_ARRAY_BUFFER, 4 * 3 * sizeof(vec3), normals, GL_STATIC_DRAW);
        glDisableVertexAttribArray(shaderIF->pvaLoc("mcNormal"));
        glVertexAttribPointer(shaderIF->pvaLoc("mcNormal"), 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(shaderIF->pvaLoc("mcNormal"));

        delete[] normals;
        delete[] mcPosition;
    }

    void Room::getMCBoundingBox(double *xyzLimits) const
    {
        xyzLimits[0] = xmin;
        xyzLimits[1] = xmax;
        xyzLimits[2] = ymin;
        xyzLimits[3] = ymax;
        xyzLimits[4] = zmin;
        xyzLimits[5] = zmax;
    }

    void Room::render()
    {
        GLint pgm;
        glGetIntegerv(GL_CURRENT_PROGRAM, &pgm);
        glUseProgram(shaderIF->getShaderPgmID());

        establishLightingEnvironment();
        establishView();

        renderFloor();
        renderBackWall();
        renderRightWall();
        // renderRug();
        // renderCeiling();

        glUseProgram(pgm);
    }

    void Room::renderFloor()
    {
        establishMaterial(fmatl);
        establishTexture();

        glBindVertexArray(vao[0]);
        glVertexAttrib3f(shaderIF->pvaLoc("mcNormal"), 0.0, 0.0, 1.0);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        // TURN OFF your "using a texture map" uniform variable before leaving.
        glUniform1i(shaderIF->ppuLoc("usingTextureMap"), 0);
    }

    void Room::renderBackWall()
    {

        // wmatl.alpha = 0.5;
        establishMaterial(wmatl);
        // glUniform1i(shaderIF->ppuLoc("sceneHasTranslucentObjects"), 1);

        glBindVertexArray(vao[1]);
        glDisableVertexAttribArray(shaderIF->pvaLoc("mcNormal"));
        glVertexAttrib3f(shaderIF->pvaLoc("mcNormal"), 0.0, -1.0, 0.0);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        // glUniform1i(shaderIF->ppuLoc("usingTextureMap"), 0);
    }

    void Room::renderRightWall()
    {
        wmatl.alpha = 1.0;
        establishMaterial(wmatl);
        glUniform1i(shaderIF->ppuLoc("sceneHasTranslucentObjects"), 0);

        glBindVertexArray(vao[2]);
        glDisableVertexAttribArray(shaderIF->pvaLoc("mcNormal"));
        glVertexAttrib3f(shaderIF->pvaLoc("mcNormal"), -1.0, 0.0, 0.0);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }

    void Room::renderRug()
    {
        establishMaterial(rmatl);
        establishTexture();

        glBindVertexArray(vao[3]);
        glDisableVertexAttribArray(shaderIF->pvaLoc("mcNormal"));
        glVertexAttrib3f(shaderIF->pvaLoc("mcNormal"), 0.0, 0.0, 1.0);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        // TURN OFF your "using a texture map" uniform variable before leaving.
        glUniform1i(shaderIF->ppuLoc("usingTextureMap"), 0);
    }

    void Room::renderCeiling()
    {
        establishMaterial(cmatl);

        glBindVertexArray(vao[4]);
        glDisableVertexAttribArray(shaderIF->pvaLoc("mcNormal"));
        glVertexAttrib3f(shaderIF->pvaLoc("mcNormal"), 0.0, 0.0, -1.0);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }
