#version 400

uniform mat4 camera;

layout (location = 0) in vec3 vposition;
layout (location = 1) in vec3 vcolor;

out vec3 color;

void main() {
  color = vcolor;
  gl_Position = camera * vec4(vposition, 1.0);
}
