#version 410
// Fragment shader


out vec4 fColor;

void main() {

  vec3 lightcolour = vec3(0.85, 0.44, 0.84); //rgb(218,112,214) orchid purple

  fColor = vec4(lightcolour, 1.0);


}
