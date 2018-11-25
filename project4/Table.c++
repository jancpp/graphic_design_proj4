// table.c++

#include "Table.h"

Table::Table(ShaderIF *sIF, PhongMaterial &tableMatlIn,
             cryph::AffPoint corner, cryph::AffVector u,
             double topHeight, double legRadius,
             double tableWidth, double tableHeight, double tableDepth)
: SceneElement(sIF), tableMatl(tableMatlIn)
{
    defineInitialGeometry(corner, u,
                          topHeight, legRadius,
                          tableWidth, tableHeight, tableDepth);
    xyz[0] = 1.0;
    xyz[1] = 0.0;
    for (int i = 0; i < 5; i++)
        if (pieces[i] == nullptr)
            piecesR[i] = nullptr;
        else
        {
            piecesR[i] = new BasicShapeRenderer(sIF, pieces[i]);
            // accumulate bounding box
            if (xyz[0] > xyz[1]) // not yet initialized
                pieces[i]->getMCBoundingBox(xyz);
            else
            {
                double thisxyz[6];
                pieces[i]->getMCBoundingBox(thisxyz);
                for (int j = 0; j < 3; j++)
                {
                    if (thisxyz[2 * j] < xyz[2 * j])
                        xyz[2 * j] = thisxyz[2 * j];
                    if (thisxyz[2 * j + 1] > xyz[2 * j + 1])
                        xyz[2 * j + 1] = thisxyz[2 * j + 1];
                }
            }
        }
}

Table::~Table()
{
    for (int i = 0; i < 5; i++)
    {
        if (pieces[i] != nullptr)
            delete pieces[i];
        if (piecesR[i] != nullptr)
            delete piecesR[i];
    }
}

void Table::defineInitialGeometry(cryph::AffPoint corner, cryph::AffVector u,
                                  double topHeight, double legRadius, 
                                  double tableWidth, double tableHeight, double tableDepth)
{
    // we assume the table is parallel to xy-plane, hence:
    cryph::AffVector uu(u[0], u[1], 0.0);
    uu.normalize();
    cryph::AffVector vv(0, 0, 1);
    cryph::AffVector ww = vv.cross(uu);
    corner = corner + vv * 0.01 + ww * 0.01; // to avoid bleading in to wall and floor
    
    // top
    pieces[0] = BasicShape::makeBlock(corner + vv * (tableHeight - topHeight),
                                      uu, tableWidth,
                                      vv, topHeight,
                                      ww, tableDepth);
    
    cryph::AffPoint bottom = corner - 	ww * legRadius * 1.5 + 
    uu * legRadius * 1.5;
    cryph::AffPoint top = bottom + vv * (tableHeight - topHeight);
    // 4 legs
    for (int i = 0; i < 3; i += 2)
    {
        cryph::AffPoint currBottom = bottom;
        cryph::AffPoint currTop = top;
        pieces[i + 1] = BasicShape::makeBoundedCylinder(currTop, currBottom,
                                                        legRadius, 20, 2, BasicShape::CAP_AT_BOTH);
        currBottom = bottom + uu * (tableWidth - 3.0 * legRadius);
        currTop = currBottom + vv * (tableHeight - topHeight);
        pieces[i + 2] = BasicShape::makeBoundedCylinder(currTop, currBottom,
                                                        legRadius, 20, 2, BasicShape::CAP_AT_BOTH);
        
        bottom = corner - ww * (tableDepth - 1.5 * legRadius) + uu * 1.5 * legRadius;
        top = bottom + vv * (tableHeight - topHeight);
    }
}

void Table::getMCBoundingBox(double *xyzLimits) const
{
    for (int i = 0; i < 6; i++)
        xyzLimits[i] = xyz[i];
}

void Table::prepareForFace(void *caller, int faceIndex)
{
}

void Table::render()
{
    // 1. Save current and establish new current shader program
    GLint pgm;
    glGetIntegerv(GL_CURRENT_PROGRAM, &pgm);
    glUseProgram(shaderIF->getShaderPgmID());
    
    // 2. Establish "mc_ec" and "ec_lds" matrices
    establishView();
    
    // 3. Establish Lighting environment
    //    complete the implementation of SceneElement::establishLightingEnvironment
    //    and then call it here.
    establishLightingEnvironment();
    
    renderTable();
    
    // 6. Reestablish previous shader program
    glUseProgram(pgm);
}

void Table::renderTable()
{
    // 4. Establish material property parameters.
    //    complete the implementation of SceneElement::establishMaterial
    //    and then call it here.
    establishMaterial(tableMatl);
    for (currentlyDrawingPiece = 0; currentlyDrawingPiece < 5; currentlyDrawingPiece++)
        if (piecesR[currentlyDrawingPiece] != nullptr)
            piecesR[currentlyDrawingPiece]->renderShape(prepareForFace, this);
    
    // 5. Establish any other attributes and make one or more calls to
    //    glDrawArrays and/or glDrawElements
    //    If all or part of this model involves texture mapping (project 4
    //    only), complete the implementation of SceneElement::establishTexture
    //    and call it from here as needed immediately before any glDrawArrays
    //    and/or glDrawElements calls to which texture is to be applied.
    establishTexture();
}
