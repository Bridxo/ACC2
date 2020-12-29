#ifndef HALFEDGE
#define HALFEDGE

// Forward declarations
class Vertex;
class Face;

class HalfEdge {

public:
  Vertex* target;
  HalfEdge* next;
  HalfEdge* prev;
  HalfEdge* twin;
  Face* polygon;
  unsigned int index;
  unsigned short sharpness;

  // Inline constructors

  HalfEdge() {
    target = nullptr;
    next = nullptr;
    prev = nullptr;
    twin = nullptr;
    polygon = nullptr;
    index = 0;
    sharpness = 0;
  }

  HalfEdge(Vertex* htarget, HalfEdge* hnext, HalfEdge* hprev, HalfEdge* htwin, Face* hpolygon, unsigned int hindex, float hsharpness=0) {
    target = htarget;
    next = hnext;
    prev = hprev;
    twin = htwin;
    polygon = hpolygon;
    index = hindex;
    sharpness = hsharpness;
  }
};

#endif // HALFEDGE
