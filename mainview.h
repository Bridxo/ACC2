#ifndef MAINVIEW_H
#define MAINVIEW_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions_4_1_Core>
#include <QOpenGLDebugLogger>
#include <QOpenGLShaderProgram>
#include <QMouseEvent>

#include "mesh.h"
#include "meshrenderer.h"
#include "meshrenderer_quad.h"
#include "meshrenderer_irregular_quad.h"
#include "meshrenderer_tri.h"
#include "limitrenderer.h"
#include "tessellationrenderer.h"
#include "gregoryrenderer_quad.h"
#include "gregoryrenderer_tri.h"

class MainView : public QOpenGLWidget, protected QOpenGLFunctions_4_1_Core {

  Q_OBJECT

public:
    MainView(QWidget *Parent = 0);
    ~MainView();

    void updateMatrices();
    void updateUniforms();
    void updateBuffers(Mesh& currentMesh);

protected:
    void initializeGL();
    void resizeGL(int newWidth, int newHeight);
    void paintGL();

    void mouseMoveEvent(QMouseEvent* event);
    void mousePressEvent(QMouseEvent* event);
    void wheelEvent(QWheelEvent* event);
    void keyPressEvent(QKeyEvent* event);

private:
  QOpenGLDebugLogger debugLogger;

  QVector2D toNormalizedScreenCoordinates(int x, int y);

  //for zoom
  float scale;
  //for handling rotation
  QVector3D oldVec;
  QQuaternion rotationQuaternion;
  bool dragging;

  MeshRenderer mr;
  MeshRendererRegularQuads regularQuadmr;
  MeshRendererIrregularQuads irregularQuadmr;
  MeshRendererTriangles trianglemr;
  LimitRenderer limr;
  TessellationRenderer tesr;
  GregoryRendererQuad gregQuadr;
  GregoryRendererQuad gregTrir;

  //we make mainwindow a friend so it can access settings and the currentMesh
  friend class MainWindow;
  Mesh mesh;
  Settings settings;

private slots:
  void onMessageLogged( QOpenGLDebugMessage Message );

};

#endif // MAINVIEW_H
