#include "meshrenderer.h"

MeshRenderer::MeshRenderer()
{
    meshIBOSize = 0;
}

MeshRenderer::~MeshRenderer() {
    gl->glDeleteVertexArrays(1, &vao);

    gl->glDeleteBuffers(1, &meshCoordsBO);
    gl->glDeleteBuffers(1, &meshNormalsBO);
    gl->glDeleteBuffers(1, &meshLimitCoordsBO);
    gl->glDeleteBuffers(1, &meshLimitNormalsBO);
    gl->glDeleteBuffers(1, &meshIndexBO);
}

void MeshRenderer::init(QOpenGLFunctions_4_1_Core* f, Settings* s) {
    gl = f;
    settings = s;

    initShaders();
    initBuffers();
}

void MeshRenderer::initShaders() {
    shaderProg.create();
    shaderProg.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vertshader.glsl");
    shaderProg.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fragshader.glsl");

    shaderProg.link();

    uniModelViewMatrix = gl->glGetUniformLocation(shaderProg.programId(), "modelviewmatrix");
    uniProjectionMatrix = gl->glGetUniformLocation(shaderProg.programId(), "projectionmatrix");
    uniNormalMatrix = gl->glGetUniformLocation(shaderProg.programId(), "normalmatrix");

    limitShaderProg.create();
    limitShaderProg.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vertshader_limit.glsl");
    limitShaderProg.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fragshader_limit.glsl");

    limitShaderProg.link();
}

void MeshRenderer::initBuffers() {

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

    gl->glGenBuffers(1, &meshLimitCoordsBO);
    gl->glBindBuffer(GL_ARRAY_BUFFER, meshLimitCoordsBO);
    gl->glEnableVertexAttribArray(2);
    gl->glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

    gl->glGenBuffers(1, &meshLimitNormalsBO);
    gl->glBindBuffer(GL_ARRAY_BUFFER, meshLimitNormalsBO);
    gl->glEnableVertexAttribArray(3);
    gl->glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);

    gl->glGenBuffers(1, &meshIndexBO);
    gl->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshIndexBO);

    gl->glBindVertexArray(0);

}

void MeshRenderer::updateBuffers(Mesh& currentMesh) {

    qDebug() << ".. updateBuffers";

    //gather attributes for current mesh
    currentMesh.extractAttributes();
    QVector<QVector3D>& vertexCoords = currentMesh.getVertexCoords();
    QVector<QVector3D>& vertexNormals = currentMesh.getVertexNorms();
    QVector<QVector3D>& vertexLimitCoords = currentMesh.getVertexLimitCoords();
    QVector<QVector3D>& vertexLimitNormals = currentMesh.getVertexLimitNorms();
    QVector<unsigned int>& polyIndices = currentMesh.getPolyIndices();

    gl->glBindBuffer(GL_ARRAY_BUFFER, meshCoordsBO);
    gl->glBufferData(GL_ARRAY_BUFFER, sizeof(QVector3D)*vertexCoords.size(), vertexCoords.data(), GL_DYNAMIC_DRAW);

    qDebug() << " → Updated meshCoordsBO";

    gl->glBindBuffer(GL_ARRAY_BUFFER, meshNormalsBO);
    gl->glBufferData(GL_ARRAY_BUFFER, sizeof(QVector3D)*vertexNormals.size(), vertexNormals.data(), GL_DYNAMIC_DRAW);

    qDebug() << " → Updated meshNormalsBO";

    gl->glBindBuffer(GL_ARRAY_BUFFER, meshLimitCoordsBO);
    gl->glBufferData(GL_ARRAY_BUFFER, sizeof(QVector3D)*vertexLimitCoords.size(), vertexLimitCoords.data(), GL_DYNAMIC_DRAW);

    qDebug() << " → Updated meshLimitCoordsBO";

    gl->glBindBuffer(GL_ARRAY_BUFFER, meshLimitNormalsBO);
    gl->glBufferData(GL_ARRAY_BUFFER, sizeof(QVector3D)*vertexLimitNormals.size(), vertexLimitNormals.data(), GL_DYNAMIC_DRAW);

    qDebug() << " → Updated meshLimitNormalsBO";

    gl->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshIndexBO);
    gl->glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*polyIndices.size(), polyIndices.data(), GL_DYNAMIC_DRAW);

    qDebug() << " → Updated meshIndexBO";

    meshIBOSize = polyIndices.size();
}

void MeshRenderer::updateUniforms() {
    gl->glUniformMatrix4fv(uniModelViewMatrix, 1, false, settings->modelViewMatrix.data());
    gl->glUniformMatrix4fv(uniProjectionMatrix, 1, false, settings->projectionMatrix.data());
    gl->glUniformMatrix3fv(uniNormalMatrix, 1, false, settings->normalMatrix.data());
}

void MeshRenderer::draw() {
    shaderProg.bind();

    if (settings->uniformUpdateRequired) {
        updateUniforms();
        settings->uniformUpdateRequired = false;
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

void MeshRenderer::drawLimitPositions() {
    qDebug() << "***draw limit positions***";

    limitShaderProg.bind();

    if (settings->uniformUpdateRequired) {
        updateUniforms();
        settings->uniformUpdateRequired = false;
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
