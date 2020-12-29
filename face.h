#ifndef FACE
#define FACE

#include <QVector3D>

// Forward declaration
class HalfEdge;

class Face {

public:
  HalfEdge* side;
  unsigned short val;
  unsigned int index;
  QVector3D normal;

  // Inline constructors

  Face() {
    side = nullptr;
    val = 0;
    index = 0;
    normal = QVector3D();
  }

  Face(HalfEdge* fside, unsigned short fval, unsigned int findex) {
    side = fside;
    val = fval;
    index = findex;
    normal = QVector3D();
  }
};

#endif // FACE
