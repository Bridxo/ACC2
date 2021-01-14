#version 410
// Vertex shader

layout (location = 0) in vec3 vertcoords_vs;

//uniform mat4 modelviewmatrix;
//uniform mat4 projectionmatrix;
//uniform mat3 normalmatrix;

out vec3 vertcoords_cs;


void main() {

   vertcoords_cs = vertcoords_vs;
   //gl_Position = projectionmatrix * modelviewmatrix * vec4(vertcoords_vs, 1.0);
}
