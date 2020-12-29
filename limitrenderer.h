#ifndef LIMITRENDERER_H
#define LIMITRENDERER_H

#include <QOpenGLShaderProgram>

#include "renderer.h"
#include "mesh.h"

class LimitRenderer : public Renderer
{
public:
    LimitRenderer();
    ~LimitRenderer();

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
    QOpenGLShaderProgram limitShaderProg;

    // Uniforms
    GLint uniModelViewMatrix, uniProjectionMatrix, uniNormalMatrix;
};

#endif // LIMITRENDERER_H
