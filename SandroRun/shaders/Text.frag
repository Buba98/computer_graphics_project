#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set=0, binding = 0) uniform sampler2D texSampler;

layout(location = 0) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

void main() {
    vec4  diffColor = texture(texSampler, fragTexCoord).rgba;

    if (diffColor.r > 0.5f) {
        outColor = vec4(1.0f, 1.0f, 1.0f, diffColor.a);
    } else {
        outColor = vec4(0.0f, 0.0f, 0.0f, diffColor.a);
    }
}