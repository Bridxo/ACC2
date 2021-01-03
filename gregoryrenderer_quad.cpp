#include "gregoryrenderer_quad.h"

GregoryRendererQuad::GregoryRendererQuad()
{
    meshIBOSize = 0;
}

GregoryRendererQuad::~GregoryRendererQuad() {
    gl->glDeleteVertexArrays(1, &vao);
    gl->glDeleteBuffers(1, &meshCoordsBO);
}

void GregoryRendererQuad::init(QOpenGLFunctions_4_1_Core* f, Settings* s) {
    gl = f;
    settings = s;

    initShaders();
    initBuffers();
}

void GregoryRendererQuad::initShaders() {
    shaderProg.create();

    shaderProg.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vertshader_surface.glsl");
    shaderProg.addShaderFromSourceFile(QOpenGLShader::TessellationControl, ":/shaders/tcs_quad.glsl");
    shaderProg.addShaderFromSourceFile(QOpenGLShader::TessellationEvaluation, ":/shaders/tes_quad.glsl");
    shaderProg.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fragshader_greg_quad.glsl");

    shaderProg.link();

    uniModelViewMatrix = gl->glGetUniformLocation(shaderProg.programId(), "modelviewmatrix");
    uniProjectionMatrix = gl->glGetUniformLocation(shaderProg.programId(), "projectionmatrix");
    uniNormalMatrix = gl->glGetUniformLocation(shaderProg.programId(), "normalmatrix");

    uniInnerLevel = gl->glGetUniformLocation(shaderProg.programId(), "innerlevel");
    uniOuterLevel = gl->glGetUniformLocation(shaderProg.programId(), "outerlevel");

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

    gl->glGenBuffers(1, &meshIndexBO);
    gl->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshIndexBO);

    //unbind
    gl->glBindVertexArray(0);

}

void GregoryRendererQuad::updateBuffers(Mesh& currentMesh) {

    qDebug() << ".. updateBuffers";

    //gather attributes for current mesh
    currentMesh.extractAttributes();
    QVector<QVector3D>& vertexGregoryQuadCoords = currentMesh.getVertexGregoryQuadCoords();
    QVector<unsigned int>& controlPointIndices = currentMesh.getGregoryQuadPointIndices();
    gl->glBindBuffer(GL_ARRAY_BUFFER, meshCoordsBO);
    gl->glBufferData(GL_ARRAY_BUFFER, sizeof(QVector3D)*vertexGregoryQuadCoords.size(), vertexGregoryQuadCoords.data(), GL_DYNAMIC_DRAW);

//    gl->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, meshIndexBO);
//    gl->glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int)*controlPointIndices.size(), controlPointIndices.data(), GL_DYNAMIC_DRAW);
    qDebug() << " → Updated meshCoordsBO for Gregory quad tessellation";
    meshIBOSize = vertexGregoryQuadCoords.size();
    qDebug() << "meshIBOSize for Gregory Quads" << meshIBOSize;

    for (int i=0;i<meshIBOSize/20;i++){
        qDebug() << "i" <<i;
        for (int j=0;j<20;j++){
            qDebug() << "quad coords" << vertexGregoryQuadCoords[20*i+j];
        }

    }
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
    qDebug()<<"--> Draw Gregory Quad patches";

    gl->glBindVertexArray(vao);
    // set number of input vertices to 20
    gl->glPatchParameteri(GL_PATCH_VERTICES, 20);
//    gl->glDrawElements(GL_PATCHES, meshIBOSize, GL_UNSIGNED_INT, 0);
    gl->glDrawArrays(GL_PATCHES, 0, meshIBOSize);
    gl->glBindVertexArray(0);

    shaderProg.release();

}


