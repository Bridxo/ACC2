#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :  QMainWindow(parent), ui(new Ui::MainWindow) {
  qDebug() << "✓✓ MainWindow constructor";
  ui->setupUi(this);
}

MainWindow::~MainWindow() {
  qDebug() << "✗✗ MainWindow destructor";
  delete ui;
}

void MainWindow::importOBJ() {
  OBJFile newModel = OBJFile(QFileDialog::getOpenFileName(this, "Import OBJ File", "models/", tr("Obj Files (*.obj)")));
  meshes.clear();
  meshes.append( Mesh(&newModel) );

  ui->MainDisplay->updateBuffers( meshes[0]);
  ui->MainDisplay->settings.modelLoaded = true;
  ui->MainDisplay->settings.showControlMesh = true;
  ui->MainDisplay->settings.showSurfacePatch = false;
  ui->MainDisplay->settings.showGregoryQuadPatch = false;
  ui->MainDisplay->settings.showGregoryTriPatch = false;
  ui->MainDisplay->settings.limitPosition = false;
  ui->MainDisplay->settings.useDifferentColors = false;
  ui->MainDisplay->settings.showEdges= false;
  ui->MainDisplay->update();

}

void MainWindow::on_ImportOBJ_clicked() {
  importOBJ();
  ui->SubdivSteps->setEnabled(true);
  ui->SubdivSteps->setValue(0);

  ui->LimitPos->setEnabled(true);
  ui->LimitPos->setChecked(false);

  ui->SurfacePatch->setEnabled(true);
  ui->SurfacePatch->setChecked(false);

  ui->GregoryQuadPatch->setEnabled(true);
  ui->GregoryQuadPatch->setChecked(false);

  ui->GregoryTriPatch->setEnabled(true);
  ui->GregoryTriPatch->setChecked(false);

  ui->RenderMesh->setEnabled(true);
  ui->RenderMesh->setChecked(true);

  ui->DiffColors->setEnabled(true);
  ui->DiffColors->setChecked(false);

  ui->Edges->setEnabled(true);
  ui->Edges->setChecked(false);
}

void MainWindow::on_SubdivSteps_valueChanged(int value) {
    unsigned short k;

    for (k = meshes.size(); k < value+1; k++) {
        meshes.append(Mesh());
        meshes[k-1].subdivideCatmullClark(meshes[k]);
    }

    ui->MainDisplay->updateBuffers( meshes[value] );
}

void MainWindow::on_LimitPos_toggled(bool checked){
    ui->MainDisplay->settings.limitPosition = checked;
    ui->MainDisplay->settings.uniformLimitUpdateRequired = true;
    ui->MainDisplay->update();
}

void MainWindow::on_SurfacePatch_toggled(bool checked){
    ui->MainDisplay->settings.showSurfacePatch = checked;
    ui->MainDisplay->settings.uniformTesUpdateRequired = true;
    ui->MainDisplay->update();
}

void MainWindow::on_RenderMesh_toggled(bool checked){
    ui->MainDisplay->settings.showControlMesh = checked;
    ui->MainDisplay->settings.uniformTesUpdateRequired = true;
    ui->MainDisplay->update();
}


void MainWindow::on_GregoryQuadPatch_toggled(bool checked)
{
    ui->MainDisplay->settings.showGregoryQuadPatch = checked;
    ui->MainDisplay->settings.uniformGregQuadUpdateRequired = true;
    ui->MainDisplay->update();
}


void MainWindow::on_GregoryTriPatch_toggled(bool checked)
{
    ui->MainDisplay->settings.showGregoryTriPatch = checked;
    ui->MainDisplay->settings.uniformGregTriUpdateRequired = true;
    ui->MainDisplay->update();
}

void MainWindow::on_DiffColors_toggled(bool checked)
{
    ui->MainDisplay->settings.useDifferentColors = checked;
    ui->MainDisplay->settings.uniformUpdateRequired = true;
    ui->MainDisplay->settings.uniformGregQuadUpdateRequired = true;
    ui->MainDisplay->settings.uniformGregTriUpdateRequired = true;
    ui->MainDisplay->update();
}

void MainWindow::on_Edges_toggled(bool checked)
{
    ui->MainDisplay->settings.showEdges = checked;
    ui->MainDisplay->settings.uniformEdgesUpdateRequired = true;
    ui->MainDisplay->update();
}

void MainWindow::on_Tess_level_valueChanged(int value)
{
    ui->MainDisplay->settings.tess_level = value;
    ui->MainDisplay->settings.uniformGregQuadUpdateRequired = true;
    ui->MainDisplay->settings.uniformGregTriUpdateRequired = true;
    ui->MainDisplay->settings.uniformTesUpdateRequired = true;
    ui->MainDisplay->update();

    qDebug() << "tess_level set to" << value;
}
