// main.c++: Starter for EECS 672 Projects 2-4

#include <iostream>
#include <algorithm>
#include "GLFWController.h"
#include "ExtendedController.h"
#include "Table.h"
#include "Couch.h"
#include "Room.h"
#include "Rug.h"
#include "Lamp.h"
#include "Light.h"
#include "ECenter.h"
#include "TVSet.h"
#include "Ornaments.h"
#include "Tree.h"
#include "Present.h"
#include "Flower.h"
#include "Stem.h"
#include "Vase.h"

void createScene(ExtendedController &c, ShaderIF *sIF, ShaderIF *sIFwGeoL, ShaderIF *sIFwGeoM)
{
    cryph::AffVector u1(1.0, 0.0, 0.0);
    cryph::AffVector u2(1.0, 1.0, 0.0);
    cryph::AffVector u3(0.0, 1.0, 0.0);
    cryph::AffVector v1(0.0, 1.0, 0.0);
    cryph::AffVector w1(0.0, 0.0, 1.0);
    cryph::AffVector k1(1.0, 1.0, 0.0);
    
    // units are in inches
    double roomHeight = 120.0, roomWidth = 240.0, roomDepth = 144.0;
    double rugWidth = 95.0, rugDepth = 60.0;
    double endTableHeight = 25.0, endTableDepth = 25.0, endTableWidth = 23.0;
    double couchHeight = 36.0, couchWidth = 100.0, couchDepth = 38.0;
    double treeBaseRadius = 15, treeHeigth = 80;
    double vaseHeight = 12, vaseTopRadius = 2.0, vaseBaseRadius = 1.5;
    double lampHeight = 29.0, topRadius = 5.0, midRadius = 10.0;
    double armChairHeight = 36.0, armChairWidth = 56.0, armChairDepth = 38.0;
    double ecHeight = 30.0, ecWidth = 60.0, ecDepth = 28.0;
    double tvHeight = 28.0, tvWidth = 40.0, tvDepth = 2.0;
    double lightHeigth = 8.0, lightRadius = 12.0;
    double tableHeight = 18.0, tableDepth = 28.0, tableWidth = 48.0;
    double presentHeight1 = 2.0, presentDepth1 = 11.0, presentWidth1 = 20.0;
    double presentHeight2 = 6.0, presentDepth2 = 18.0, presentWidth2 = 10.0;
    double poleHeight = 18.0, poleRadius = 1.5, baseRadius = 6.0;
    double flowerHeight = 18, flowerTopRadius = 3.5, flowerBaseRadius = 0.3;
    double stemHeight = 18, stemTopRadius = 0.2, stemBaseRadius = 0.3;
    
    const char *floorTexImageSource = "images/wood.jpg";
    const float floordIn = 35;
    const char *rugTexImageSource = "images/rug.jpg";
    const char *presentTexImageSource1 = "images/wrap_paper1.jpg";
    const char *presentTexImageSource2 = "images/wrap_paper2.jpg";
    cryph::AffPoint p1(0.0, 0.0, 0.0);
    cryph::AffPoint midRoom(roomWidth / 2, roomDepth / 2, 0.0);
    PhongMaterial floorMatl(0.5, 0.3, 0.15, // r, g, b
                            0.5, 0.8, 0.5,  // ka, kd, ks
                            25.0, 1.0);     // m, alpha
    PhongMaterial rugMatl(1.0, 1.0, 1.0,
                          0.5, 0.2, 0.1,
                          2.0, 1.0);
    PhongMaterial ceilingMatl(0.99, 0.99, 0.99,
                              0.75, 0.6, 0.01,
                              1.0, 1.0);
    PhongMaterial wallMatl(0.85, 0.95, 0.85,
                           0.75, 0.5, 0.01,
                           5.0, 1.0);
    PhongMaterial topCeilingMatl(0.75, 0.0, 0.0,
                                 0.95, 0.2, 0.1,
                                 2.0, 1.0);
    PhongMaterial bottomMatl(0.99, 0.99, 0.99,
                             0.95, 0.95, 0.1,
                             10.0, 1.0);
    PhongMaterial treeMatl(0.1, 0.8, 0.1,
                           0.2, 0.2, 0.01,
                           2.0, 1.0);
    PhongMaterial yellowMatl(1.0, 1.0, 0.0,
                             0.6, 0.6, 0.6,
                             80.0, 1.0);
    PhongMaterial redMatl(1.0, 0.0, 0.0,
                          0.6, 0.6, 0.6,
                          80.0, 1.0);
    PhongMaterial whiteMatl(1.0, 1.0, 1.0,
                            0.6, 0.6, 0.6,
                            80.0, 1.0);
    PhongMaterial blueMatl(0.0, 0.0, 1.0,
                           0.6, 0.6, 0.6,
                           80.0, 1.0);
    PhongMaterial couchMatl(0.75, 0.0, 0.0,
                            0.35, 0.2, 0.1,
                            2.0, 1.0);
    PhongMaterial topMatl(0.99, 0.99, 0.99,
                          0.95, 0.8, 0.1,
                          2.0, 1.0);
    PhongMaterial poleMatl(0.2124, 0.127, 0.054,
                           0.714, 0.423, 0.181,
                           0.393, 0.272, 0.167,
                           25.0, 1.0);
    PhongMaterial bulbMatl(0.25, 0.2, 0.2,
                           1.0, 0.83, 0.83,
                           0.3, 0.3, 0.3,
                           2.0, 1.0);
    PhongMaterial tableMatl(0.75, 0.0, 0.0,
                            0.35, 0.4, 0.1,
                            2.0, 1.0);
    PhongMaterial tvMatl(0.1, 0.1, 0.1,
                         0.5, 0.8, 0.3,
                         40.0, 1.0);
    PhongMaterial ecMatl(0.75, 0.0, 0.0,
                         0.5, 0.2, 0.1,
                         10.0, 1.0);
    PhongMaterial vaseMatl(0.77, 0.88, 0.89,
                           0.5, 0.5, 0.2,
                           10.0, 0.6);
    // room
    Room *r1 = new Room(sIF, floorTexImageSource, floordIn, floorMatl,
                        rugTexImageSource, rugMatl, rugWidth, rugDepth,
                        ceilingMatl, wallMatl,
                        p1, u1,
                        roomWidth, roomHeight, roomDepth);
    c.addModel(r1);
    // vase
    cryph::AffPoint p17(midRoom.x,
                        midRoom.y ,
                        midRoom.z + tableHeight);
    
    Vase *vs1 = new Vase(sIF, vaseMatl,
                         p17, u1,
                         vaseHeight, vaseTopRadius, vaseBaseRadius);
    c.addModel(vs1);
    // flower 1
    Flower *f1a = new Flower(sIFwGeoM, yellowMatl, p17, u1, flowerHeight, flowerTopRadius, flowerBaseRadius);
    c.addModel(f1a);
    Stem *f1b = new Stem(sIF, treeMatl, p17, u1, stemHeight, stemTopRadius, stemBaseRadius);
    c.addModel(f1b);
    // flower 2
    Flower *f2a = new Flower(sIFwGeoL, redMatl, p17, u2, flowerHeight + 2.0, flowerTopRadius, flowerBaseRadius);
    c.addModel(f2a);
    Stem *f2b = new Stem(sIF, treeMatl, p17, u2, stemHeight + 2.0, stemTopRadius, stemBaseRadius);
    c.addModel(f2b);
    // flower 3
    Flower *f3a = new Flower(sIFwGeoL, blueMatl, p17, u3, flowerHeight + 6.0, flowerTopRadius, flowerBaseRadius);
    c.addModel(f3a);
    Stem *f3b = new Stem(sIF, treeMatl, p17, u3, stemHeight + 6.0, stemTopRadius, stemBaseRadius);
    c.addModel(f3b);
    
    // rug
    Rug *r2 = new Rug(sIF, rugTexImageSource, rugMatl,
                      p1, u1,
                      rugWidth, rugDepth,
                      roomWidth, roomHeight, roomDepth);
    c.addModel(r2);
    
    // couch
    cryph::AffPoint p2(midRoom.x - couchWidth / 2, roomDepth, 0.0);
    
    Couch *c1 = new Couch(sIF, couchMatl, p2, u1,
                          4.0, 3.0, 5.0, couchWidth, couchHeight, couchDepth);
    c.addModel(c1);
    
    // arm chair
    cryph::AffPoint p22(0.01, (roomDepth - armChairWidth) / 2, 0.0);
    Couch *c2 = new Couch(sIF, couchMatl, p22, v1,
                          4.0, 3.0, 5.0, armChairWidth, armChairHeight, armChairDepth);
    c.addModel(c2);
    
    // left end table
    cryph::AffPoint p3(p2.x - endTableWidth - 2.0, roomDepth, 0.0);
    
    Table *t1 = new Table(sIF, tableMatl, p3, u1,
                          4.5, 2.0, endTableWidth, endTableHeight, endTableDepth);
    c.addModel(t1);
    
    // right end table
    cryph::AffPoint p4(p2.x + couchWidth + 2.0, roomDepth, 0.0);
    Table *t2 = new Table(sIF, tableMatl, p4, u1,
                          4.5, 2.0, endTableWidth, endTableHeight, endTableDepth);
    c.addModel(t2);
    
    // table
    cryph::AffPoint p5(midRoom.x - tableWidth / 2,
                       midRoom.y + tableDepth / 2,
                       0.0);
    Table *t3 = new Table(sIF, tableMatl, p5, u1,
                          4.5, 2.0, tableWidth, tableHeight, tableDepth);
    c.addModel(t3);
    
    // left lamp
    cryph::AffPoint p6(p3.x + endTableWidth / 2,
                       p3.y - endTableDepth / 2,
                       p3.z + endTableHeight);
    Lamp *l1 = new Lamp(sIF, topMatl, poleMatl, bulbMatl,
                        p6, u1,
                        lampHeight,
                        topRadius, midRadius,
                        poleHeight, poleRadius,
                        baseRadius);
    c.addModel(l1);
    
    // right lamp
    cryph::AffPoint p7(p4.x + endTableWidth / 2,
                       p4.y - endTableDepth / 2,
                       p4.z + endTableHeight);
    Lamp *l2 = new Lamp(sIF, topMatl, poleMatl, bulbMatl,
                        p7, u1,
                        lampHeight,
                        topRadius, midRadius,
                        poleHeight, poleRadius,
                        baseRadius);
    c.addModel(l2);
    
    // entertainment center
    cryph::AffPoint p8(roomWidth, midRoom.y + ecWidth / 2, 0.01);
    
    ECenter *ec1 = new ECenter(sIF, ecMatl, p8, -v1, ecWidth, ecHeight, ecDepth);
    c.addModel(ec1);
    
    // TV set
    cryph::AffPoint p9(roomWidth - ecDepth/2, midRoom.y + tvWidth / 2, ecHeight);
    const char *tvTexImageSource = "images/mahomes.png";
    
    TVSet *tv1 = new TVSet(sIF, tvTexImageSource, tvMatl, p9, -v1, tvWidth, tvHeight, tvDepth);
    c.addModel(tv1);
    
    // light
    cryph::AffPoint p10(midRoom.x, midRoom.y, roomHeight);
    Light *l3 = new Light(sIF, poleMatl, bottomMatl,
                          p10, u1,
                          lightHeigth, lightRadius);
    // c.addModel(l3);
    
    // christmas tree
    cryph::AffPoint p11(p1.x + roomWidth - 20, p1.y + 20, 14.0);
    cryph::AffPoint p12(p1.x + roomWidth - 20, p1.y + 20, 0.01);
    topRadius = 12;
    midRadius = 16;
    baseRadius = 20;
    poleHeight = 75;
    Tree *xmasTreeTop = new Tree(sIFwGeoL, treeMatl,
                                 p11, u1,
                                 treeHeigth - 3.0, topRadius - 3.0, midRadius - 3.0, baseRadius - 3.0);
    c.addModel(xmasTreeTop);
    Lamp *xmasTreeStand = new Lamp(sIF, poleMatl, poleMatl, bulbMatl,
                                   p12, u1,
                                   60.0,
                                   0.2, 1.0,
                                   30, 1.0,
                                   topRadius);
    c.addModel(xmasTreeStand);
    Ornaments *xmasOrnamentsYellow = new Ornaments(sIFwGeoM, yellowMatl,
                                                   p11, u1,
                                                   treeHeigth, topRadius, midRadius, baseRadius, 2);
    c.addModel(xmasOrnamentsYellow);
    Ornaments *xmasOrnamentsRed = new Ornaments(sIFwGeoM, redMatl,
                                                p11, u1,
                                                treeHeigth, topRadius, midRadius, baseRadius, 4);
    c.addModel(xmasOrnamentsRed);
    
    // presents
    cryph::AffPoint p13(p12.x - treeBaseRadius*2, p12.y + treeBaseRadius/2, 0.01);
    cryph::AffPoint p14(p13.x-5, p13.y - 12, 0.01);
    
    Present *pr1 = new Present(sIF, presentTexImageSource2, whiteMatl,
                               p13, u2,
                               presentWidth1, presentHeight1, presentDepth1);
    c.addModel(pr1);
    
    Present *pr2 = new Present(sIF, presentTexImageSource1, whiteMatl,
                               p14, u2,
                               presentWidth2, presentHeight2, presentDepth2);
    c.addModel(pr2);
}

void set3DViewingInformation(double overallBB[])
{
    // Viewing Strategy #1,
    // Notify class ModelView that we initially want to see the entire scene:
    ModelView::setMCRegionOfInterest(overallBB);
    // diameter r = max(Δx/2, Δy/2, Δz/2)
    double xdelta = overallBB[1] - overallBB[0];
    double ydelta = overallBB[3] - overallBB[2];
    double zdelta = overallBB[5] - overallBB[4];
    
    double diameter = xdelta;
    if (ydelta > diameter) {
        diameter = ydelta;
    }
    if (zdelta > diameter) {
        diameter = zdelta;
    }
    
    // Midpoints
    double xmid = 0.5 * (overallBB[0] + overallBB[1]);
    double ymid = 0.5 * (overallBB[2] + overallBB[3]);
    double zmid = 0.5 * (overallBB[4] + overallBB[5]);
    
    // MC -> EC:
    // Compute/set eye, center, up
    cryph::AffPoint center(xmid, ymid, zmid);
    double distEyeCenter = 4 * diameter;
    cryph::AffPoint eye(xmid, ymid, zmid + distEyeCenter);
    cryph::AffVector up = cryph::AffVector::yu;
    ModelView::setEyeCenterUp(eye, center, up);
    
    // EC -> LDS:
    // Specify the initial projection type desired
    ModelView::setProjection(PERSPECTIVE);
    
    // Compute/set ecZpp
    double ecZpp = -(distEyeCenter - (0.5*diameter));
    ModelView::setProjectionPlaneZ(ecZpp);
    
    // Compute/set ecZmin, ecZmax (It is often useful to exaggerate
    //       these limits somewhat to prevent unwanted depth clipping.)
    double ecZmin, ecZmax;
    
    ecZmin = ecZpp - (1.5 * diameter);
    ecZmax = ecZpp + (0.5 * diameter);
    
    ModelView::setECZminZmax(ecZmin, ecZmax);
}

int main(int argc, char* argv[])
{
    ExtendedController c("MY LIVING ROOM", MVC_USE_DEPTH_BIT);
    c.reportVersions(std::cout);
    
    ShaderIF *sIF = new ShaderIF("shaders/basic.vsh", "shaders/phong.fsh");
    ShaderIF::ShaderSpec glslProgL[] = {
        {"shaders/basic.vsh", GL_VERTEX_SHADER},
        {"shaders/shrinkLittle.gsh", GL_GEOMETRY_SHADER},
        {"shaders/phong.fsh", GL_FRAGMENT_SHADER}};
    ShaderIF::ShaderSpec glslProgM[] = {
        {"shaders/basic.vsh", GL_VERTEX_SHADER},
        {"shaders/shrinkMore.gsh", GL_GEOMETRY_SHADER},
        {"shaders/phong.fsh", GL_FRAGMENT_SHADER}};
    
    ShaderIF *sIFwGeoL = new ShaderIF(glslProgL, 3);
    ShaderIF *sIFwGeoM = new ShaderIF(glslProgM, 3);
    createScene(c, sIF, sIFwGeoL, sIFwGeoM);
    
    glClearColor(1.0, 1.0, 1.0, 1.0);
    
    double xyz[6];
    c.getOverallMCBoundingBox(xyz);
    std::cout << "Bounding box: " << xyz[0] << " <= x <= " << xyz[1] << '\n';
    std::cout << "              " << xyz[2] << " <= y <= " << xyz[3] << '\n';
    std::cout << "              " << xyz[4] << " <= z <= " << xyz[5] << "\n\n";
    set3DViewingInformation(xyz);
    
    c.run();
    
    delete sIF;
    
    return 0;
}
