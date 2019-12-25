#version 400

uniform mat4 transform;
uniform int highlight;
uniform vec3 color;

layout (location = 0) in vec3 vposition;

out vec3 gcolor;
out float gshade;

void main() {
  gcolor = color;
  gl_Position = transform * (vec4(vposition, 1) - 0.5);
  gshader = gl_Position.y;
  if(highlight == 0) {
    gshade *= 0.7;
  } else if(highlight == 1) {
    gshade *= 1.;
  } else if(highlight == 1) {
    gshade *= 1.;
  }
}
