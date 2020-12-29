#include "gregoryrenderer.h"

GregoryRenderer::GregoryRenderer()
{
    meshIBOSize = 0;
}

GregoryRenderer::~GregoryRenderer() {
    gl->glDeleteVertexArrays(1, &vao);

    gl->glDeleteBuffers(1, &meshCoordsBO);
    gl->glDeleteBuffers(1, &meshNormalsBO);
    gl->glDeleteBuffers(1, &meshIndexBO);
}

void GregoryRenderer::init(QOpenGLFunctions_4_1_Core* f, Settings* s) {
    gl = f;
    settings = s;

    initShaders();
    initBuffers();
}

void GregoryRenderer::initShaders() {
    shaderProg.create();

    shaderProg.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vertshader_surface.glsl");
    shaderProg.addShaderFromSourceFile(QOpenGLShader::TessellationControl, ":/shaders/tcs.glsl");
    shaderProg.addShaderFromSourceFile(QOpenGLShader::TessellationEvaluation, ":/shaders/tes.glsl");
    shaderProg.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fragshader_greg_tri.glsl");

    shaderProg.link();

    uniModelViewMatrix = gl->glGetUniformLocation(shaderProg.programId(), "modelviewmatrix");
    uniProjectionMatrix = gl->glGetUniformLocation(shaderProg.programId(), "projectionmatrix");
    uniNormalMatrix = gl->glGetUniformLocation(shaderProg.programId(), "normalmatrix");

    uniInnerLevel = gl->glGetUniformLocation(shaderProg.programId(), "innerlevel");
    uniOuterLevel = gl->glGetUniformLocation(shaderProg.programId(), "outerlevel");

}

void GregoryRenderer::initBuffers() {

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

    gl->glGenBuffers(1, &meshNormalsBO);
    gl->glBindBuffer(GL_ARRAY_BUFFER, meshNormalsBO);
    gl->glEnableVertexAttribArray(1);
    gl->glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);


    gl->glGenBuffers(1, &meshIndexBO);
    gl->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshIndexBO);

    //unbind
    gl->glBindVertexArray(0);

}

void GregoryRenderer::updateBuffers(Mesh& currentMesh) {

    qDebug() << ".. updateBuffers";

    //gather attributes for current mesh
    currentMesh.extractAttributes();
    QVector<QVector3D>& vertexCoords = currentMesh.getVertexLimitCoords();
    QVector<QVector3D>& vertexNormals = currentMesh.getVertexLimitNorms();
    QVector<unsigned int>& controlPointIndices = currentMesh.getControlPointIndices();

    qDebug() << "controlPointIndices size" << controlPointIndices.size();

    gl->glBindBuffer(GL_ARRAY_BUFFER, meshCoordsBO);
    gl->glBufferData(GL_ARRAY_BUFFER, sizeof(QVector3D)*vertexCoords.size(), vertexCoords.data(), GL_DYNAMIC_DRAW);

    qDebug() << " → Updated meshCoordsBO for tessellation";

    gl->glBindBuffer(GL_ARRAY_BUFFER, meshNormalsBO);
    gl->glBufferData(GL_ARRAY_BUFFER, sizeof(QVector3D)*vertexNormals.size(), vertexNormals.data(), GL_DYNAMIC_DRAW);

    qDebug() << " → Updated meshNormalsBO for tessellation";

    gl->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshIndexBO);
    gl->glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*controlPointIndices.size(), controlPointIndices.data(), GL_DYNAMIC_DRAW);

    qDebug() << " → Updated meshIndexBO for tessellation";

    meshIBOSize = controlPointIndices.size();
}

void GregoryRenderer::updateUniforms() {
    gl->glUniformMatrix4fv(uniModelViewMatrix, 1, false, settings->modelViewMatrix.data());
    gl->glUniformMatrix4fv(uniProjectionMatrix, 1, false, settings->projectionMatrix.data());
    gl->glUniformMatrix3fv(uniNormalMatrix, 1, false, settings->normalMatrix.data());

    gl->glUniform1f(uniInnerLevel, float(settings->tess_level)); // for the inner tessellation
    gl->glUniform1f(uniOuterLevel, float(settings->tess_level)); // for the outer tessellation
}

void GregoryRenderer::draw() {

    shaderProg.bind();

    if (settings->uniformGregUpdateRequired) {
        updateUniforms();
        settings->uniformGregUpdateRequired = false;
    }

    //enable primitive restart
    gl->glEnable(GL_PRIMITIVE_RESTART);
    gl->glPrimitiveRestartIndex(INT_MAX);

    gl->glBindVertexArray(vao);

    // set number of input vertices to 16

    // tried to render the regular quads using this, but it doesn't work
    gl->glPatchParameteri(GL_PATCH_VERTICES, 16);
    gl->glDrawElements(GL_PATCHES, meshIBOSize, GL_UNSIGNED_INT, 0);
    gl->glBindVertexArray(0);

    shaderProg.release();

    //disable it again as you might want to draw something else at some point
    gl->glDisable(GL_PRIMITIVE_RESTART);
}


