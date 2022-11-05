#version 450

layout(location = 0) in vec2 position;

layout(push_constant) uniform PushConstants {
	vec2 translation;
	float aspectRatio;
} constants;

void main() {
	gl_Position = vec4(
		(position + constants.translation) * vec2(1.0 / constants.aspectRatio, -1.0),
		0.0,
		1.0
	);
}
