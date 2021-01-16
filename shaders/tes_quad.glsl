#version 400 core
//Tessellation Evaluation Shader

// 0  1  2  3  4   5   6   7   8   9   10  11  12  13  14  15  16  17  18  19
// p0 p1 p2 p3 e0+ e0- e1+ e1- e2+ e2- e3+ e3- f0+ f0- f1+ f1- f2+ f2- f3+ f3-

//Patch Structure
// p3  e3- e2+  p2
// e3+ F3  F2   e2-
// e0- F0  F1   e1+
// p0  e0+ e1-  p1

layout(quads, equal_spacing, ccw) in ;

in vec3[] vertcoords_es;

uniform mat4 modelviewmatrix;
uniform mat4 projectionmatrix;
uniform mat3 normalmatrix;

// Bezier functions
float B0(float u) {
    return pow((1.0 - u), 3.0 );
}

float B1(float u) {
    return 3.0 * pow((1.0 - u), 2.0 ) * u;
}

float B2(float u) {
    return 3.0 * (1.0-u) * pow(u,2.0);
}

float B3(float u) {
    return pow(u, 3.0);
}
vec3 calcPos(float u, float v) {

    //if u,v is in a corner (to avoid zero division)
    if (u==0.0 && v==0.0){
        return vertcoords_es[0];
    } else if (u==0.0 && v==1.0){
        return vertcoords_es[3];
    } else if (u==1.0 && v==0.0){
        return vertcoords_es[1];
    } else if (u==1.0 && v==1.0){
        return vertcoords_es[2];
    }
    else{

        vec3 F0 = (u * vertcoords_es[12] + v * vertcoords_es[13]) / (u + v);
        vec3 F1 = ((1.0 - u) * vertcoords_es[15] + v * vertcoords_es[14]) / (1.0 - u + v);
        vec3 F2 = ((1.0 - u) * vertcoords_es[16] + (1.0 - v) * vertcoords_es[17]) / (2.0 - u - v);
        vec3 F3 = (u * vertcoords_es[19] + (1.0 - v) * vertcoords_es[18]) / (1.0 + u - v);

        return B0(u) * B0(v) * vertcoords_es[0] + //p0
               B1(u) * B0(v) * vertcoords_es[4]+ //e0+
               B2(u) * B0(v) * vertcoords_es[7] + //e1-
               B3(u) * B0(v) * vertcoords_es[1] + //p1

               B0(u) * B1(v) * vertcoords_es[5] + //e0-
               B1(u) * B1(v) * F0 +               //F0
               B2(u) * B1(v) * F1 +               //F1
               B3(u) * B1(v) * vertcoords_es[6]+ //e1+

               B0(u) * B2(v) * vertcoords_es[10] + //e3+
               B1(u) * B2(v) * F3 +               //F1
               B2(u) * B2(v) * F2 +               //F2
               B3(u) * B2(v) * vertcoords_es[9] + //e2-

               B0(u) * B3(v) * vertcoords_es[3] + //p3
               B1(u) * B3(v) * vertcoords_es[11] + //e3-
               B2(u) * B3(v) * vertcoords_es[8] +  //e2+
               B3(u) * B3(v) * vertcoords_es[2];  //p2

    }


}
//vec3 calPos2(float u, float v) {
//    return  (1.0 - u)*(1.0 - v) * vertcoords_es[0] +
//            u*(1.0 - v)         * vertcoords_es[1] +
//            u*v                 * vertcoords_es[2] +
//            (1.0 - u)*v         * vertcoords_es[3];
//}

//vec3 calPos3(float u, float v) {
//    return  (1.0 - u)*(1.0 - v) * vertcoords_es[4] +
//            u*(1.0 - v)         * vertcoords_es[5] +
//            u*v                 * vertcoords_es[6] +
//            (1.0 - u)*v         * vertcoords_es[7];
//}

//vec3 calPos4(float u, float v) {
//    return  (1.0 - u)*(1.0 - v) * vertcoords_es[8] +
//            u*(1.0 - v)         * vertcoords_es[9] +
//            u*v                 * vertcoords_es[10] +
//            (1.0 - u)*v         * vertcoords_es[11];
//}

//vec3 calPos5(float u, float v) {
//    return  (1.0 - u)*(1.0 - v) * vertcoords_es[12] +
//            u*(1.0 - v)         * vertcoords_es[13] +
//            u*v                 * vertcoords_es[14] +
//            (1.0 - u)*v         * vertcoords_es[15];
//}

//vec3 calPos6(float u, float v) {
//    return  (1.0 - u)*(1.0 - v) * vertcoords_es[16] +
//            u*(1.0 - v)         * vertcoords_es[17] +
//            u*v                 * vertcoords_es[18] +
//            (1.0 - u)*v         * vertcoords_es[19];
//}

//vec3 calPos7(float u, float v) {

//    vec3 F0 = (u * vertcoords_es[12] + v * vertcoords_es[13]) / (u + v);
//    vec3 F1 = ((1.0 - u) * vertcoords_es[15] + v * vertcoords_es[14]) / (1.0 - u + v);
//    vec3 F2 = ((1.0 - u) * vertcoords_es[16] + (1.0 - v) * vertcoords_es[17]) / (2.0 - u - v);
//    vec3 F3 = (u * vertcoords_es[19] + (1.0 - v) * vertcoords_es[18]) / (1.0 + u - v);


//    return  (1.0 - u)*(1.0 - v) * F0 +
//            u*(1.0 - v)         * F1 +
//            u*v                 * F2 +
//            (1.0 - u)*v         * F3;
//}

void main() {
////    ver3
//    float u = gl_TessCoord.x;
//    float v = gl_TessCoord.y;
//    vec3 F0 = (u * vertcoords_es[12] + v * vertcoords_es[13]) / (u + v);
//    vec3 F1 = ((1.0 - u) * vertcoords_es[15] + v * vertcoords_es[14]) / (1.0 - u + v);
//    vec3 F2 = ((1.0 - u) * vertcoords_es[16] + (1.0 - v) * vertcoords_es[17]) / (2.0 - u - v);
//    vec3 F3 = (u * vertcoords_es[19] + (1.0 - v) * vertcoords_es[18]) / (1.0 + u - v);

//    //u - basis
//    float bu0 = (1.0-u) * (1.0-u) * (1.0-u);
//    float bu1 = 3.0 * u * (1.0-u) * (1.0-u);
//    float bu2 = 3.0 * u * u * (1.0-u);
//    float bu3 = u * u * u;
//    //v - basis
//    float bv0 = (1.0-v) * (1.0-v) * (1.0-v);
//    float bv1 = 3.0 * v * (1.0-v) * (1.0-v);
//    float bv2 = 3.0 * v * v * (1.0-v);
//    float bv3 = v * v * v;

    //u,v, -derivatives
//    float dbu0 = -3.0 * (1.0-u) * (1.0-u);
//    float dbu1 = 3.0 * (1.0-u) * (1.0-3.0*u);
//    float dbu2 = 3.0 * u * (2.0-3.0*u);
//    float dbu3 = 3.0 * u * u;
//    float dbv0 = -3.0 * (1.0-v) * (1.0-v);
//    float dbv1 = 3.0 * (1.0-v) * (1.0-3.0*v);
//    float dbv2 = 3.0 * v * (2.0-3.0*v);
//    float dbv3 = 3.0 * v * v;

//    // 0  1  2  3  4   5   6   7   8   9   10  11  12  13  14  15  16  17  18  19
//    // p0 p1 p2 p3 e0+ e0- e1+ e1- e2+ e2- e3+ e3- f0+ f0- f1+ f1- f2+ f2- f3+ f3-
//    vec3 position = bu0 * ( bv0*vertcoords_es[0] + bv1*vertcoords_es[4] + bv2*vertcoords_es[7] + bv3*vertcoords_es[1] )
//    + bu1 * ( bv0*vertcoords_es[5] + bv1*F0 + bv2*F1 + bv3*vertcoords_es[6] )
//    + bu2 * ( bv0*vertcoords_es[10] + bv1*F3 + bv2*F2 + bv3*vertcoords_es[9] )
//    + bu3 * ( bv0*vertcoords_es[3] + bv1*vertcoords_es[11] + bv2*vertcoords_es[8] + bv3*vertcoords_es[2] );

//   //ver2
//    vec3 F0,F1,F2,F3;
//    float u = gl_TessCoord.x;
//    float v = gl_TessCoord.y;
//    F0 = (u * vertcoords_es[12] + v * vertcoords_es[13]) / (u + v);
//    F1 = ((1.0 - u) * vertcoords_es[15] + v * vertcoords_es[14]) / (1.0 - u + v);
//    F2 = ((1.0 - u) * vertcoords_es[16] + (1.0 - v) * vertcoords_es[17]) / (2.0 - u - v);
//    F3 = (u * vertcoords_es[19] + (1.0 - v) * vertcoords_es[18]) / (1.0 + u - v);
//    vec4 b_u = vec4(pow((1.0 - u), 3.0 ), 3.0 * pow((1.0 - u), 2.0 ) * u, 3.0 * (1.0-u) * pow(u,2.0),pow(u, 3.0));
//    vec4 b_v = vec4(pow((1.0 - v), 3.0 ), 3.0 * pow((1.0 - v), 2.0 ) * v, 3.0 * (1.0-v) * pow(v,2.0),pow(v, 3.0));
//    //es_to_patch
//    // 0  5 10  3
//    // 4  F0 F3 11
//    // 7  F1 F2 8
//    // 1  6  9  2
//    mat4x4 G = mat4(vertcoords_es[0],vertcoords_es[5],vertcoords_es[10],vertcoords_es[3],
//                    vertcoords_es[4],F0,F3,vertcoords_es[11],
//                    vertcoords_es[7],F1,F2,vertcoords_es[8],
//                    vertcoords_es[1],vertcoords_es[6],vertcoords_es[9],vertcoords_es[2]);
//    vec3 position = b_u * G * b_v;

    vec3 position = calcPos(gl_TessCoord.x, gl_TessCoord.y);
    //    // 0  1  2  3  4   5   6   7   8   9   10  11  12  13  14  15  16  17  18  19
    //    // p0 p1 p2 p3 e0+ e0- e1+ e1- e2+ e2- e3+ e3- f0+ f0- f1+ f1- f2+ f2- f3+ f3-
//        vec3 position = bu0 * ( bv0*vertcoords_es[0] + bv1*vertcoords_es[4] + bv2*vertcoords_es[7] + bv3*vertcoords_es[1] )
//        + bu1 * ( bv0*vertcoords_es[5] + bv1*F0 + bv2*F1 + bv3*vertcoords_es[6] )
//        + bu2 * ( bv0*vertcoords_es[10] + bv1*F3 + bv2*F2 + bv3*vertcoords_es[9] )
//        + bu3 * ( bv0*vertcoords_es[3] + bv1*vertcoords_es[11] + bv2*vertcoords_es[8] + bv3*vertcoords_es[2] );
//    vec4 dpdu = dbu0 * ( bv0*p00 + bv1*p01 + bv2*p02 + bv3*p03 )
//    + dbu1 * ( bv0*p10 + bv1*p11 + bv2*p12 + bv3*p13 )
//    + dbu2 * ( bv0*p20 + bv1*p21 + bv2*p22 + bv3*p23 )
//    + dbu3 * ( bv0*p30 + bv1*p31 + bv2*p32 + bv3*p33 );
//    vec4 dpdv = bu0 * ( dbv0*p00 + dbv1*p01 + dbv2*p02 + dbv3*p03 )
//    + bu1 * ( dbv0*p10 + dbv1*p11 + dbv2*p12 + dbv3*p13 )
//    + bu2 * ( dbv0*p20 + dbv1*p21 + dbv2*p22 + dbv3*p23 )
//    + bu3 * ( dbv0*p30 + dbv1*p31 + dbv2*p32 + dbv3*p33 );
//    teNormal = normalize( cross( dpdu.xyz, dpdv.xyz ) );

    gl_Position = projectionmatrix * modelviewmatrix * vec4(position, 1.0); //finalize value with projection and modelview
}

