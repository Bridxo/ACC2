#version 410
// Fragment shader with green surface (basic)


out vec4 fColor;

void main() {

  vec3 lightcolour = vec3(0.0, 1.0, 0.0); //green

  fColor = vec4(lightcolour, 1.0);


}
