#version 400 core
//Tessellation Evaluation Shader

layout(quads, equal_spacing, ccw) in ;

in vec3[] vertcoords_es;

uniform mat4 modelviewmatrix;
uniform mat4 projectionmatrix;
uniform mat3 normalmatrix;

float B0(float u) {
    return pow((1.0 - u), 3.0 ) / 6.0;
}

float B1(float u) {
    return (4.0 - 6.0 * pow (u, 2.0) + 3.0 * pow(u,3.0) ) / 6.0;
}

float B2(float u) {
    return (1.0 + 3.0 * u + 3.0 * pow(u, 2.0) - 3.0 * pow(u, 3.0) ) / 6.0;
}

float B3(float u) {
    return pow(u, 3.0) / 6.0;
}

vec3 calcPos(float u, float v) {
    vec3 F0 = (u * vertcoords_es[12] + v * vertcoords_es[13]) / (u + v);
    vec3 F1 = ((1.0 - u) * vertcoords_es[14] + v * vertcoords_es[15]) / (1.0 - u + v);
    vec3 F2 = ((1.0 - u) * vertcoords_es[16] + (1.0 - v) * vertcoords_es[17]) / (2.0 - u - v);
    vec3 F3 = (u * vertcoords_es[18] + (1.0 - v) * vertcoords_es[19]) / (1.0 + u - v);;

    return B0(u) * B0(v) * vertcoords_es[0] +
           B1(u) * B0(v) * vertcoords_es[4] +
           B2(u) * B0(v) * vertcoords_es[7] +
           B3(u) * B0(v) * vertcoords_es[1] +

           B0(u) * B1(v) * vertcoords_es[5] +
           B1(u) * B1(v) * F0 +
           B2(u) * B1(v) * F1 +
           B3(u) * B1(v) * vertcoords_es[6] +

           B0(u) * B2(v) * vertcoords_es[10] +
           B1(u) * B2(v) * F3 +
           B2(u) * B2(v) * F2 +
           B3(u) * B2(v) * vertcoords_es[9] +

           B0(u) * B3(v) * vertcoords_es[3] +
           B1(u) * B3(v) * vertcoords_es[11] +
           B2(u) * B3(v) * vertcoords_es[8] +
           B3(u) * B3(v) * vertcoords_es[2];
     }


void main() {
    vec3 position = calcPos(gl_TessCoord.x, gl_TessCoord.y);
    gl_Position = projectionmatrix * modelviewmatrix * vec4(position, 1.0);
}

