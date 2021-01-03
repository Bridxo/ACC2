#-------------------------------------------------
#
# Project created by QtCreator 2016-12-08T16:31:55
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ACC2
TEMPLATE = app


SOURCES += main.cpp\
    limitrenderer.cpp \
        mainwindow.cpp \
    meshrenderer.cpp \
    objfile.cpp \
    mesh.cpp \
    meshtools.cpp \
    mainview.cpp \
    settings.cpp \
    tessellationrenderer.cpp \
    gregoryrenderer_quad.cpp \
    gregoryrenderer_tri.cpp \
    meshrenderer_irregular_quad.cpp \
    meshrenderer_quad.cpp \
    meshrenderer_tri.cpp


HEADERS  += mainwindow.h \
    limitrenderer.h \
    mesh.h \
    meshrenderer.h \
    objfile.h \
    renderer.h \
    settings.h \
    tessellationrenderer.h \
    vertex.h \
    halfedge.h \
    face.h \
    mainview.h \
    gregoryrenderer_quad.h \
    gregoryrenderer_tri.h \
    gregoryrenderer_irregular_quad.h \
    meshrenderer_tri.h \
    meshrenderer_quad.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc
