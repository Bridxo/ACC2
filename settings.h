#ifndef SETTINGS_H
#define SETTINGS_H

#include <QMatrix4x4>

class Settings
{
public:
    Settings();

    bool modelLoaded;
    bool wireframeMode;
    bool limitPosition;
    bool showSurfacePatch;
    bool showControlMesh;
    bool showGregoryPatch;

    float FoV;
    float dispRatio;
    float rotAngle;

    bool uniformUpdateRequired;
    bool uniformLimitUpdateRequired;
    bool uniformTesUpdateRequired;
    bool uniformGregUpdateRequired;

    QMatrix4x4 modelViewMatrix, projectionMatrix;
    QMatrix3x3 normalMatrix;
    int tess_level;
};

#endif // SETTINGS_H
