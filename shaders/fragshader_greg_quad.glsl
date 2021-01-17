#version 410
// Fragment shader


out vec4 fColor;

void main() {

  vec3 lightcolour = vec3(0.85, 0.43, 0.83); //orchid color
  fColor = vec4(lightcolour, 1.0);


}
