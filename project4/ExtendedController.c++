// ExtendedController.c++

#include "ExtendedController.h"
#include "ModelView.h"

ExtendedController::ExtendedController(const std::string &name, int rcFlags) : GLFWController(name, rcFlags)
{
}

void ExtendedController::handleMouseMotion(int x, int y)
{
    // 1. Get a pixel (dx, dy)
    float dx = x - screenBaseX;
    float dy = y - screenBaseY;
    
    // 2. update screenBaseX, screenBaseY
    screenBaseX = x;
    screenBaseY = y;
    
    // 3. if translation, map (dx, dy) to (dxInLDS, dyInLDS) and pass to
    if (mouseMotionIsTranslate == true)
    {
        float dxInLDS = 2 * dx / newWindowWidth;
        float dyInLDS = -2 * dy / newWindowHeight;
        
        ModelView::addToGlobalPan(dxInLDS, dyInLDS, 0);
    }
    else if (mouseMotionIsRotate == true)
    {
        // 4. else if rotation, map (dx, dy) to rotation angles and pass to
        float rotX = dy * 1.38;
        float rotY = dx * 1.38;
        
        ModelView::addToGlobalRotationDegrees(rotX, rotY, 0);
    }
    // 5. do a redraw()
    redraw();
}

void ExtendedController::handleDisplay()
{
    if (theWindow == nullptr)
        return;
    glfwMakeContextCurrent(theWindow);
    int width, height;
    glfwGetFramebufferSize(theWindow, &width, &height);
    glViewport(0, 0, width, height);
    glClear(glClearFlags);
    
    glDepthMask(GL_TRUE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // draw opaque objects
    glDisable(GL_BLEND);
    drawingOpaqueObjects = true; // record in instance variable so ModelView instances can query
    renderAllModels();

    // draw translucent objects
    glDepthMask(GL_FALSE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    drawingOpaqueObjects = false; // record in instance variable so ModelView instances can query
    renderAllModels();

    swapBuffers();
}

bool ExtendedController::drawingOpaque() const // CALLED FROM SceneElement or descendant classes
{
    return drawingOpaqueObjects;
}
