#version 450

layout(location = 0) in vec3 position;

layout(push_constant) uniform PushConstants {
	mat4 transform;
} constants;

void main() {
	gl_Position = vec4(position, 1.0) * constants.transform;
	gl_Position.y *= -1.0;
}
