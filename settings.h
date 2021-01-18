#ifndef SETTINGS_H
#define SETTINGS_H

#include <QMatrix4x4>

class Settings
{
public:
    Settings();

    bool modelLoaded;                           //model load check
    bool wireframeMode;                         //wireframe mode on/off
    bool limitPosition;                         //limit position mode on/off
    bool showSurfacePatch;                      //b-spline patch mode on/off
    bool showControlMesh;                       //basic mesh mode on/off
    bool showGregoryPatch;                      //gregory patch mode on/off
    bool showGregoryPoints;                     //gregory point mode on/off
    bool useDifferentColors;                    //shape coloring on/off
    bool showEdges;                             //edge mode on/off (only works on wire-frame mode on)
    bool update_CC;                             //Following CC -> ACC2

    float FoV;                                  //Field of View
    float dispRatio;                            //Display ratio
    float rotAngle;                             //rotation angle

    //uniformupdates for renderer
    bool uniformUpdateRequired;
    bool uniformEdgesUpdateRequired;
    bool uniformLimitUpdateRequired;
    bool uniformTesUpdateRequired;
    bool uniformGregUpdateRequired;

    //Basic matrix for QT visualization
    QMatrix4x4 modelViewMatrix, projectionMatrix;
    QMatrix3x3 normalMatrix;

    int tess_level;                            //Tessellation level
    int subdiv_value;                          //subdivision level (CC)
    int subdiv_value_2;                        //subdivision level (CC -> ACC2)
};

#endif // SETTINGS_H
