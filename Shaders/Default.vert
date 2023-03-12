#version 450

layout(location = 0) in vec3 position;

layout(push_constant) uniform PushConstants {
	mat4 transform;
	float aspectRatio;
} constants;

void main() {
	gl_Position = vec4(position, 1.0)
		* constants.transform
		* vec4(1.0 / constants.aspectRatio, -1.0, 1.0, 1.0);
}
