#include "gregoryrenderer_tri.h"

GregoryRendererTriangle::GregoryRendererTriangle()
{
    meshIBOSize = 0;
}

GregoryRendererTriangle::~GregoryRendererTriangle() {
    gl->glDeleteVertexArrays(1, &vao);
    gl->glDeleteBuffers(1, &meshCoordsBO);
}

void GregoryRendererTriangle::init(QOpenGLFunctions_4_1_Core* f, Settings* s) {
    gl = f;
    settings = s;

    initShaders();
    initBuffers();
}

void GregoryRendererTriangle::initShaders() {
    shaderProg.create();

//    shaderProg.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vertshader_surface.glsl");
//    shaderProg.addShaderFromSourceFile(QOpenGLShader::TessellationControl, ":/shaders/tcs_quad.glsl");
//    shaderProg.addShaderFromSourceFile(QOpenGLShader::TessellationEvaluation, ":/shaders/tes_quad.glsl");
//    shaderProg.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fragshader_greg_quad.glsl");
    //for debugging
    shaderProg.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vertshader_limit.glsl");
    shaderProg.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fragshader_greg_tri.glsl");

    shaderProg.link();

    uniModelViewMatrix = gl->glGetUniformLocation(shaderProg.programId(), "modelviewmatrix");
    uniProjectionMatrix = gl->glGetUniformLocation(shaderProg.programId(), "projectionmatrix");
    uniNormalMatrix = gl->glGetUniformLocation(shaderProg.programId(), "normalmatrix");
    uniInnerLevel = gl->glGetUniformLocation(shaderProg.programId(), "innerlevel");
    uniOuterLevel = gl->glGetUniformLocation(shaderProg.programId(), "outerlevel");

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

    qDebug() << ".. updateBuffers";

    //gather attributes for current mesh
    currentMesh.extractAttributes();
    QVector<QVector3D>& vertexGregoryTriCoords = currentMesh.getVertexGregoryTriCoords();

    gl->glBindBuffer(GL_ARRAY_BUFFER, meshCoordsBO);
    gl->glBufferData(GL_ARRAY_BUFFER, sizeof(QVector3D)*vertexGregoryTriCoords.size(), vertexGregoryTriCoords.data(), GL_DYNAMIC_DRAW);

    qDebug() << " → Updated meshCoordsBO for tessellation";

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

    if (settings->uniformGreg_tri_UpdateRequired) {
        updateUniforms();
        settings->uniformGreg_tri_UpdateRequired = false;
    }
    qDebug()<<"--> Draw Gregory Tri patches";
    gl->glBindVertexArray(vao);

    // set number of input vertices to 15
//    gl->glPatchParameteri(GL_PATCH_VERTICES, 15);
//    gl->glDrawElements(GL_PATCHES, meshIBOSize, GL_UNSIGNED_INT, 0);
    //for debugging
    gl->glDrawArrays(GL_POINTS,0,9);
    gl->glEnable(GL_PROGRAM_POINT_SIZE);

    gl->glBindVertexArray(0);

    shaderProg.release();

}


