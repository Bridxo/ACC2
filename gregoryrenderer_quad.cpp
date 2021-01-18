#include "gregoryrenderer_quad.h"

GregoryRendererQuad::GregoryRendererQuad()
{
    meshIBOSize = 0;
}

GregoryRendererQuad::~GregoryRendererQuad() {
    gl->glDeleteVertexArrays(1, &vao);
    gl->glDeleteBuffers(1, &meshCoordsBO);
}

void GregoryRendererQuad::init(QOpenGLFunctions_4_1_Core* f, Settings* s, bool colors) {
    gl = f;
    settings = s;

    initShaders(colors);
    initBuffers();
}

void GregoryRendererQuad::initShaders(bool colors) {
    shaderProg.create();

    //Gregory Quad vertex -> tessellation -> fragment shader loading
    shaderProg.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vertshader_surface.glsl");
    shaderProg.addShaderFromSourceFile(QOpenGLShader::TessellationControl, ":/shaders/tcs_quad.glsl");
    shaderProg.addShaderFromSourceFile(QOpenGLShader::TessellationEvaluation, ":/shaders/tes_quad.glsl");

    if (colors){ // if coloring mode on colors on quad
        shaderProg.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fragshader_greg_quad.glsl");
    } else {
        shaderProg.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fragshader_acc2_surface.glsl");
    }

    shaderProg.link();

    uniModelViewMatrix = gl->glGetUniformLocation(shaderProg.programId(), "modelviewmatrix");
    uniProjectionMatrix = gl->glGetUniformLocation(shaderProg.programId(), "projectionmatrix");
    uniNormalMatrix = gl->glGetUniformLocation(shaderProg.programId(), "normalmatrix");

    uniInnerLevel = gl->glGetUniformLocation(shaderProg.programId(), "innerlevel");
    uniOuterLevel = gl->glGetUniformLocation(shaderProg.programId(), "outerlevel");

    //Edge shaders
    shaderEdgesProg.create();
    shaderEdgesProg.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vertshader_surface.glsl");
    shaderEdgesProg.addShaderFromSourceFile(QOpenGLShader::TessellationControl, ":/shaders/tcs_quad.glsl");
    shaderEdgesProg.addShaderFromSourceFile(QOpenGLShader::TessellationEvaluation, ":/shaders/tes_quad.glsl");
    shaderEdgesProg.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fragshaderEdges.glsl");

    shaderEdgesProg.link();

}

void GregoryRendererQuad::initBuffers() {

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

void GregoryRendererQuad::updateBuffers(Mesh& currentMesh) {

//    qDebug() << ".. updateBuffers";

    //gather attributes for current mesh
    currentMesh.extractAttributes();
    QVector<QVector3D>& vertexGregoryQuadCoords = currentMesh.getVertexGregoryQuadCoords();
    meshIBOSize = vertexGregoryQuadCoords.size();

//    qDebug() << "meshIBOSize for Gregory Quads" << meshIBOSize;


    gl->glBindBuffer(GL_ARRAY_BUFFER, meshCoordsBO);
    gl->glBufferData(GL_ARRAY_BUFFER, sizeof(QVector3D)*vertexGregoryQuadCoords.size(), vertexGregoryQuadCoords.data(), GL_DYNAMIC_DRAW);

//    qDebug() << " → Updated meshCoordsBO for Gregory quad tessellation";

}

void GregoryRendererQuad::updateUniforms() {
    gl->glUniformMatrix4fv(uniModelViewMatrix, 1, false, settings->modelViewMatrix.data());
    gl->glUniformMatrix4fv(uniProjectionMatrix, 1, false, settings->projectionMatrix.data());
    gl->glUniformMatrix3fv(uniNormalMatrix, 1, false, settings->normalMatrix.data());

    gl->glUniform1f(uniInnerLevel, float(settings->tess_level)); // for the inner tessellation
    gl->glUniform1f(uniOuterLevel, float(settings->tess_level)); // for the outer tessellation
}

void GregoryRendererQuad::draw() {

    shaderProg.bind();

    if (settings->uniformGregUpdateRequired) {
        updateUniforms();
        settings->uniformGregUpdateRequired = false;
    }

    gl->glBindVertexArray(vao);

//     set number of input vertices to 20
    gl->glPatchParameteri(GL_PATCH_VERTICES, 20);
    gl->glDrawArrays(GL_PATCHES, 0, meshIBOSize);

    gl->glBindVertexArray(0);

    shaderProg.release();

}

void GregoryRendererQuad::drawEdges() {

    shaderEdgesProg.bind();

    if (settings->uniformGregUpdateRequired) {
        updateUniforms();
        settings->uniformGregUpdateRequired = false;
    }

    gl->glBindVertexArray(vao);

//     set number of input vertices to 20
    gl->glPatchParameteri(GL_PATCH_VERTICES, 20);
    gl->glDrawArrays(GL_PATCHES, 0, meshIBOSize);

    gl->glBindVertexArray(0);

    shaderEdgesProg.release();

}


