#ifndef MESHRENDERER_H
#define MESHRENDERER_H

#include <QOpenGLShaderProgram>

#include "renderer.h"
#include "mesh.h"

class MeshRenderer : public Renderer
{
public:
    MeshRenderer();
    ~MeshRenderer();

    void init(QOpenGLFunctions_4_1_Core* f, Settings* s);

    void initShaders();
    void initBuffers();

    void updateUniforms();

    void updateBuffers(Mesh& m);
    void draw();
    void drawLimitPositions();

private:

    GLuint vao;
    GLuint meshCoordsBO, meshNormalsBO, meshLimitCoordsBO, meshLimitNormalsBO, meshIndexBO;
    unsigned int meshIBOSize;
    QOpenGLShaderProgram shaderProg, limitShaderProg;

    // Uniforms
    GLint uniModelViewMatrix, uniProjectionMatrix, uniNormalMatrix;
};

#endif // MESHRENDERER_H
