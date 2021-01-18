#include "settings.h"

Settings::Settings()
{
    modelLoaded = false;                //model load check
    wireframeMode = true;               //wireframe mode on/off
    limitPosition = false;              //limit position mode on/off
    showSurfacePatch = false;           //b-spline patch mode on/off
    showControlMesh = false;            //basic mesh mode on/off
    showGregoryPatch = false;           //gregory patch mode on/off
    update_CC = false;                  //Following CC -> ACC2



    //uniformupdates for renderer
    uniformUpdateRequired = true;
    uniformLimitUpdateRequired = true;
    uniformTesUpdateRequired = true;
    uniformGregUpdateRequired = true;

    subdiv_value = 0;       //subdivision level (CC)
    subdiv_value_2 = 0;     //subdivision level (CC -> ACC2)
    rotAngle = 0.0;         //rotation angle
    dispRatio = 16.0/9.0;   //Display ratio
    FoV = 120.0;            //Field of View
    tess_level = 1;         //Tessellation level
}
