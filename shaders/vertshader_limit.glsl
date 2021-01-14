#version 410
// Vertex shader

layout (location = 0) in vec3 vertlimitcoords_world_vs;
layout (location = 1) in vec3 vertlimitnormal_world_vs;

uniform mat4 modelviewmatrix;
uniform mat4 projectionmatrix;
uniform mat3 normalmatrix;

layout (location = 0) out vec3 vertlimitcoords_camera_fs;
layout (location = 1) out vec3 vertlimitnormal_camera_fs;

void main() {
  gl_Position = projectionmatrix * modelviewmatrix * vec4(vertlimitcoords_world_vs, 1.0);

  vertlimitcoords_camera_fs = vec3(modelviewmatrix * vec4(vertlimitcoords_world_vs, 1.0));
  vertlimitnormal_camera_fs = normalize(normalmatrix * vertlimitnormal_world_vs);
}


