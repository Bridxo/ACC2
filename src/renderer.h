#ifndef RENDERER_H
#define RENDERER_H

#include <QOpenGLFunctions_4_1_Core>
#include "settings.h"

class Renderer
{
public:
    Renderer() {
        gl = nullptr;
    }
    Renderer(QOpenGLFunctions_4_1_Core * functions, Settings* settings);
    ~Renderer() {}

//    virtual void initShaders();
//    virtual void initBuffers();

protected:

    QOpenGLFunctions_4_1_Core *gl;
    Settings *settings;
};


#endif // RENDERER_H
