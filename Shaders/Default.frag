#version 450

layout(location = 0) out vec4 outColor;

layout(push_constant) uniform PushConstants {
	layout(offset = 64) vec3 color;
} constants;

void main() {
	outColor = vec4(constants.color, 1.0);
}
