#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(set=0, binding = 0) uniform sampler2D texSampler;

layout(location = 0) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

void main() {
    vec4  diffColor = texture(texSampler, fragTexCoord).rgba;

    vec3 color;
    if (diffColor.r > 0.5f) {
        color = vec3(1.0f, 1.0f, 1.0f);
    } else {
        color = vec3(0.0f, 0.0f, 0.0f);
    }

    outColor = vec4(color, diffColor.a);
}