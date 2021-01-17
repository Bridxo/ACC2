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
  ui->MainDisplay->settings.showGregoryPatch = false;
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

  ui->GregoryPatch->setEnabled(true);
  ui->GregoryPatch->setChecked(false);

  ui->RenderMesh->setEnabled(true);
  ui->RenderMesh->setChecked(true);

  ui->DiffColors->setEnabled(true);
  ui->DiffColors->setChecked(false);

  ui->Edges->setEnabled(true);
  ui->Edges->setChecked(false);

  ui->checkBox->setEnabled(true);
  ui->checkBox->setChecked(false);
}

void MainWindow::on_SubdivSteps_valueChanged(int value) {
    unsigned short k;
     ui->MainDisplay->settings.subdiv_value = value;
    for (k = meshes.size(); k < value+1; k++) {
        meshes.append(Mesh());
        meshes[k-1].subdivideCatmullClark(meshes[k]);
    }
    ui->MainDisplay->updateBuffers( meshes[value] );

    if(ui->MainDisplay->settings.update_CC){
        ui->MainDisplay->updateBuffers_2( meshes[ ui->MainDisplay->settings.subdiv_value ] );
        ui->ACC2_S_count->display(ui->MainDisplay->settings.subdiv_value);
    }
    else{
        ui->MainDisplay->updateBuffers_2( meshes[ ui->MainDisplay->settings.subdiv_value_2 ] );
        ui->ACC2_S_count->display(ui->MainDisplay->settings.subdiv_value_2);
    }
}

void MainWindow::on_LimitPos_toggled(bool checked){
    ui->MainDisplay->settings.limitPosition = checked;
    ui->MainDisplay->settings.uniformLimitUpdateRequired = true;
    ui->MainDisplay->update();
}

void MainWindow::on_SurfacePatch_toggled(bool checked){
    ui->MainDisplay->settings.showSurfacePatch = checked;
    ui->MainDisplay->settings.uniformTesUpdateRequired = true;
    if(ui->MainDisplay->settings.update_CC){
        ui->MainDisplay->updateBuffers_2( meshes[ ui->MainDisplay->settings.subdiv_value ] );
        ui->ACC2_S_count->display(ui->MainDisplay->settings.subdiv_value);
    }
    else{
        ui->MainDisplay->updateBuffers_2( meshes[ ui->MainDisplay->settings.subdiv_value_2 ] );
        ui->ACC2_S_count->display(ui->MainDisplay->settings.subdiv_value_2);
    }
    ui->MainDisplay->update();
}

void MainWindow::on_RenderMesh_toggled(bool checked){
    ui->MainDisplay->settings.showControlMesh = checked;
    ui->MainDisplay->settings.uniformTesUpdateRequired = true;
    ui->MainDisplay->update();
}


void MainWindow::on_GregoryPatch_toggled(bool checked)
{
    ui->MainDisplay->settings.showGregoryPatch = checked;
    ui->MainDisplay->settings.uniformGregUpdateRequired = true;
    if(ui->MainDisplay->settings.update_CC){
        ui->MainDisplay->updateBuffers_2( meshes[ ui->MainDisplay->settings.subdiv_value ] );
        ui->ACC2_S_count->display(ui->MainDisplay->settings.subdiv_value);
    }
    else{
        ui->MainDisplay->updateBuffers_2( meshes[ ui->MainDisplay->settings.subdiv_value_2 ] );
        ui->ACC2_S_count->display(ui->MainDisplay->settings.subdiv_value_2);
    }
    ui->MainDisplay->update();
}

void MainWindow::on_DiffColors_toggled(bool checked)
{
    ui->MainDisplay->settings.useDifferentColors = checked;
    ui->MainDisplay->settings.uniformUpdateRequired = true;
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
    ui->MainDisplay->settings.uniformGregUpdateRequired = true;
    ui->MainDisplay->settings.uniformTesUpdateRequired = true;
    ui->MainDisplay->update();
}

void MainWindow::on_checkBox_toggled(bool checked) //
{
    ui->MainDisplay->settings.update_CC = checked;
    if(ui->MainDisplay->settings.update_CC){
        ui->MainDisplay->updateBuffers_2( meshes[ ui->MainDisplay->settings.subdiv_value ] );
        ui->ACC2_S_count->display(ui->MainDisplay->settings.subdiv_value);
    }
    else{
        ui->MainDisplay->settings.subdiv_value_2 = ui->MainDisplay->settings.subdiv_value;
        ui->MainDisplay->updateBuffers_2( meshes[ ui->MainDisplay->settings.subdiv_value_2 ] );
        ui->ACC2_S_count->display(ui->MainDisplay->settings.subdiv_value_2);
    }
    ui->MainDisplay->update();
}
