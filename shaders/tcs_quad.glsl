#version 400 core
// Tessellation Control Shader

// define the number of CPs in the output patch
layout(vertices = 20) out;

uniform float innerlevel;
uniform float outerlevel;

//uniform int innerlevel;
//uniform int outerlevel;

in vec3[] vertcoords_cs;

out vec3[] vertcoords_es;

void main() {
    // set inner outer tess level
    if (gl_InvocationID == 0){
    //set inner and outer tessellation levels
        gl_TessLevelInner[ 0 ] = gl_TessLevelInner[ 1 ] = innerlevel;
        gl_TessLevelOuter[ 0 ] = gl_TessLevelOuter[ 1 ] = gl_TessLevelOuter[ 2 ] = gl_TessLevelOuter[ 3 ] = outerlevel;
    }

    vertcoords_es[gl_InvocationID] = vertcoords_cs[gl_InvocationID];

}
