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
    //ver3
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

//    //u,v, -derivatives
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
//    vec3 position = bv0 * ( bu0*vertcoords_es[0] + bu1*vertcoords_es[4] + bu2*vertcoords_es[7] + bu3*vertcoords_es[1] )
//    + bv1 * ( bu0*vertcoords_es[5] + bu1*F0 + bu2*F1 + bu3*vertcoords_es[6] )
//    + bv2 * ( bu0*vertcoords_es[10] + bu1*F3 + bu2*F2 + bu3*vertcoords_es[9] )
//    + bv3 * ( bu0*vertcoords_es[3] + bu1*vertcoords_es[11] + bu2*vertcoords_es[8] + bu3*vertcoords_es[2] );

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

    //ver1
    vec3 position = calcPos(gl_TessCoord.x, gl_TessCoord.y);
//    vec3 position = calPos2(gl_TessCoord.x, gl_TessCoord.y);
//    vec3 position = calPos3(gl_TessCoord.x, gl_TessCoord.y);
//    vec3 position = calPos4(gl_TessCoord.x, gl_TessCoord.y);
//    vec3 position = calPos5(gl_TessCoord.x, gl_TessCoord.y);
//    vec3 position = calPos6(gl_TessCoord.x, gl_TessCoord.y);
//    vec3 position = calPos7(gl_TessCoord.x, gl_TessCoord.y);

    gl_Position = projectionmatrix * modelviewmatrix * vec4(position, 1.0); //finalize value with projection and modelview
}

