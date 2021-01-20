#ifndef VERTEX
#define VERTEX

#include <QVector3D>
#include <QDebug>

// Forward declaration
class HalfEdge;

class Vertex {
public:
  QVector3D coords;
  QVector3D coords_limit; // coordinates for the limit position
  HalfEdge* out;
  unsigned short val;
  unsigned int index;
  unsigned short sharpness;

  // Inline constructors
  Vertex() {
    // qDebug() << "Default Vertex Constructor";
    coords = QVector3D();
    coords_limit = QVector3D();
    out = nullptr;
    val = 0;
    index = 0;
    sharpness = 0;
  }

  Vertex(QVector3D vcoords, HalfEdge* vout, unsigned short vval, unsigned int vindex, float vsharpness = 0) {
    //qDebug() << "QVector3D Vertex Constructor";
    coords = vcoords;
    coords_limit = QVector3D();
    out = vout;
    val = vval;
    index = vindex;
    sharpness = vsharpness;
  }

  Vertex(QVector3D vcoords, QVector3D vcoords_limit, HalfEdge* vout, unsigned short vval, unsigned int vindex, float vsharpness = 0) {
    //qDebug() << "QVector3D Vertex Constructor";
    coords = vcoords;
    coords_limit = vcoords_limit;
    out = vout;
    val = vval;
    index = vindex;
    sharpness = vsharpness;
  }
};

#endif // VERTEX
