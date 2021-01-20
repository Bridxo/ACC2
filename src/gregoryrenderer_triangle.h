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

    void init(QOpenGLFunctions_4_1_Core* f, Settings* s, bool colors);

    void initShaders(bool colors);
    void initBuffers();

    void updateUniforms();

    void updateBuffers(Mesh& m);
    void draw();
    void drawEdges();

private:

    GLuint vao;
    GLuint meshCoordsBO, meshNormalsBO, meshIndexBO;
    unsigned int meshIBOSize;
    QOpenGLShaderProgram shaderProg, shaderEdgesProg;

    // Uniforms
    GLint uniModelViewMatrix, uniProjectionMatrix, uniNormalMatrix, uniInnerLevel, uniOuterLevel;

};

#endif // GREGORYRENDERER_TRIANGLE_H
