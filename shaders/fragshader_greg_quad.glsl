#version 410
// Fragment shader


out vec4 fColor;

void main() {

  vec3 lightcolour = vec3(0.0, 0.7, 0.2); //dark green

  fColor = vec4(lightcolour, 1.0);


}
