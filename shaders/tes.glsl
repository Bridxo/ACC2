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
    return B0(v) * B0(u) * vertcoords_es[3] +
           B1(v) * B0(u) * vertcoords_es[2] +
           B2(v) * B0(u) * vertcoords_es[7] +
           B3(v) * B0(u) * vertcoords_es[6] +

           B0(v) * B1(u) * vertcoords_es[0] +
           B1(v) * B1(u) * vertcoords_es[1] +
           B2(v) * B1(u) * vertcoords_es[4] +
           B3(v) * B1(u) * vertcoords_es[5] +

           B0(v) * B2(u) * vertcoords_es[8] +
           B1(v) * B2(u) * vertcoords_es[11] +
           B2(v) * B2(u) * vertcoords_es[12] +
           B3(v) * B2(u) * vertcoords_es[15] +

           B0(v) * B3(u) * vertcoords_es[9] +
           B1(v) * B3(u) * vertcoords_es[10] +
           B2(v) * B3(u) * vertcoords_es[13] +
           B3(v) * B3(u) * vertcoords_es[14];
     }


void main() {
    vec3 position = calcPos(gl_TessCoord.x, gl_TessCoord.y);
    gl_Position = projectionmatrix * modelviewmatrix * vec4(position, 1.0);
}

