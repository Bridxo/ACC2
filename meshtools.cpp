#define _USE_MATH_DEFINES
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

    vertexSurfaceCoords.clear();
    vertexSurfaceCoords.reserve(numFaces*16);

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
                vertexSurfaceCoords.append(currentEdge->target->coords);
                currentEdge = currentEdge->twin;
                for (j=0; j<3; j++){
                        controlPointIndices.append(currentEdge->target->index);
                        vertexSurfaceCoords.append(currentEdge->target->coords);
                        currentEdge = currentEdge->next;
                    }

                    // find vertices in top-right corner
                    currentEdge = topEdge;
                    currentEdge = currentEdge->twin;
                    controlPointIndices.append(currentEdge->target->index);
                    vertexSurfaceCoords.append(currentEdge->target->coords);
                    currentEdge = currentEdge->next->twin->next;
                    for (j=0; j<3; j++){
                        controlPointIndices.append(currentEdge->target->index);
                        vertexSurfaceCoords.append(currentEdge->target->coords);
                        currentEdge = currentEdge->next;
                    }

                    //find vertices in bottom-left corner
                    currentEdge = leftEdge;
                    currentEdge = currentEdge->twin->next->next->next->twin;
                    controlPointIndices.append(currentEdge->target->index);
                    vertexSurfaceCoords.append(currentEdge->target->coords);
                    currentEdge = currentEdge->next;
                    for (j=0; j<3; j++){
                        controlPointIndices.append(currentEdge->target->index);
                        vertexSurfaceCoords.append(currentEdge->target->coords);
                        currentEdge = currentEdge->next;
                    }

                    //find vertices in bottom-right corner
                    currentEdge = rightEdge;
                    currentEdge = currentEdge->twin->next->twin;
                    controlPointIndices.append(currentEdge->target->index);
                    vertexSurfaceCoords.append(currentEdge->target->coords);
                    currentEdge = currentEdge->next;
                    for (j=0; j<3; j++){
                        controlPointIndices.append(currentEdge->target->index);
                        vertexSurfaceCoords.append(currentEdge->target->coords);
                        currentEdge = currentEdge->next;
                    }

            }

        }

    }

//    qDebug() << "total number of regular quads is" << count_quads;

//    qDebug() << "controlPointIndices size" << controlPointIndices.size();

//    qDebug() << "vertexSurfaceCoords size" << vertexSurfaceCoords.size();

}
void Mesh::computeSurfacePatches_v2() { // triangle, quad, but without boundaries
    qDebug() << "compute gregory patches";

    unsigned int numFaces = faces.size();
    unsigned int k, currentFaceVal;

    //QUAD Case 20 control points indices 0 - 19
    // 0  1  2  3  4   5   6   7   8   9   10  11  12  13  14  15  16  17  18  19
    // p0 p1 p2 p3 e0+ e0- e1+ e1- e2+ e2- e3+ e3- f0+ f0- f1+ f1- f2+ f2- f3+ f3-
    //Patch Structure
    // p0  e0- e3+  p3
    // e0+ F0  F3   e3-
    // e1- F1  F2   e2+
    // p1  e1+ e2-  p2

    vertexGregoryQuadCoords.clear();
    vertexGregoryQuadCoords.reserve(numFaces*20);

    vertexGregoryTriCoords.clear();
    vertexGregoryTriCoords.reserve(numFaces*15);


    for (k=0; k<numFaces; k++) { // along with all faces

        //EDGE Points
        QVector<QVector3D> e;
        e.clear(); //reset for next patch

        //r Points (To calculate face points)
        QVector<QVector3D> r;
        r.clear();

        //FACE Points send to Tessellation (calculate only f values not F)
        QVector<QVector3D> f;
        f.clear(); //reset for next patch

        //Basic variables initialization
        Face* currentFace = &faces[k];
        currentFaceVal = currentFace->val;
        HalfEdge* startEdge = currentFace->side; //indicates edge of face

        //qDebug()<< "face index" << k << "valency is" << currentFaceVal;

        //abnormal case
        if(currentFaceVal != 4 && currentFaceVal != 3) {
//            qDebug()<<"The face at index " << k << "is not a quad nor triangle";
        }

        if( (currentFaceVal == 4 && !(
            startEdge->target->val == 4 &&
            startEdge->next->target->val == 4 &&
            startEdge->next->next->target->val == 4 &&
            startEdge->next->next->next->target->val == 4)
             ) || currentFaceVal == 3 ){
             //qDebug() << "found an irregular quad or triangle";

             //if (currentFace->index == 1) {
                 //quad or triangle case
                 for (int v_ind=0; v_ind< currentFaceVal; v_ind++){
                     // find Vertex related mid edge points and face center points
                     // this can be optimized with --> set m, c into the vertex class (once per vertex to calculate) but need to know which edge is connected m and c

                     //values needed to determine the limit position of non-boundary vertices
                     Vertex* currentVertex = startEdge->target;

                     //FIXME: this turned out to be the primary fix!
//                      taking the out of the vertex is not necesarilly going to be the halfedge that is part of this face!
//                     HalfEdge* currentEdge = currentVertex->out;
                     HalfEdge* currentEdge = startEdge->next;


                     int currentVertex_ind = currentVertex->index;
                     QVector<QVector3D> m, c;
                     m.reserve(currentVertex->val);
                     c.reserve(currentVertex->val);
                     unsigned int n = currentVertex->val;

                     //qDebug() << "***currentVertex index" << currentVertex->index << "valency" << n;

                     for (unsigned int k = 0; k < n; k++) {
                         QVector3D m_coords = QVector3D(); // mid-point
                         QVector3D c_coords = QVector3D(); // center-point
                         HalfEdge* edgeInFace = currentEdge; // edge in face to determine c

                         //qDebug() << "connected edge target index" << currentEdge->target->index;

                         m_coords = (currentVertex->coords + currentEdge->target->coords) / 2;

                         unsigned int faceValency = currentEdge->polygon->val;

                         //qDebug() << "face index to determine c" <<  currentEdge->polygon->index;

                         for (unsigned int j = 0; j < faceValency; j++){
                             c_coords += edgeInFace->target->coords;

                             edgeInFace = edgeInFace->next;
                         }

                         c_coords /= faceValency; //center-point

                         m.append(m_coords);
                         c.append(c_coords);

                         //currentEdge = currentEdge->twin->next;
                         currentEdge = currentEdge->prev->twin;
                     }

                     //qDebug() << "m" << m;
                     //qDebug() << "c" << c;


                     //calculate Edge points
                     //qDebug()<<"calculate Edge points";

                     float lambda = cal_lambda(n);
                     QVector<QVector3D> q = cal_q(n,m,c);
                     e.append((vertexLimitCoords[currentVertex_ind]) + ((2.0 / 3.0) * lambda * q[0])); //e+
                     e.append((vertexLimitCoords[currentVertex_ind]) + ((2.0 / 3.0) * lambda * q[1])); //e-

                     /*qDebug() << "lambda" << lambda;
                     qDebug() << "q[0]" << q[0];
                     qDebug() << "e+" << e[2*v_ind];
                     qDebug() << "e-" << e[2*v_ind+1];
                     */

                     //calculate r values
                     QVector<QVector3D> temp_r = cal_r(m,c);
                     r.append(temp_r[0]);
                     r.append(temp_r[1]);

                     //qDebug() << "r+" << r[2*v_ind];
                     //qDebug() << "r-" << r[2*v_ind+1];

                     //move to next vertex point
                     currentEdge = currentEdge->next;
                     currentVertex = currentEdge->target; // move to next vertex on quad
                     currentVertex_ind = currentVertex->index;

                     startEdge = startEdge->next;


                 } // looping over vertices of the current face

                 //qDebug() << "e" << e;
                 //qDebug() << "r" << r;


            //calculate Face points
            //qDebug() << "####calculate Face points###";
            HalfEdge* currentEdge = currentFace->side; //indicates edge of face

            float d = currentFaceVal==4 ? 3.0 : 4.0; // depends on the shape of the patch
            //int arr_size = currentFaceVal * 2;

            for(int f_ind = 0 ; f_ind < currentFaceVal ; f_ind++){
                //currentVertex_ind change
                //Vertex* currentVertex = currentEdge->target;
                int currentVertex_ind = currentEdge->target->index;
                //qDebug() << "currentVertex at index" << currentVertex_ind;
                //calculate c0, c1
                float n_0 = float(currentEdge->target->val);
                float n_1 = float(currentEdge->next->target->val);
                float c_scaler_0 = cos(2*M_PI/n_0);
                float c_scaler_1 = cos(2*M_PI/n_1);
                //qDebug() << "c0 at for vertex at index" << currentVertex_ind << "with valency" << currentEdge->target->val << "is" << c_scaler_0;
                //qDebug() << "c1 at for vertex at index" << currentEdge->next->target->index << "with valency" << currentEdge->next->target->val << "is" << c_scaler_1;

                //get face points
                //int first_index = (2*f_ind+3)% arr_size;
                //int secont_index = (2*f_ind+6)% arr_size;
                int e0_plus_ind = 2*f_ind;
                int e0_minus_ind = 2*f_ind+1;
                int e1_minus_ind = 2*( (f_ind+1) % currentFaceVal) + 1;
                //FIXME: this turned out to be +3!
//                int e3_plus_ind  = 2*( (f_ind+2) % currentFaceVal);
                int e3_plus_ind  = 2*( (f_ind+3) % currentFaceVal);

                /*qDebug() << "e0_plus at index" << e0_plus_ind << "is" << e[e0_plus_ind];
                qDebug() << "e1_minus at index" << e1_minus_ind << "is" << e[e1_minus_ind];
                qDebug() << "r0" << r[e0_plus_ind];
                qDebug() << "limit position" << vertexLimitCoords[currentVertex_ind];
                qDebug() << "d" << d;

                qDebug() << "f+ intermediate result" << (c_scaler_1 * vertexLimitCoords[currentVertex_ind]) + ((d - (2.0 * c_scaler_0) - c_scaler_1) * e[e0_plus_ind]) + ((2.0 * c_scaler_0) * e[e1_minus_ind]) + r[e0_plus_ind];
                */

                f.append( (1.0/ d) * ( c_scaler_1 * vertexLimitCoords[currentVertex_ind] +
                                       (d - 2.0 * c_scaler_0 - c_scaler_1) * e[e0_plus_ind] +
                                       2.0 * c_scaler_0 * e[e1_minus_ind] +
                                       r[e0_plus_ind]) //f+
                          );
                f.append( (1.0/ d) * ( c_scaler_0 * vertexLimitCoords[currentVertex_ind] +
                                       (d - 2.0 * c_scaler_1 - c_scaler_0) * e[e0_minus_ind] +
                                       2.0 * c_scaler_1 * e[e3_plus_ind] +
                                       r[e0_minus_ind]) //f-
                          );

//                qDebug() << "f values" << f;

                currentEdge = currentEdge->next; //move to next vertex point
            }


//            qDebug() << "####Add coordinates to vertexGregoryQuadCoords";
            //Add coordinates (Corner, Edge and Face points)
            if(currentFaceVal == 4){     // quad

//                qDebug() << "append corner points";
                vertexGregoryQuadCoords.append(vertexLimitCoords[startEdge->target->index]);                     //p0
                vertexGregoryQuadCoords.append(vertexLimitCoords[startEdge->next->target->index]);               //p1
                vertexGregoryQuadCoords.append(vertexLimitCoords[startEdge->next->next->target->index]);         //p2
                vertexGregoryQuadCoords.append(vertexLimitCoords[startEdge->next->next->next->target->index]);   //p3

//                qDebug() << "append edge points";
//                qDebug() << "e" << e;
                vertexGregoryQuadCoords.append(e[0]);                                               //coord add e0+
                vertexGregoryQuadCoords.append(e[1]);                                               //coord add e0-
                vertexGregoryQuadCoords.append(e[2]);                                               //coord add e1+
                vertexGregoryQuadCoords.append(e[3]);                                               //coord add e1-
                vertexGregoryQuadCoords.append(e[4]);                                               //coord add e2+
                vertexGregoryQuadCoords.append(e[5]);                                               //coord add e2-
                vertexGregoryQuadCoords.append(e[6]);                                               //coord add e3+
                vertexGregoryQuadCoords.append(e[7]);                                               //coord add e3-


                vertexGregoryQuadCoords.append(f[0]);                                               //coord add f0+
                vertexGregoryQuadCoords.append(f[1]);                                               //coord add f0-
                vertexGregoryQuadCoords.append(f[2]);                                               //coord add f1+
                vertexGregoryQuadCoords.append(f[3]);                                               //coord add f1-
                vertexGregoryQuadCoords.append(f[4]);                                               //coord add f2+
                vertexGregoryQuadCoords.append(f[5]);                                               //coord add f2-
                vertexGregoryQuadCoords.append(f[6]);                                               //coord add f3+
                vertexGregoryQuadCoords.append(f[7]);                                               //coord add f3-

            }

            else        //triangle
            {


                vertexGregoryTriCoords.append(vertexLimitCoords[startEdge->target->index]);                     //p0
                vertexGregoryTriCoords.append(vertexLimitCoords[startEdge->next->target->index]);               //p1
                vertexGregoryTriCoords.append(vertexLimitCoords[startEdge->next->next->target->index]);         //p2

                vertexGregoryTriCoords.append(e[0]);                                               //coord add e0+
                vertexGregoryTriCoords.append(e[1]);                                               //coord add e0-
                vertexGregoryTriCoords.append(e[2]);                                               //coord add e1+
                vertexGregoryTriCoords.append(e[3]);                                               //coord add e1-
                vertexGregoryTriCoords.append(e[4]);                                               //coord add e2+
                vertexGregoryTriCoords.append(e[5]);                                               //coord add e2-

                vertexGregoryTriCoords.append(f[0]);                                               //coord add f0+
                vertexGregoryTriCoords.append(f[1]);                                               //coord add f0-
                vertexGregoryTriCoords.append(f[2]);                                               //coord add f1+
                vertexGregoryTriCoords.append(f[3]);                                               //coord add f1-
                vertexGregoryTriCoords.append(f[4]);                                               //coord add f2+
                vertexGregoryTriCoords.append(f[5]);                                               //coord add f2-

            }

          //} //test for face 1

        } else {
            //qDebug() << "the current face is part of a regular patch";
        }

    } // Looping on all Faces

//    qDebug() << "Finished calculating Gregory patches";
}


float Mesh:: cal_lambda(int val) // calculate rambda for Edge point
{
    float rambda = 0.0;
     rambda = (1.0/16.0) * (5.0 + cos(2.0*M_PI/float(val)) + cos(M_PI/float(val)) * sqrt(18.0 + (2.0 * cos(2.0*M_PI/float(val)))));
    return rambda;
}

QVector<QVector3D> Mesh:: cal_q(int val, QVector<QVector3D> m, QVector<QVector3D> c) // calculate Q tffor Edge point [0] == + [1] == -
{
    //qDebug() << "call cal_q";
    QVector<QVector3D> q;
    q.reserve(2);
    q.append(QVector3D());
    q.append(QVector3D());

    float theta = pow(4 + pow(cos(M_PI/val),2),-0.5);
    //qDebug() << "theta" <<theta;
    //qDebug() << "M_PI" << M_PI;

    //qDebug() << "starting index for e+" << 0;
    //qDebug() << "starting index for e-" << val-1;

    for(int i = 0 ; i<val ; i++)
    {
        //qDebug() << "m[i]" << m[i];
        //qDebug() << "c[i]" << c[i];
        //qDebug() << "q" << q;
        q[0] = q[0] + ((( 1.0- theta * cos(M_PI / float(val))) * cos(2.0*float(i)*M_PI / float(val))* m[i])
              + (2.0 * theta * cos( (2.0*float(i)*M_PI + M_PI)/float(val)) * c[i]));

        //int q_mod = (i+1) % val; // next neighbor edge (e-)

        //FIXME: I fixed this to point to the next edge and not to the previous edge!
//        int q_mod = (i + val - 1) % val; // next neighbor edge (e-)
        int q_mod = (i + val + 1) % val; // next neighbor edge (e-)

        //qDebug() << "q_mod" <<q_mod;

        //q[1] = q[1] + (((1.0- theta * cos(M_PI/float(val)))*cos(2.0*float(q_mod)*M_PI/float(val))* m[i])    // q -
        //              + (2.0 * theta * cos( (2.0*float(q_mod)*M_PI + M_PI)/float(val)) * c[i]));

        q[1] = q[1] + (((1.0- theta * cos(M_PI/float(val)))*cos(2.0*float(i)*M_PI/float(val))* m[q_mod])    // q -
                      + (2.0 * theta * cos( (2.0*float(i)*M_PI + M_PI)/float(val)) * c[q_mod]));
    }
    q[0] = 2.0/float(val) * q[0];
    q[1] = 2.0/float(val) * q[1];

    //qDebug() << "q values" << q;
    return q;
}
QVector<QVector3D> Mesh:: cal_r(QVector<QVector3D> m, QVector<QVector3D> c) // calculate r for Face point [0] == + [1] == -
{
    //qDebug() << "call cal_r";
    QVector<QVector3D> r;
    r.reserve(2);
    r.append ( (1.0/3.0 * (m[1] - m.last())) + (2.0/3.0 * (c[0] - c.last())));    // r +
    r.append ( (1.0/3.0 * (m[0] - m[2])) + (2.0/3.0 * (c[0] - c[1])));            // r - into patch
    //r.append ( (1.0/3.0 * (m[2] - m[0])) + (2.0/3.0 * (c[1] - c[0])));
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

            //currentEdge = currentEdge->twin->next;
            currentEdge = currentEdge->prev->twin;
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

//    qDebug() << " * Created vertex points";

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

//    qDebug() << " * Created edge points";


    // Split halfedges
    splitHalfEdges(newVertices, newHalfEdges);

//    qDebug() << " * Split halfedges";

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
