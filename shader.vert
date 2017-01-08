#version 400

layout(location = 0) in vec3 vposition;
layout(location = 1) in vec3 vcolor;

out vec3 color;

void main() {
  color = vcolor;
  gl_Position = vec4(vposition, 1.0);
}
