#include "mesh.h"
#include<cmath>

HalfEdge* vertOnBoundary(Vertex* currentVertex) {

    unsigned short n = currentVertex->val;
    HalfEdge* currentEdge = currentVertex->out;

    for (unsigned int k = 0; k < n; k++) {
        if (!currentEdge->polygon) {
            return currentEdge;
        }
        currentEdge = currentEdge->prev->twin;
    }

    return nullptr;
}

void Mesh::computeSurfacePatches() {
    qDebug() << "compute surface patches";

    // p3 p2  p7  p6
    // p0 p1  p4  p5
    // p8 p11 p12 15
    // p9 p10 p13 p14

    unsigned int numFaces = faces.size();
    unsigned int k, j, n;

    controlPointIndices.clear();
    controlPointIndices.reserve(numFaces*16);

    int count_quads = 0;

    for (k=0; k<numFaces; k++) {
        Face* currentFace = &faces[k];
        n = currentFace->val;

        // determine patch for regular quads (all vertices have valency 4)
        if (n == 4) {
            //qDebug() << "found a quad, checking vertices valencies...";
             HalfEdge* bottomEdge = currentFace->side;
             HalfEdge* rightEdge = currentFace->side->next;
             HalfEdge* topEdge = currentFace->side->next->next;
             HalfEdge* leftEdge = currentFace->side->next->next->next;
            if ( bottomEdge->target->val == 4 &&
                 rightEdge->target->val == 4 &&
                 topEdge->target->val == 4 &&
                 leftEdge->target->val == 4
                 ) {

                count_quads+=1;

                HalfEdge* currentEdge = leftEdge;

                // find vertices in top-left corner
                currentEdge = currentEdge->twin->next;
                controlPointIndices.append(currentEdge->target->index);
                currentEdge = currentEdge->twin;
                for (j=0; j<3; j++){
                        controlPointIndices.append(currentEdge->target->index);
                        currentEdge = currentEdge->next;
                    }

                    // find vertices in top-right corner
                    currentEdge = topEdge;
                    currentEdge = currentEdge->twin;
                    controlPointIndices.append(currentEdge->target->index);
                    currentEdge = currentEdge->next->twin->next;
                    for (j=0; j<3; j++){
                        controlPointIndices.append(currentEdge->target->index);
                        currentEdge = currentEdge->next;
                    }

                    //find vertices in bottom-left corner
                    currentEdge = leftEdge;
                    currentEdge = currentEdge->twin->next->next->next->twin;
                    controlPointIndices.append(currentEdge->target->index);
                    currentEdge = currentEdge->next;
                    for (j=0; j<3; j++){
                        controlPointIndices.append(currentEdge->target->index);
                        currentEdge = currentEdge->next;
                    }

                    //find vertices in bottom-right corner
                    currentEdge = rightEdge;
                    currentEdge = currentEdge->twin->next->twin;
                    controlPointIndices.append(currentEdge->target->index);
                    currentEdge = currentEdge->next;
                    for (j=0; j<3; j++){
                        controlPointIndices.append(currentEdge->target->index);
                        currentEdge = currentEdge->next;
                    }

            }

        }

    }

    qDebug() << "total number of regular quads is" << count_quads;

    qDebug() << "controlPointIndices size" << controlPointIndices.size();

}
void Mesh::computeSurfacePatches_v2() { // triangle, quad, but execpts boundaries
    qDebug() << "compute gregory patches";

    unsigned int numFaces = faces.size();
    unsigned int k, j, n, i;

    //QUAD Case 20 control points indices 0 - 19
    // p0 p1 p2 p3 e0+ e0- e1+ e1- e2+ e2- e3+ e3- f0+ f0- f1+ f1- f2+ f2- f3+ f3- INT_MAX

    // p0 p5 p10 p3
    // p4 f0 f3 p11
    // p7 f1 f2 p8
    // p1 p6 p9 p2

    GregoryQuadPointIndices.clear();
    GregoryQuadPointIndices.reserve(numFaces*20);

    //Triangle Case 15 control points indices 0 - 14
    // p0 p1 p2 e0+ e0- e1+ e1- e2+ e2- f0+ f0- f1+ f1- f2+ f2- INT_MAX

    GregoryTriPointIndices.clear();
    GregoryTriPointIndices.reserve(numFaces*15);


    for (k=0; k<numFaces; k++) { // along with all faces
        Face* currentFace = &faces[k];
        n = currentFace->val;
        //EDGE Points calculation
        QVector<QVector3D> e;
        e.clear();


        //FACE Points send to Tessellation (calculate only f values not F)
        QVector<QVector3D> f;
        f.clear();


        if (n == 4) {// quads case
            HalfEdge* bottomEdge = currentFace->side;
//            HalfEdge* rightEdge = currentFace->side->next;
//            HalfEdge* topEdge = currentFace->side->next->next;
            HalfEdge* leftEdge = currentFace->side->next->next->next;
            HalfEdge* currentEdge = bottomEdge;

            Vertex* currentVertex = leftEdge->target;
            int currentVertex_ind = currentVertex->index;

            for (int v_ind=0;v_ind<4;v_ind++){
                // find Vertex related mid edge points and face center points
                // this can be optimized with --> set m, c into the vertex class (once per vertex to calculate) but need to know which edge is connected m and c
                QVector<QVector3D> m, c;
                m.reserve(currentVertex->val);
                c.reserve(currentVertex->val);

                for(i=currentVertex->val;i > 0;i--) // looping val times on vertex (one ring neighbor)
                {
                    //calculate Centroid point C (Shape dependancy tri, quad)
                    int shape = currentEdge->polygon->val;
                    HalfEdge* find_centroid = currentEdge;
                    QVector3D centroid = currentVertex->coords;

                    for(int idx = 0;idx<shape-1;idx++)
                    {
                        centroid = centroid + find_centroid->target->coords;
                        find_centroid=find_centroid->next;
                    }
                    //calculate Edge mid point M
                    if(i==currentVertex->val){
                        m[0] = (0.5 * currentVertex->coords + 0.5 * currentEdge->target->coords);
                        c[0] = (centroid/float(shape));
                    }
                    else{
                        m[i] = (0.5 * currentVertex->coords + 0.5 * currentEdge->target->coords);
                        c[i] = (centroid/float(shape));
                    }
                    currentEdge = currentEdge->twin->next; // move to next neighbor edge
                }
                //calculate Edge points
                float lambda = cal_lambda(currentVertex->val);
                QVector<QVector3D> q = cal_q(currentVertex->val,m,c);
                e.append((vertexLimitCoords[currentVertex_ind]) + ((2.0 / 3.0) * lambda * q[0])); //e+
                e.append((vertexLimitCoords[currentVertex_ind]) + ((2.0 / 3.0) * lambda * q[1])); //e-

                //calculate Face points -- need to modify again
                QVector<QVector3D> r = cal_r(m,c);
                f.append(1.0/3.0 * ((c[1] * vertexLimitCoords[currentVertex_ind]) + ((-2.0 * c[0]) - c[1]) * e[0] + (2.0 * c[0] * e[3]) + r[0] + 3.0 * e[0])); //f+
                f.append(1.0/3.0 * ((c[1] * vertexLimitCoords[currentVertex_ind]) + ((-2.0 * c[0]) - c[1]) * e[0] + (2.0 * c[0] * e[3]) + r[0] + 3.0 * e[0])); //f-

                //move to next vertex point
                currentEdge = currentEdge->next;
                currentVertex = currentEdge->target; // move to next vertex on quad
                currentVertex_ind = currentVertex->index;
            }

        }


        else if(n == 3) // triangle case
        {

        }

        else
        {
            qDebug() << "Error --> The mesh includes Some diffrent shape";
        }

    }

    qDebug() << "controlPointIndices size" << controlPointIndices.size();

}
float Mesh:: cal_lambda(int val) // calculate rambda for Edge point
{
    float rambda = 0.0;
     rambda = (1.0/16.0) * (5.0 + cos(2.0*M_PI/float(val)) + cos(M_PI/float(val)) * sqrt(18.0 + (2.0 * cos(2.0*M_PI/float(val)))));
    return rambda;
}

QVector<QVector3D> Mesh:: cal_q(int val, QVector<QVector3D> m, QVector<QVector3D> c) // calculate Q tffor Edge point [0] == + [1] == -
{
    QVector<QVector3D> q;
    q.reserve(2);
    float theta = pow(4 + pow(cos(M_PI/val),2),-0.5);
    for(int i = 0 ; i<val ; i++)
    {
        q[0] = q[0] + (((1.0- theta * cos(M_PI/float(val)))*cos(2.0*float(i)*M_PI/float(val))* m[i])        // q +
              + (2.0 * theta * cos(2.0*float(i)*M_PI/float(val)) * c[i]));
        int q_mod = (i+1) % val; // next neighbor edge (e-)
        q[1] = q[1] + (((1.0- theta * cos(M_PI/float(q_mod)))*cos(2.0*float(i)*M_PI/float(q_mod))* m[i])    // q -
              + (2.0 * theta * cos(2.0*float(i)*M_PI/float(val)) * c[i]));
    }
    q[0] = 2/val * q[0];
    q[1] = 2/val * q[1];
    return q;
}
QVector<QVector3D> Mesh:: cal_r(QVector<QVector3D> m, QVector<QVector3D> c) // calculate r for Face point [0] == + [1] == -
{

    QVector<QVector3D> r;
    r.reserve(2);
    r[0] = (1.0/3.0 * (m[1] - m.last())) + (2.0/3.0 * (c[0] - c.last()));    // r +
    r[1] = (1.0/3.0 * (m[2] - m[0])) + (2.0/3.0 * (c[1] - c[0]));               // r -
    return r;
}

QVector3D Mesh::computeLimitPosition(Vertex* currentVertex) { // corner

    QVector3D newPosition;
    newPosition = QVector3D();

    HalfEdge* currentEdge = currentVertex->out;

    // if currentVertex is on the boundary
    if (HalfEdge* boundaryEdge = vertOnBoundary(currentVertex)) {

        Vertex* p0 = boundaryEdge->prev->target;
        Vertex* p1 = currentVertex;
        Vertex* p2 = boundaryEdge->next->target;

        newPosition  = 1.0 * p0->coords;
        newPosition += 4.0 * p1->coords;
        newPosition += 1.0 * p2->coords;
        newPosition /= 6.0;

    }
    // calculate newPosition for non-boundary vertex
    else {

        //values needed to determine the limit position of non-boundary vertices
        unsigned int n = currentVertex->val;
        QVector3D sum_m_c = QVector3D();

        for (unsigned int k = 0; k < n; k++) {
            QVector3D m = QVector3D(); // mid-point
            QVector3D c = QVector3D(); // center-point
            HalfEdge* edgeInFace = currentEdge; // edge in face to determine c

            m = (currentVertex->coords + currentEdge->target->coords) / 2;

            unsigned int faceValency = currentEdge->polygon->val;

            for (unsigned int j = 0; j < faceValency; j++){
                c += edgeInFace->target->coords;

                edgeInFace = edgeInFace->next;
            }

            c /= faceValency; //center-point

            sum_m_c = sum_m_c + m + c;

            currentEdge = currentEdge->twin->next;
        }


        float firsTerm = (n - 3.0) / (n + 5.0);
        float secondTerm = 4.0 / (n * (n + 5.0));

        newPosition = firsTerm * currentVertex->coords + secondTerm * sum_m_c;

    }

   currentVertex->coords_limit = newPosition; //set limit position for currentVertex

   return newPosition;

}

void Mesh::subdivideCatmullClark(Mesh& mesh) {
    QVector<Vertex>& newVertices = mesh.getVertices();
    QVector<HalfEdge>& newHalfEdges = mesh.getHalfEdges();
    QVector<Face>& newFaces = mesh.getFaces();

    unsigned int numVerts, numHalfEdges, numFaces, sumFaceValences;
    unsigned int k, m, s, t;
    unsigned int vIndex, hIndex, fIndex;
    unsigned short n;
    HalfEdge* currentEdge;

    qDebug() << ":: Creating new Catmull-Clark mesh";

    numVerts = vertices.size();
    numHalfEdges = halfEdges.size();
    numFaces = faces.size();

    // Reserve memory
    newVertices.reserve(numFaces + numVerts + numHalfEdges/2);

    sumFaceValences = 0;
    for (k=0; k<numFaces; k++) {
        sumFaceValences += faces[k].val;
    }

    newHalfEdges.reserve(2*numHalfEdges + 2*sumFaceValences);
    newFaces.reserve(sumFaceValences);

    // Create face points
    for (k=0; k<numFaces; k++) {
        n = faces[k].val;
        // Coords (x,y,z), Out, Valence, Index
        newVertices.append( Vertex(facePoint(&faces[k]),
                                   nullptr,
                                   n,
                                   k) );
    }

    qDebug() << " * Created face points";

    vIndex = numFaces;

    // Create vertex points
    for (k=0; k<numVerts; k++) {
        n = vertices[k].val;
        // Coords (x,y,z), Out, Valence, Index
        newVertices.append( Vertex(vertexPoint(vertices[k].out, newVertices),
                                   nullptr,
                                   n,
                                   vIndex) );
        vIndex++;
    }

    qDebug() << " * Created vertex points";

    // Create edge points
    for (k=0; k<numHalfEdges; k++) {
        currentEdge = &halfEdges[k];

        if (k < currentEdge->twin->index) {
            m = (!currentEdge->polygon || !currentEdge->twin->polygon) ? 3 : 4;
            // Coords (x,y,z), Out, Valence, Index
            newVertices.append( Vertex(edgePoint(currentEdge, newVertices),
                                       nullptr,
                                       m,
                                       vIndex) );
            vIndex++;
        }
    }

    qDebug() << " * Created edge points";


    // Split halfedges
    splitHalfEdges(newVertices, newHalfEdges);

    qDebug() << " * Split halfedges";

    hIndex = 2*numHalfEdges;
    fIndex = 0;

    // Create faces and remaining halfedges
    for (k=0; k<numFaces; k++) {
        currentEdge = faces[k].side;
        n = faces[k].val;

        for (m=0; m<n; m++) {

            s = currentEdge->prev->index;
            t = currentEdge->index;

            // Side, Val, Index
            newFaces.append(Face(nullptr,
                                 4,
                                 fIndex));

            newFaces[fIndex].side = &newHalfEdges[ 2*t ];

            // Target, Next, Prev, Twin, Poly, Index
            newHalfEdges.append(HalfEdge( &newVertices[k],
                                          nullptr,
                                          &newHalfEdges[ 2*t ],
                                nullptr,
                                &newFaces[fIndex],
                                hIndex ));

            newHalfEdges.append(HalfEdge( nullptr,
                                          &newHalfEdges[2*s+1],
                                &newHalfEdges[hIndex],
                                nullptr,
                                &newFaces[fIndex],
                                hIndex+1 ));

            newHalfEdges[hIndex].next = &newHalfEdges[hIndex+1];
            newHalfEdges[hIndex+1].target = newHalfEdges[hIndex+1].next->twin->target;

            newHalfEdges[2*s+1].next = &newHalfEdges[2*t];
            newHalfEdges[2*s+1].prev = &newHalfEdges[hIndex+1];
            newHalfEdges[2*s+1].polygon = &newFaces[fIndex];

            newHalfEdges[2*t].next = &newHalfEdges[hIndex];
            newHalfEdges[2*t].prev = &newHalfEdges[2*s+1];
            newHalfEdges[2*t].polygon = &newFaces[fIndex];

            if (m > 0) {
                // Twins
                newHalfEdges[hIndex+1].twin = &newHalfEdges[hIndex-2];
                newHalfEdges[hIndex-2].twin = &newHalfEdges[hIndex+1];
            }

            // For edge points
            newHalfEdges[2*t].target->out = &newHalfEdges[hIndex];

            hIndex += 2;
            fIndex++;
            currentEdge = currentEdge->next;
        }

        newHalfEdges[hIndex-2*n+1].twin = &newHalfEdges[hIndex-2];
        newHalfEdges[hIndex-2].twin = &newHalfEdges[hIndex-2*n+1];

        // For face points
        newVertices[k].out = &newHalfEdges[hIndex-1];

    }

    qDebug() << " * Created faces and remaining halfedges";

    // For vertex points
    for (k=0; k<numVerts; k++) {
        newVertices[numFaces + k].out = &newHalfEdges[ 2*vertices[k].out->index ];
    }

    qDebug() << " * Completed!";
    qDebug() << "   # Vertices:" << newVertices.size();
    qDebug() << "   # HalfEdges:" << newHalfEdges.size();
    qDebug() << "   # Faces:" << newFaces.size();

}


// ---

QVector3D Mesh::vertexPoint(HalfEdge* firstEdge, QVector<Vertex>& newVertices) {
    unsigned short k;
    QVector3D sumStarPts, sumFacePts;
    QVector3D vertexPt;

    Vertex* currentVertex = firstEdge->twin->target;
    unsigned int n = currentVertex->val;

    sumStarPts = QVector3D();
    sumFacePts = QVector3D();
    HalfEdge* currentEdge = firstEdge;

    // Catmull-Clark (also supporting initial meshes containing n-gons)
    if (HalfEdge* boundaryEdge = vertOnBoundary(currentVertex)) {

        if (boundaryEdge->twin->target->val == 2) {
            // Interpolate corners
            vertexPt = boundaryEdge->twin->target->coords;
        }
        else {
            vertexPt  = 1.0 * boundaryEdge->target->coords;
            vertexPt += 6.0 * boundaryEdge->twin->target->coords;
            vertexPt += 1.0 * boundaryEdge->prev->twin->target->coords;
            vertexPt /= 8.0;
        }
    }
    else {
        for (k = 0; k < n; k++) {
            sumStarPts += currentEdge->target->coords;
            sumFacePts += newVertices[currentEdge->polygon->index].coords;
            currentEdge = currentEdge->prev->twin;
        }

        vertexPt = ((n-2)*currentVertex->coords + sumStarPts/n + sumFacePts/n)/n;
    }

    return vertexPt;

}

QVector3D Mesh::edgePoint(HalfEdge* firstEdge, QVector<Vertex>& newVertices) {
    QVector3D EdgePt = QVector3D();
    HalfEdge* currentEdge = firstEdge;

    // Catmull-Clark (also supporting initial meshes containing n-gons)
    if (!currentEdge->polygon || !currentEdge->twin->polygon) {
        EdgePt  = 4.0 * currentEdge->target->coords;
        EdgePt += 4.0 * currentEdge->twin->target->coords;
        EdgePt /= 8.0;
    }
    else {
        EdgePt  = currentEdge->target->coords;
        EdgePt += currentEdge->twin->target->coords;
        EdgePt += newVertices[currentEdge->polygon->index].coords;
        EdgePt += newVertices[currentEdge->twin->polygon->index].coords;
        EdgePt /= 4.0;
    }

    return EdgePt;

}

QVector3D Mesh::facePoint(Face* f) {
    QVector3D facePt;
    HalfEdge* currentEdge = f->side;

    for (unsigned int k = 0; k < f->val; k++) {
        // General approach
        facePt += 1.0f/float(f->val) * currentEdge->target->coords;
        currentEdge = currentEdge->next;
    }

    return facePt;

}


// For Bilinear, Catmull-Clark and Loop
void Mesh::splitHalfEdges(QVector<Vertex>& newVertices, QVector<HalfEdge>& newHalfEdges) {
    unsigned int k, m;
    unsigned int vIndex;
    HalfEdge* currentEdge;

    vIndex = vertices.size() + faces.size();

    for (k = 0; k < halfEdges.size(); k++) {
        currentEdge = &halfEdges[k];
        m = currentEdge->twin->index;

        // Target, Next, Prev, Twin, Poly, Index
        newHalfEdges.append(HalfEdge(nullptr,
                                     nullptr,
                                     nullptr,
                                     nullptr,
                                     nullptr,
                                     2*k));

        newHalfEdges.append(HalfEdge(nullptr,
                                     nullptr,
                                     nullptr,
                                     nullptr,
                                     nullptr,
                                     2*k+1));

        if (k < m) {
            newHalfEdges[2*k].target = &newVertices[ vIndex ];
            newHalfEdges[2*k+1].target = &newVertices[ faces.size() + currentEdge->target->index ];
            vIndex++;
        }
        else {
            newHalfEdges[2*k].target = newHalfEdges[2*m].target;
            newHalfEdges[2*k+1].target = &newVertices[ faces.size() + currentEdge->target->index ];

            // Assign Twins
            newHalfEdges[2*k].twin = &newHalfEdges[2*m+1];
            newHalfEdges[2*k+1].twin = &newHalfEdges[2*m];
            newHalfEdges[2*m].twin = &newHalfEdges[2*k+1];
            newHalfEdges[2*m+1].twin = &newHalfEdges[2*k];

            // Boundary edges are added last when importing a mesh, so their index will always be higher than their twins.
            if (!currentEdge->polygon) {
                newHalfEdges[2*k].next = &newHalfEdges[2*k+1];
                newHalfEdges[2*k+1].prev = &newHalfEdges[2*k];

                if (currentEdge > currentEdge->next) {
                    m = currentEdge->next->index;
                    newHalfEdges[2*k+1].next = &newHalfEdges[2*m];
                    newHalfEdges[2*m].prev = &newHalfEdges[2*k+1];
                }

                if (currentEdge > currentEdge->prev) {
                    m = currentEdge->prev->index;
                    newHalfEdges[2*k].prev = &newHalfEdges[2*m+1];
                    newHalfEdges[2*m+1].next = &newHalfEdges[2*k];
                }
            }
        }
    }

    // Note that Next, Prev and Poly are not yet assigned at this point.

}
