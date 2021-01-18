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
  void on_ImportOBJ_clicked();                  //import obj file
  void on_SubdivSteps_valueChanged(int value);  //CC Subdivision level change
  void on_LimitPos_toggled(bool checked);       //Limit position on/off
  void on_SurfacePatch_toggled(bool checked);   //Limit position with cubic B-spline patch on/off == Regular quads
  void on_RenderMesh_toggled(bool checked);     //CC Mesh on/off
  void on_GregoryPatch_toggled(bool checked);   //Gregory patch on/off
  void on_DiffColors_toggled(bool checked);     //Coloring based on shape on/off
  void on_Edges_toggled(bool checked);          //Edge coloring (on the filled mesh) on/off

  void on_Tess_level_valueChanged(int value);   //Tessellation level slider (1-30) inner==outer level

  void on_checkBox_toggled(bool checked);       //Subdivision following checkbox (checked = follow CC Mesh level / unchecked = follow CC Mesh level before toggle this box)

private:
  Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
