// ExtendedController.h

#ifndef EXTENDEDCONTROLLER_H
#define EXTENDEDCONTROLLER_H

#include "GLFWController.h"

class ExtendedController : public GLFWController
{
protected:
    void handleMouseMotion(int x, int y);
    void handleDisplay();
    bool drawingOpaqueObjects;
    
public:
    ExtendedController(const std::string &name, int rcFlags = 0);
    bool drawingOpaque() const;
};

#endif
