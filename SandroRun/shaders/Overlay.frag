#version 450#extension GL_ARB_separate_shader_objects : enablelayout(set = 0, binding = 0) uniform UniformBufferObject {	float visible;	int splashSelector;} ubo;layout(location = 0) in vec2 fragUV;layout(location = 0) out vec4 outColor;layout(binding = 1) uniform sampler2D texStart;layout(binding = 2) uniform sampler2D texEnd;void main() {	switch (ubo.splashSelector) {		case 0:			outColor = vec4(texture(texStart, fragUV).rgb,  ubo.visible);			break;		case 1:			outColor = vec4(texture(texEnd, fragUV).rgb, ubo.visible);			break;	}}