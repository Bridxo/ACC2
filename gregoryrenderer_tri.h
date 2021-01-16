#ifndef GREGORYRENDERER_TRIANGLE_H
#define GREGORYRENDERER_TRIANGLE_H

#include <QOpenGLShaderProgram>

#include "renderer.h"
#include "mesh.h"

class GregoryRendererTriangle: public Renderer
{
public:
    GregoryRendererTriangle();
    ~GregoryRendererTriangle();

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
    QOpenGLShaderProgram shaderProg;

    // Uniforms
    GLint uniModelViewMatrix, uniProjectionMatrix, uniNormalMatrix, uniInnerLevel, uniOuterLevel;

};

#endif // GREGORYRENDERER_TRIANGLE_H
