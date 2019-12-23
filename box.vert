#version 400

uniform mat4 transform;
uniform int highlight;
uniform vec3 vcolor;

layout (location = 0) in vec3 vposition;

out vec3 gcolor;
out float gshade;

void main() {
  gcolor = vcolor;
  if(highlight == 0) {
    gshade = 0.8;
  } else {
    gshade = 1.0;
  }
  gl_Position = transform * (vec4(vposition, 1) - 0.5);
}
