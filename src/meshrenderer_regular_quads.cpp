#include "meshrenderer_regular_quads.h"

MeshRendererRegularQuads::MeshRendererRegularQuads()
{
    meshIBOSize = 0;
}

MeshRendererRegularQuads::~MeshRendererRegularQuads() { //regular quads in mesh
    gl->glDeleteVertexArrays(1, &vao);

    gl->glDeleteBuffers(1, &meshCoordsBO);
    gl->glDeleteBuffers(1, &meshNormalsBO);
    gl->glDeleteBuffers(1, &meshIndexBO);
}

void MeshRendererRegularQuads::init(QOpenGLFunctions_4_1_Core* f, Settings* s) {
    gl = f;
    settings = s;

    initShaders();
    initBuffers();
}

void MeshRendererRegularQuads::initShaders() {
    shaderProg.create();
    shaderProg.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vertshader.glsl");
    shaderProg.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fragshader_regular_quads.glsl");

    shaderProg.link();

    uniModelViewMatrix = gl->glGetUniformLocation(shaderProg.programId(), "modelviewmatrix");
    uniProjectionMatrix = gl->glGetUniformLocation(shaderProg.programId(), "projectionmatrix");
    uniNormalMatrix = gl->glGetUniformLocation(shaderProg.programId(), "normalmatrix");


}

void MeshRendererRegularQuads::initBuffers() {

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

void MeshRendererRegularQuads::updateBuffers(Mesh& currentMesh) {

//    qDebug() << ".. updateBuffers";

    //gather attributes for current mesh
    currentMesh.extractAttributes();
    QVector<QVector3D>& vertexCoords = currentMesh.getVertexCoords();
    QVector<QVector3D>& vertexNormals = currentMesh.getVertexNorms();

    QVector<unsigned int>& regularQuadIndices = currentMesh.getRegularQuadIndices();

    gl->glBindBuffer(GL_ARRAY_BUFFER, meshCoordsBO);
    gl->glBufferData(GL_ARRAY_BUFFER, sizeof(QVector3D)*vertexCoords.size(), vertexCoords.data(), GL_DYNAMIC_DRAW);

//    qDebug() << " → Updated meshCoordsBO";

    gl->glBindBuffer(GL_ARRAY_BUFFER, meshNormalsBO);
    gl->glBufferData(GL_ARRAY_BUFFER, sizeof(QVector3D)*vertexNormals.size(), vertexNormals.data(), GL_DYNAMIC_DRAW);

//    qDebug() << " → Updated meshNormalsBO";

    gl->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshIndexBO);
    gl->glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*regularQuadIndices.size(), regularQuadIndices.data(), GL_DYNAMIC_DRAW);

//    qDebug() << " → Updated meshIndexBO";

    meshIBOSize = regularQuadIndices.size();
}

void MeshRendererRegularQuads::updateUniforms() {
    //qDebug() << "###update uniforms in regular meshrenderer###";
    gl->glUniformMatrix4fv(uniModelViewMatrix, 1, false, settings->modelViewMatrix.data());
    gl->glUniformMatrix4fv(uniProjectionMatrix, 1, false, settings->projectionMatrix.data());
    gl->glUniformMatrix3fv(uniNormalMatrix, 1, false, settings->normalMatrix.data());
}

void MeshRendererRegularQuads::draw() {
//    qDebug() << "***draw regular quads***";
    shaderProg.bind();

    if (settings->uniformUpdateRequired) {
        updateUniforms();
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

    shaderProg.release();

    //disable it again as you might want to draw something else at some point
    gl->glDisable(GL_PRIMITIVE_RESTART);

}

