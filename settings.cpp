#include "settings.h"

Settings::Settings()
{
    modelLoaded = false;
    wireframeMode = true;
    uniformUpdateRequired = true;
    uniformLimitUpdateRequired = true;
    uniformTesUpdateRequired = true;
    uniformGregUpdateRequired = true;
    uniformGreg_tri_UpdateRequired = true;
    limitPosition = false;
    showSurfacePatch = false;
    showControlMesh = false;
    showGregoryPatch = false;

    rotAngle = 0.0;
    dispRatio = 16.0/9.0;
    FoV = 120.0;
    tess_level = 1;
}
