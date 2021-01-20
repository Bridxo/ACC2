#ifndef TESSELLATIONRENDERER_H
#define TESSELLATIONRENDERER_H

#include <QOpenGLShaderProgram>

#include "renderer.h"
#include "mesh.h"

class TessellationRenderer : public Renderer
{
public:
    TessellationRenderer();
    ~TessellationRenderer();

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

#endif // TESSELLATIONRENDERER_H
