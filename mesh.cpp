#include "mesh.h"
#include "math.h"

Mesh::Mesh() {
    qDebug() << "✓✓ Mesh constructor (Empty)";
}

Mesh::Mesh(OBJFile* loadedOBJFile) {

    qDebug() << "✓✓ Mesh constructor (OBJ)";

    // Convert loaded OBJ file to HalfEdge mesh
    unsigned int numVertices, numHalfEdges, numFaces;
    unsigned int k, m, n;

    numVertices = loadedOBJFile->vertexCoords.size();
    numHalfEdges = 0;

    for (k=0; k<loadedOBJFile->faceValences.size(); k++) {
        numHalfEdges += loadedOBJFile->faceValences[k];
    }

    numFaces = loadedOBJFile->faceValences.size();

    // Note - resize() invokes the Vertex() constructor, reserve() does not.
    vertices.reserve(numVertices);
    // If boundaries are present, reserve twice as much = worst case scenario
    halfEdges.reserve(2*numHalfEdges);
    faces.reserve(numFaces);

    // Add Vertices

    for (k = 0; k < numVertices; k++) {
        // Coords (x,y,z), Out, Valence, Index
        vertices.append(Vertex(loadedOBJFile->vertexCoords[k],
                               nullptr,
                               0,
                               k));
        // Out and valence are unknown at this point.
    }

    qDebug() << "   # Vertices" << vertices.capacity() << vertices.size();

    unsigned int indexH = 0;
    unsigned int currentIndex = 0;

    // Initialize every entry of PotentialTwins with an empty QVector (using resize() )
    //potentialTwins.resize(loadedOBJFile->vertexCoords.size());
    QVector<QVector<unsigned int>> potentialTwins(loadedOBJFile->vertexCoords.size());

    // Add Faces and most of the HalfEdges

    for (m = 0; m < numFaces; m++) {
        // Side, Val, Index
        faces.append(Face(nullptr,
                          loadedOBJFile->faceValences[m],
                          m));

        for (n = 0; n < loadedOBJFile->faceValences[m]; n++) {
            // Target, Next, Prev, Twin, Poly, Index
            halfEdges.append(HalfEdge(&vertices[loadedOBJFile->faceCoordInd[currentIndex+n]],
                    nullptr,
                    nullptr,
                    nullptr,
                    &faces[m],
                    indexH));

            // Next, Prev and Twin of the above HalfEdge have to be assigned later! Starting below...

            if (n > 0) {
                halfEdges[indexH-1].next = &halfEdges[indexH];
                halfEdges[indexH].prev = &halfEdges[indexH-1];

                // Append index of HalfEdge to list of OutgoingHalfEdges of its TailVertex.
                potentialTwins[loadedOBJFile->faceCoordInd[currentIndex+n-1]].append(indexH);
            }
            indexH++;
        }

        // HalfEdges[indexH-1] is the most recent addition.
        faces[m].side = &halfEdges[indexH-1];

        halfEdges[indexH-1].next = &halfEdges[indexH-n];
        halfEdges[indexH-n].prev = &halfEdges[indexH-1];

        potentialTwins[loadedOBJFile->faceCoordInd[currentIndex+n-1]].append(indexH-n);

        currentIndex += loadedOBJFile->faceValences[m];
    }

    qDebug() << "   # Faces" << faces.capacity() << faces.size();
    qDebug() << "   # HalfEdges" << halfEdges.capacity() << halfEdges.size();

    // Outs and Valences of vertices
    for (k = 0; k < vertices.size(); k++) {
        if (potentialTwins[k].size() == 0) {
            qWarning() << " ! Isolated Vertex? PotentialTwins empty for Index" << k;
            dispVertInfo(k);
            continue;
        }
        vertices[k].out = &halfEdges[potentialTwins[k][0]];
        // Not the correct valence when on the boundary! Fixed below.
        vertices[k].val = potentialTwins[k].size();
    }

    setTwins(numHalfEdges, indexH, potentialTwins);


    qDebug() << "   # Updated HalfEdges" << halfEdges.capacity() << halfEdges.size();

}

Mesh::~Mesh() {
    qDebug() << "✗✗ Mesh destructor";

    qDebug() << "   # Vertices:" << vertices.size();
    qDebug() << "   # HalfEdges:" << halfEdges.size();
    qDebug() << "   # Faces:" << faces.size();

    vertices.clear();
    vertices.squeeze();
    halfEdges.clear();
    halfEdges.squeeze();
    faces.clear();
    faces.squeeze();
}

void Mesh::extractAttributes() {
    HalfEdge* currentEdge;

    vertexCoords.clear();
    vertexCoords.reserve(vertices.size());

    for (int k = 0; k < vertices.size(); k++) {
        vertexCoords.append(vertices[k].coords);
    }

    vertexNormals.clear();
    vertexNormals.reserve(vertices.size());
    vertexLimitCoords.clear();
    vertexLimitCoords.reserve(vertices.size());

    for (int k = 0; k < faces.size(); k++) {
        setFaceNormal(&faces[k]);
    }

    for (int k = 0; k < vertices.size(); k++) {
        vertexNormals.append( computeVertexNormal(&vertices[k]) );
    }

     // extract and assign limit position coordinates for all vertices
    for (int k = 0; k < vertices.size(); k++) {
        vertexLimitCoords.append( computeLimitPosition(&vertices[k]) );
        vertexGregoryCoords.append( computeLimitPosition(&vertices[k]) );
        //qDebug() << "append limit coords" << computeLimitPosition(&vertices[k]);
    }

    // extract normals for vertex limit positions
    for (int k = 0; k < vertices.size(); k++) {
        vertexLimitNormals.append( computeVertexLimitNormal(&vertices[k]) );
        vertexGregoryNormals.append( computeVertexLimitNormal(&vertices[k]) );
    }

    polyIndices.clear();
    polyIndices.reserve(halfEdges.size() + faces.size());

    for(int k = 0; k < faces.size(); k++) {
        currentEdge = faces[k].side;
        for (int m = 0; m < faces[k].val; m++) {
            polyIndices.append(currentEdge->target->index);
            currentEdge = currentEdge->next;
        }
        //append MAX_INT to signify end of face
        polyIndices.append(INT_MAX);
    }

    // compute surface patches
    computeSurfacePatches();

    if (controlPointIndices.size() != 0){
        qDebug() << "***Mesh contains regular guads***";

    } else qDebug() << "***There are no guads in the mesh***";

    /*for (unsigned int k=0; k<polyIndices.size();k++){
        qDebug() << "polyIndex is" << polyIndices[k];
    }*/

    /*for (unsigned int k=0; k<controlPointIndices.size();k++){
        qDebug() << "controlPointIndex is" << controlPointIndices[k];
    }*/

}

void Mesh::setTwins(unsigned int numHalfEdges, unsigned int indexH, QVector<QVector<unsigned int>>& potentialTwins) {

    unsigned int m, n;
    unsigned int hTail, hHead, len;
    QSet<unsigned int> twinless;

    // Assign Twins
    for (m = 0; m < numHalfEdges; m++) {
        if (halfEdges[m].twin == nullptr) {
            hTail = halfEdges[m].prev->target->index;
            hHead = halfEdges[m].target->index;
            len = halfEdges[m].target->val;
            for(n = 0; n < len; n++) {
                if (halfEdges[potentialTwins[hHead][n]].target->index == hTail) {
                    //qDebug() << "Found Twin!";
                    halfEdges[m].twin = &halfEdges[potentialTwins[hHead][n]];
                    halfEdges[potentialTwins[hHead][n]].twin = &halfEdges[m];
                    break;
                }
            }
            if (n == len) {
                // Twin not found...
                twinless.insert(m);
            }
        }
    }


    if (twinless.size() > 0) {
        qDebug() << " * There are" << twinless.size() << "HalfEdges without Twin (i.e. the model contains boundaries)";
        // The mesh is not closed

        //qDebug() << Twinless.values();

        HalfEdge* initialEdge;
        HalfEdge* currentEdge;
        unsigned int startBoundaryLoop;

        while (twinless.size() > 0) {
            // Select a HalfEdge without Twin. The Twin that we will create is part of a boundary edge loop
            qDebug() << " → Processing new Boundary Edge Loop";

            initialEdge = &halfEdges[*twinless.begin()];
            twinless.remove(initialEdge->index);

            // Target, Next, Prev, Twin, Poly, Index
            halfEdges.append(HalfEdge( initialEdge->prev->target,
                                       nullptr,
                                       nullptr,
                                       initialEdge,
                                       nullptr,
                                       indexH ));
            startBoundaryLoop = indexH;
            // Twin of initialEdge should be assigned AFTER the central while loop!
            indexH++;

            // Use a sketch to properly understand these steps (assume counter-clockwise HalfEdges) :)
            currentEdge = initialEdge->prev;
            while (currentEdge->twin != nullptr) {
                currentEdge = currentEdge->twin->prev;
            }

            // Trace the current boundary loop
            while (currentEdge != initialEdge) {
                twinless.remove(currentEdge->index);

                // Target, Next, Prev, Twin, Poly, Index
                halfEdges.append(HalfEdge( currentEdge->prev->target,
                                           nullptr,
                                           &halfEdges[indexH-1],
                                 currentEdge,
                                 nullptr,
                                 indexH ));
                halfEdges[indexH-1].next = &halfEdges[indexH];

                currentEdge->target->val += 1;
                currentEdge->twin = &halfEdges[indexH];
                indexH++;

                currentEdge = currentEdge->prev;
                while (currentEdge->twin != nullptr) {
                    currentEdge = currentEdge->twin->prev;
                }
            }

            halfEdges[startBoundaryLoop].prev = &halfEdges[indexH-1];
            halfEdges[indexH-1].next = &halfEdges[startBoundaryLoop];

            initialEdge->target->val += 1;
            // Set Twin of initialEdge!
            initialEdge->twin = &halfEdges[startBoundaryLoop];
        }

    }

}

void Mesh::setFaceNormal(Face* currentFace) {
    QVector3D faceNormal;
    HalfEdge* currentEdge;

    faceNormal = QVector3D();
    currentEdge = currentFace->side;

    for(int k = 0; k < currentFace->val; k++) {
        faceNormal += QVector3D::crossProduct(
                    currentEdge->next->target->coords - currentEdge->target->coords,
                    currentEdge->twin->target->coords - currentEdge->target->coords );
        currentEdge = currentEdge->next;
    }

    currentFace->normal = faceNormal / faceNormal.length();

}


QVector3D Mesh::computeVertexNormal(Vertex* currentVertex) {
    QVector3D vertexNormal;
    HalfEdge* currentEdge;
    float faceAngle;

    vertexNormal = QVector3D();
    currentEdge = currentVertex->out;

    for (int k = 0; k < currentVertex->val; k++) {

        faceAngle = acos( fmax(-1.0, QVector3D::dotProduct(
                                   (currentEdge->target->coords - currentVertex->coords).normalized(),
                                   (currentEdge->prev->twin->target->coords - currentVertex->coords).normalized() ) ) );

        if (currentEdge->polygon) {
            vertexNormal += faceAngle * currentEdge->polygon->normal;
        }

        currentEdge = currentEdge->twin->next;

    }

    return vertexNormal;

}

// compute normal of vertex limit position
QVector3D Mesh::computeVertexLimitNormal(Vertex* currentVertex) {
    QVector3D vertexNormal;
    HalfEdge* currentEdge;
    float faceAngle;

    vertexNormal = QVector3D();
    currentEdge = currentVertex->out;

    for (int k = 0; k < currentVertex->val; k++) {

        faceAngle = acos( fmax(-1.0, QVector3D::dotProduct(
                                   (currentEdge->target->coords_limit - currentVertex->coords_limit).normalized(),
                                   (currentEdge->prev->twin->target->coords_limit - currentVertex->coords_limit).normalized() ) ) );

        if (currentEdge->polygon) {
            vertexNormal += faceAngle * currentEdge->polygon->normal;
        }

        currentEdge = currentEdge->twin->next;

    }

    return vertexNormal;

}

void Mesh::dispVertInfo(unsigned short vertIndex) {
    Vertex* dispVert = &vertices[vertIndex];
    qDebug() << "Vertex at Index =" << dispVert->index << "Coords =" << dispVert->coords << "Out =" << dispVert->out << "Val =" << dispVert->val;
}

void Mesh::dispHalfEdgeInfo(unsigned short edgeIndex) {
    HalfEdge* dispEdge = &halfEdges[edgeIndex];
    qDebug() << "HalfEdge at Index =" << dispEdge->index << "Target =" << dispEdge->target << "Next =" << dispEdge->next << "Prev =" << dispEdge->prev << "Twin =" << dispEdge->twin << "Poly =" << dispEdge->polygon;
}

void Mesh::dispFaceInfo(unsigned short faceIndex){
    Face* dispFace = &faces[faceIndex];
    qDebug() << "Face at Index =" << dispFace->index << "Side =" << dispFace->side << "Val =" << dispFace->val;
}
