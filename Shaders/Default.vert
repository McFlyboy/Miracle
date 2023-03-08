#version 450

layout(location = 0) in vec3 position;

layout(push_constant) uniform PushConstants {
	vec3 translation;
	float aspectRatio;
} constants;

void main() {
	gl_Position = vec4(
		(position.x + constants.translation.x) *  1.0 / constants.aspectRatio,
		(position.y + constants.translation.y) * -1.0,
		 position.z + constants.translation.z,
		 1.0
	);
}
