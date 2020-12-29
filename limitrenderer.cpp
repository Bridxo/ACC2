#include "limitrenderer.h"

LimitRenderer::LimitRenderer()
{
    meshIBOSize = 0;
}

LimitRenderer::~LimitRenderer() {
    gl->glDeleteVertexArrays(1, &vao);

    gl->glDeleteBuffers(1, &meshCoordsBO);
    gl->glDeleteBuffers(1, &meshNormalsBO);
    gl->glDeleteBuffers(1, &meshIndexBO);
}

void LimitRenderer::init(QOpenGLFunctions_4_1_Core* f, Settings* s) {
    gl = f;
    settings = s;

    initShaders();
    initBuffers();
}

void LimitRenderer::initShaders() {

    limitShaderProg.create();
    limitShaderProg.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vertshader_limit.glsl");
    limitShaderProg.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fragshader_limit.glsl");

    limitShaderProg.link();

    uniModelViewMatrix = gl->glGetUniformLocation(limitShaderProg.programId(), "modelviewmatrix");
    uniProjectionMatrix = gl->glGetUniformLocation(limitShaderProg.programId(), "projectionmatrix");
    uniNormalMatrix = gl->glGetUniformLocation(limitShaderProg.programId(), "normalmatrix");



}

void LimitRenderer::initBuffers() {

    gl->glGenVertexArrays(1, &vao);
    gl->glBindVertexArray(vao);

    gl->glGenBuffers(1, &meshCoordsBO);
    gl->glBindBuffer(GL_ARRAY_BUFFER, meshCoordsBO);
    gl->glEnableVertexAttribArray(0);
    gl->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    gl->glGenBuffers(1, &meshNormalsBO);
    gl->glBindBuffer(GL_ARRAY_BUFFER, meshNormalsBO);
    gl->glEnableVertexAttribArray(1);
    gl->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

    gl->glGenBuffers(1, &meshIndexBO);
    gl->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshIndexBO);

    gl->glBindVertexArray(0);

}

void LimitRenderer::updateBuffers(Mesh& currentMesh) {

    qDebug() << ".. updateBuffers";

    //gather attributes for current mesh
    currentMesh.extractAttributes();
    QVector<QVector3D>& vertexLimitCoords = currentMesh.getVertexLimitCoords();
    QVector<QVector3D>& vertexLimitNormals = currentMesh.getVertexLimitNorms();

    QVector<unsigned int>& polyIndices = currentMesh.getPolyIndices();

    qDebug() << "polyIndices size" << polyIndices.size();

    gl->glBindBuffer(GL_ARRAY_BUFFER, meshCoordsBO);
    gl->glBufferData(GL_ARRAY_BUFFER, sizeof(QVector3D)*vertexLimitCoords.size(), vertexLimitCoords.data(), GL_DYNAMIC_DRAW);

    qDebug() << " → Updated meshLimitCoordsBO";

    gl->glBindBuffer(GL_ARRAY_BUFFER, meshNormalsBO);
    gl->glBufferData(GL_ARRAY_BUFFER, sizeof(QVector3D)*vertexLimitNormals.size(), vertexLimitNormals.data(), GL_DYNAMIC_DRAW);

    qDebug() << " → Updated meshLimitNormalsBO";

    gl->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshIndexBO);
    gl->glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*polyIndices.size(), polyIndices.data(), GL_DYNAMIC_DRAW);

    qDebug() << " → Updated meshIndexBO";

    meshIBOSize = polyIndices.size();
}

void LimitRenderer::updateUniforms() {
    //qDebug() << "###update uniforms in limitrenderer###";
    gl->glUniformMatrix4fv(uniModelViewMatrix, 1, false, settings->modelViewMatrix.data());
    gl->glUniformMatrix4fv(uniProjectionMatrix, 1, false, settings->projectionMatrix.data());
    gl->glUniformMatrix3fv(uniNormalMatrix, 1, false, settings->normalMatrix.data());
}

void LimitRenderer::draw() {
    //qDebug() << "***draw limit positions***";

    limitShaderProg.bind();

    if (settings->uniformLimitUpdateRequired) {
        updateUniforms();
        settings->uniformLimitUpdateRequired = false;
    }

    //enable primitive restart
    gl->glEnable(GL_PRIMITIVE_RESTART);
    gl->glPrimitiveRestartIndex(INT_MAX);

    gl->glBindVertexArray(vao);

    if (settings->wireframeMode) {
        gl->glDrawElements(GL_LINE_LOOP, meshIBOSize, GL_UNSIGNED_INT, 0);
    } else {
        gl->glDrawElements(GL_TRIANGLE_FAN, meshIBOSize, GL_UNSIGNED_INT, 0);
    }

    gl->glBindVertexArray(0);

    limitShaderProg.release();

    //disable it again as you might want to draw something else at some point
    gl->glDisable(GL_PRIMITIVE_RESTART);
}
