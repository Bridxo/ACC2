#version 410
// Vertex shader

layout (location = 0) in vec3 vertcoords_vs;

out vec3 vertcoords_cs;


void main() {

   vertcoords_cs = vertcoords_vs;
}
