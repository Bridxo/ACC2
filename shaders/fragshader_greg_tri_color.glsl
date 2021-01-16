#version 410
// Fragment shader


out vec4 fColor;

void main() {

  vec3 lightcolour = vec3(0.53, 0.81, 0.98); //rgb(135,206,250) lightsky blue

  fColor = vec4(lightcolour, 1.0);


}
