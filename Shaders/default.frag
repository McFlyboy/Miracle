#version 450

layout(location = 0) in vec3 passColor;

layout(location = 0) out vec4 outColor;

void main() {
	outColor = vec4(passColor, 1.0);
}
