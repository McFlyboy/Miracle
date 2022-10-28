#version 450

layout(location = 0) in vec2 vertex;

void main() {
	gl_Position = vec4(vertex.x, vertex.y * -1.0, 0.0, 1.0);
}
