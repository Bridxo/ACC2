#version 410
// Fragment shader


out vec4 fColor;

void main() {

  vec3 lightcolour = vec3(0.81, 0.71, 0.23); //old gold

  fColor = vec4(lightcolour, 1.0);


}
