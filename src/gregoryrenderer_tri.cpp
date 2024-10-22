#include "gregoryrenderer_triangle.h"

GregoryRendererTriangle::GregoryRendererTriangle()
{
    meshIBOSize = 0;
}

GregoryRendererTriangle::~GregoryRendererTriangle() {
    gl->glDeleteVertexArrays(1, &vao);
    gl->glDeleteBuffers(1, &meshCoordsBO);
}

void GregoryRendererTriangle::init(QOpenGLFunctions_4_1_Core* f, Settings* s, bool colors) {
    gl = f;
    settings = s;

    initShaders(colors);
    initBuffers();
}

void GregoryRendererTriangle::initShaders(bool colors) {
    shaderProg.create();
    //Gregory Triangle vertex -> tessellation -> fragment shader loading
    shaderProg.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vertshader_surface.glsl");
    shaderProg.addShaderFromSourceFile(QOpenGLShader::TessellationControl, ":/shaders/tcs_tri.glsl");
    shaderProg.addShaderFromSourceFile(QOpenGLShader::TessellationEvaluation, ":/shaders/tes_tri.glsl");

    if (colors){ // if coloring mode on colors on triangle
        shaderProg.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fragshader_greg_tri.glsl");
    } else{
        shaderProg.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fragshader_acc2_surface.glsl");
    }

    shaderProg.link();

    uniModelViewMatrix = gl->glGetUniformLocation(shaderProg.programId(), "modelviewmatrix");
    uniProjectionMatrix = gl->glGetUniformLocation(shaderProg.programId(), "projectionmatrix");
    uniNormalMatrix = gl->glGetUniformLocation(shaderProg.programId(), "normalmatrix");

    uniInnerLevel = gl->glGetUniformLocation(shaderProg.programId(), "innerlevel");
    uniOuterLevel = gl->glGetUniformLocation(shaderProg.programId(), "outerlevel");

    shaderEdgesProg.create();

    //Edge shaders
    shaderEdgesProg.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vertshader_surface.glsl");
    shaderEdgesProg.addShaderFromSourceFile(QOpenGLShader::TessellationControl, ":/shaders/tcs_tri.glsl");
    shaderEdgesProg.addShaderFromSourceFile(QOpenGLShader::TessellationEvaluation, ":/shaders/tes_tri.glsl");
    shaderEdgesProg.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fragshaderEdges.glsl");

    shaderEdgesProg.link();

}

void GregoryRendererTriangle::initBuffers() {

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

void GregoryRendererTriangle::updateBuffers(Mesh& currentMesh) {

    //qDebug() << ".. updateBuffers for Gregory Triangles";

    //gather attributes for current mesh
    currentMesh.extractAttributes();
    QVector<QVector3D>& vertexGregoryTriCoords = currentMesh.getVertexGregoryTriCoords();

    gl->glBindBuffer(GL_ARRAY_BUFFER, meshCoordsBO);
    gl->glBufferData(GL_ARRAY_BUFFER, sizeof(QVector3D)*vertexGregoryTriCoords.size(), vertexGregoryTriCoords.data(), GL_DYNAMIC_DRAW);

    qDebug() << " → Updated meshCoordsBO for Gregory triangle tessellation";

    meshIBOSize = vertexGregoryTriCoords.size();
}

void GregoryRendererTriangle::updateUniforms() {
    gl->glUniformMatrix4fv(uniModelViewMatrix, 1, false, settings->modelViewMatrix.data());
    gl->glUniformMatrix4fv(uniProjectionMatrix, 1, false, settings->projectionMatrix.data());
    gl->glUniformMatrix3fv(uniNormalMatrix, 1, false, settings->normalMatrix.data());

    gl->glUniform1f(uniInnerLevel, float(settings->tess_level)); // for the inner tessellation
    gl->glUniform1f(uniOuterLevel, float(settings->tess_level)); // for the outer tessellation
}

void GregoryRendererTriangle::draw() {

    shaderProg.bind();

    if (settings->uniformGregUpdateRequired) {
        updateUniforms();
        settings->uniformGregUpdateRequired = false;
    }
    gl->glBindVertexArray(vao);

    // set number of input vertices to 15
    gl->glPatchParameteri(GL_PATCH_VERTICES, 15);
    gl->glDrawArrays(GL_PATCHES, 0, meshIBOSize); //meshIBOSize
    gl->glBindVertexArray(0);

    shaderProg.release();

}

void GregoryRendererTriangle::drawEdges() {

    shaderEdgesProg.bind();

    if (settings->uniformGregUpdateRequired) {
        updateUniforms();
        settings->uniformGregUpdateRequired = false;
    }
    gl->glBindVertexArray(vao);

    // set number of input vertices to 15
    gl->glPatchParameteri(GL_PATCH_VERTICES, 15);
    gl->glDrawArrays(GL_PATCHES, 0, meshIBOSize); //meshIBOSize
    gl->glBindVertexArray(0);

    shaderEdgesProg.release();

}


