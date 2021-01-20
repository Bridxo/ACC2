#ifndef MESH_H
#define MESH_H

#include <QVector>

#include "vertex.h"
#include "face.h"
#include "halfedge.h"

#include "objfile.h"

//#define MAX_INT ((unsigned int) -1)

class Mesh {

public:
    Mesh();
    Mesh(OBJFile *loadedOBJFile);
    ~Mesh();

    inline QVector<Vertex>& getVertices() { return vertices; }
    inline QVector<HalfEdge>& getHalfEdges() { return halfEdges; }
    inline QVector<Face>& getFaces() { return faces; }

    inline QVector<QVector3D>& getVertexCoords() { return vertexCoords; }
    inline QVector<QVector3D>& getVertexSurfaceCoords() { return vertexSurfaceCoords; }
    inline QVector<QVector3D>& getVertexNorms() { return vertexNormals; }
    inline QVector<QVector3D>& getVertexLimitCoords() { return vertexLimitCoords; } //array of vertex limit positions
    inline QVector<QVector3D>& getVertexLimitNorms() { return vertexLimitNormals; } //array of normals of vertex limit positions
    inline QVector<QVector3D>& getVertexGregoryQuadCoords() { return vertexGregoryQuadCoords; }
    inline QVector<QVector3D>& getVertexGregoryTriCoords() { return vertexGregoryTriCoords; }
    inline QVector<unsigned int>& getRegularQuadIndices() { return regularQuadIndices; } //array of indices for regular quads
    inline QVector<unsigned int>& getIrregularQuadIndices() { return irregularQuadIndices; } //array of indices for non-regular quads
    inline QVector<unsigned int>& getTriangleIndices() { return triangleIndices; } //array of indices for triangles
    inline QVector<unsigned int>& getPolyIndices() { return polyIndices; }
    inline QVector<unsigned int>& getControlPointIndices() { return controlPointIndices; };
    void setTwins(unsigned int numHalfEdges, unsigned int indexH, QVector<QVector<unsigned int>>& potentialTwins);

    QVector3D facePoint(Face* firstEdge);
    QVector3D edgePoint(HalfEdge* firstEdge, QVector<Vertex>& newVertices);
    QVector3D vertexPoint(HalfEdge* firstEdge, QVector<Vertex>& newVertices);

    void subdivideLoop(Mesh& mesh);

    void extractAttributes();

    //QVector<QVector<unsigned int> > PotentialTwins;

    void setFaceNormal(Face* currentFace);
    QVector3D computeVertexNormal(Vertex* currentVertex);
    QVector3D computeLimitPosition(Vertex* currentVertex);
    QVector3D computeVertexLimitNormal(Vertex* currentVertex);

    // Limit Patch V1 ' Gregory Patch V2
    void computeSurfacePatches();
    void computeSurfacePatches_v2();

    // For debugging
    void dispVertInfo(unsigned short vertIndex);
    void dispHalfEdgeInfo(unsigned short edgeIndex);
    void dispFaceInfo(unsigned short faceIndex);

    void subdivideCatmullClark(Mesh& mesh);
    void splitHalfEdges(QVector<Vertex>& newVertices, QVector<HalfEdge>& newHalfEdges);

    // Gregory patch calculation
    float cal_lambda(int val);
    QVector<QVector3D> cal_q(int val, QVector<QVector3D> m, QVector<QVector3D> c);
    QVector<QVector3D> cal_r(QVector<QVector3D> m, QVector<QVector3D> c);


private:
    QVector<Vertex> vertices;
    QVector<Face> faces;
    QVector<HalfEdge> halfEdges;

    QVector<QVector3D> vertexCoords;
    QVector<QVector3D> vertexSurfaceCoords;
    QVector<QVector3D> vertexNormals;

    QVector<QVector3D> vertexLimitCoords;
    QVector<QVector3D> vertexLimitNormals;

    QVector<QVector3D> vertexGregoryCoords;
    QVector<QVector3D> vertexGregoryQuadCoords;
    QVector<QVector3D> vertexGregoryTriCoords;
    QVector<QVector3D> vertexGregoryNormals;

    QVector<unsigned int> polyIndices;
    QVector<unsigned int> regularQuadIndices;   //regular quads
    QVector<unsigned int> irregularQuadIndices; //irregular quads
    QVector<unsigned int> triangleIndices;     //triangles
    QVector<unsigned int> controlPointIndices; //Catmull-Clark/regular quads
    QVector<unsigned int> GregoryQuadPointIndices; //Gregory irregular quads
    QVector<unsigned int> GregoryTriPointIndices;  //Gregory triangles

};

#endif // MESH_H
