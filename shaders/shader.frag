#version 400

in vec3 fcolor;
in float fshade;

out vec4 frag_color;

void main() {
	frag_color = fshade * vec4(fcolor, 1.);
}
