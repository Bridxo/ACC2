#version 410
// Fragment shader


out vec4 fColor;

void main() {

  vec3 lightcolour = vec3(0.52,0.80,0.92); //sky blue

  fColor = vec4(lightcolour, 1.0);


}
