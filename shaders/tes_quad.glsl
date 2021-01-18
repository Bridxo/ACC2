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

void main() {

    vec3 position = calcPos(gl_TessCoord.x, gl_TessCoord.y);

    gl_Position = projectionmatrix * modelviewmatrix * vec4(position, 1.0); //finalize value with projection and modelview
}

