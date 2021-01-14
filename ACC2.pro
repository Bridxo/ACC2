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
    gregoryrenderer_quad.cpp \
    gregoryrenderer_tri.cpp \
    limitrenderer.cpp \
        mainwindow.cpp \
    meshrenderer.cpp \
    meshrenderer_irregular_quads.cpp \
    meshrenderer_regular_quads.cpp \
    meshrenderer_triangles.cpp \
    objfile.cpp \
    mesh.cpp \
    meshtools.cpp \
    mainview.cpp \
    settings.cpp \
    tessellationrenderer.cpp

HEADERS  += mainwindow.h \
    gregoryrenderer_quad.h \
    gregoryrenderer_triangle.h \
    limitrenderer.h \
    mesh.h \
    meshrenderer.h \
    meshrenderer_irregular_quads.h \
    meshrenderer_regular_quads.h \
    meshrenderer_triangles.h \
    objfile.h \
    renderer.h \
    settings.h \
    tessellationrenderer.h \
    vertex.h \
    halfedge.h \
    face.h \
    mainview.h

FORMS    += mainwindow.ui

RESOURCES += \
    resources.qrc
