#ifndef GREGORYRENDERER_H
#define GREGORYRENDERER_H

#include <QOpenGLShaderProgram>

#include "renderer.h"
#include "mesh.h"

class GregoryRenderer: public Renderer
{
public:
    GregoryRenderer();
    ~GregoryRenderer();

    void init(QOpenGLFunctions_4_1_Core* f, Settings* s);

    void initShaders();
    void initBuffers();

    void updateUniforms();

    void updateBuffers(Mesh& m);
    void draw();

private:

    GLuint vao;
    GLuint meshCoordsBO, meshNormalsBO, meshIndexBO;
    unsigned int meshIBOSize;
    QOpenGLShaderProgram shaderProg, shaderProg_tri; // quad shader and triangle shader

    // Uniforms
    GLint uniModelViewMatrix, uniProjectionMatrix, uniNormalMatrix, uniInnerLevel, uniOuterLevel;

};

#endif // GREGORYRENDERER_H
