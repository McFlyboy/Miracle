#version 450

layout(location = 0) in vec2 position;

void main() {
	gl_Position = vec4(position.x, position.y * -1.0, 0.0, 1.0);
}
