#ifndef GREGORYRENDERER_QUAD_POINTS_H
#define GREGORYRENDERER_QUAD_POINTS_H

#include <QOpenGLShaderProgram>

#include "renderer.h"
#include "mesh.h"

class GregoryRendererQuadPoints: public Renderer
{
public:
    GregoryRendererQuadPoints();
    ~GregoryRendererQuadPoints();

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

#endif // GREGORYRENDERER_QUAD_POINTS_H
