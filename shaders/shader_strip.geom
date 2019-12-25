#version 400

layout (triangles, invocations = 1) in;
layout (line_strip, max_vertices = 2) out;

in vec3 gcolor[3];
in float gshade[3];

out vec3 fcolor;
out float fshade;

void main() {
  fcolor = gcolor[0];
  fshade = gshade[0];
  gl_Position = gl_in[0].gl_Position;
  EmitVertex();
  fcolor = gcolor[1];
  fshade = gshade[1];
  gl_Position = gl_in[1].gl_Position;
  EmitVertex();
  EndPrimitive();
  fcolor = gcolor[1];
  fshade = gshade[1];
  gl_Position = gl_in[1].gl_Position;
  EmitVertex();
  fcolor = gcolor[2];
  fshade = gshade[2];
  gl_Position = gl_in[2].gl_Position;
  EmitVertex();
  EndPrimitive();
  fcolor = gcolor[0];
  fshade = gshade[0];
  gl_Position = gl_in[0].gl_Position;
  EmitVertex();
  fcolor = gcolor[2];
  fshade = gshade[2];
  gl_Position = gl_in[2].gl_Position;
  EmitVertex();
  EndPrimitive();
}
