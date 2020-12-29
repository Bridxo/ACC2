#version 400 core
//Tessellation Evaluation Shader

layout(triangles, equal_spacing, ccw) in ;

in vec3[] vertcoords_es;

uniform mat4 modelviewmatrix;
uniform mat4 projectionmatrix;
uniform mat3 normalmatrix;


void main() {
    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;
    float w = gl_TessCoord.z;
    vec3 F0,F1,F2;


    F0 = (w * vertcoords_es[10] + v * vertcoords_es[9]) / (v + w);
    F1 = (u * vertcoords_es[12] + w * vertcoords_es[11]) / (u + w);
    F2 = (v * vertcoords_es[14] + u * vertcoords_es[13]) / (v + u);

    vec3 position = pow(u,3.0) * vertcoords_es[0] + pow(v,3.0) * vertcoords_es[1] + pow(w,3.0) * vertcoords_es[2] +
                    3.0 * u * v * (u + v) * (u * vertcoords_es[3] + v * vertcoords_es[6]) +
                    3.0 * v * w * (v + w) * (v * vertcoords_es[5] + v * vertcoords_es[8]) +
                    3.0 * u * v * (w + u) * (w * vertcoords_es[7] + v * vertcoords_es[4]) +
                    12.0 * u * v * w * (u * F0 + v * F1 + w * F2);


    gl_Position = projectionmatrix * modelviewmatrix * vec4(position, 1.0);
}

