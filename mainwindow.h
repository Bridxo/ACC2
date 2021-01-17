#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "objfile.h"
#include <QFileDialog>
#include "mesh.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {

  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

  QVector<Mesh> meshes;
  void importOBJ();

private slots:
  void on_ImportOBJ_clicked();
  void on_SubdivSteps_valueChanged(int value);
  void on_LimitPos_toggled(bool checked);
  void on_SurfacePatch_toggled(bool checked);
  void on_RenderMesh_toggled(bool checked);
  void on_GregoryPatch_toggled(bool checked);
  void on_DiffColors_toggled(bool checked);
  void on_Edges_toggled(bool checked);

  void on_Tess_level_valueChanged(int value);

  void on_checkBox_toggled(bool checked);

private:
  Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
