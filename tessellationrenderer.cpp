#include "tessellationrenderer.h"

TessellationRenderer::TessellationRenderer()
{
    meshIBOSize = 0;
}

TessellationRenderer::~TessellationRenderer() { // regular quads
    gl->glDeleteVertexArrays(1, &vao);

    gl->glDeleteBuffers(1, &meshCoordsBO);
    gl->glDeleteBuffers(1, &meshNormalsBO);
    gl->glDeleteBuffers(1, &meshIndexBO);
}

void TessellationRenderer::init(QOpenGLFunctions_4_1_Core* f, Settings* s, bool colors) {
    gl = f;
    settings = s;

    initShaders(colors);
    initBuffers();
}

void TessellationRenderer::initShaders(bool colors) {
    shaderProg.create();

    shaderProg.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vertshader_surface.glsl");
    shaderProg.addShaderFromSourceFile(QOpenGLShader::TessellationControl, ":/shaders/tcs.glsl");
    shaderProg.addShaderFromSourceFile(QOpenGLShader::TessellationEvaluation, ":/shaders/tes.glsl");

    if (colors){
        shaderProg.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fragshader_surface.glsl");

    } else {
        shaderProg.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fragshader_acc2_surface.glsl");

    }

    shaderProg.link();

    uniModelViewMatrix = gl->glGetUniformLocation(shaderProg.programId(), "modelviewmatrix");
    uniProjectionMatrix = gl->glGetUniformLocation(shaderProg.programId(), "projectionmatrix");
    uniNormalMatrix = gl->glGetUniformLocation(shaderProg.programId(), "normalmatrix");

    uniInnerLevel = gl->glGetUniformLocation(shaderProg.programId(), "innerlevel");
    uniOuterLevel = gl->glGetUniformLocation(shaderProg.programId(), "outerlevel");

    shaderEdgesProg.create();

    shaderEdgesProg.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vertshader_surface.glsl");
    shaderEdgesProg.addShaderFromSourceFile(QOpenGLShader::TessellationControl, ":/shaders/tcs.glsl");
    shaderEdgesProg.addShaderFromSourceFile(QOpenGLShader::TessellationEvaluation, ":/shaders/tes.glsl");
    shaderEdgesProg.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fragshaderEdges.glsl");

    shaderEdgesProg.link();

}

void TessellationRenderer::initBuffers() {

    //create vao
    gl->glGenVertexArrays(1, &vao);
    //bind vao
    gl->glBindVertexArray(vao);

    //generate single buffer object
    gl->glGenBuffers(1, &meshCoordsBO);
    //bind it
    gl->glBindBuffer(GL_ARRAY_BUFFER, meshCoordsBO);
    //enable vertex attribute array with index 0 in the currently bound vao
    gl->glEnableVertexAttribArray(0);
    //tell the currently bound vao what the layout is of the meshCoordsBO
    gl->glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

    //unbind
    gl->glBindVertexArray(0);

}

void TessellationRenderer::updateBuffers(Mesh& currentMesh) {

//    qDebug() << ".. updateBuffers";

    //gather attributes for current mesh
    currentMesh.extractAttributes();
    QVector<QVector3D>& vertexSurfaceCoords = currentMesh.getVertexSurfaceCoords(); //to test

//    qDebug() << "controlPointIndices size" << controlPointIndices.size();

    gl->glBindBuffer(GL_ARRAY_BUFFER, meshCoordsBO);
    gl->glBufferData(GL_ARRAY_BUFFER, sizeof(QVector3D)*vertexSurfaceCoords.size(), vertexSurfaceCoords.data(), GL_DYNAMIC_DRAW);

    qDebug() << " → Updated meshCoordsBO for regular quad tessellation";

    meshIBOSize = vertexSurfaceCoords.size();
}

void TessellationRenderer::updateUniforms() {
    gl->glUniformMatrix4fv(uniModelViewMatrix, 1, false, settings->modelViewMatrix.data());
    gl->glUniformMatrix4fv(uniProjectionMatrix, 1, false, settings->projectionMatrix.data());
    gl->glUniformMatrix3fv(uniNormalMatrix, 1, false, settings->normalMatrix.data());

    gl->glUniform1f(uniInnerLevel, float(settings->tess_level)); // for the inner tessellation
    gl->glUniform1f(uniOuterLevel, float(settings->tess_level)); // for the outer tessellation
}

void TessellationRenderer::draw() {

    shaderProg.bind();

    if (settings->uniformTesUpdateRequired) {
        updateUniforms();
        settings->uniformTesUpdateRequired = false;
    }

    gl->glBindVertexArray(vao);

    // set number of input vertices to 16
    gl->glPatchParameteri(GL_PATCH_VERTICES, 16);
    gl->glDrawArrays(GL_PATCHES, 0, meshIBOSize);
    gl->glBindVertexArray(0);

    shaderProg.release();

}

void TessellationRenderer::drawEdges() {

    shaderEdgesProg.bind();

    if (settings->uniformTesUpdateRequired) {
        updateUniforms();
        settings->uniformTesUpdateRequired = false;
    }

    gl->glBindVertexArray(vao);

    // set number of input vertices to 16
    gl->glPatchParameteri(GL_PATCH_VERTICES, 16);
    gl->glDrawArrays(GL_PATCHES, 0, meshIBOSize);
    gl->glBindVertexArray(0);

    shaderEdgesProg.release();

}
