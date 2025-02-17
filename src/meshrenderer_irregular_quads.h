#ifndef MESHRENDERER_IRREGULAR_QUADS_H
#define MESHRENDERER_IRREGULAR_QUADS_H

#include <QOpenGLShaderProgram>

#include "renderer.h"
#include "mesh.h"

class MeshRendererIrregularQuads : public Renderer
{
public:
    MeshRendererIrregularQuads();
    ~MeshRendererIrregularQuads();

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
    GLint uniModelViewMatrix, uniProjectionMatrix, uniNormalMatrix;
};

#endif // MESHRENDERER_IRREGULAR_QUADS_H
