#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec3 fragPos;
layout(location = 1) in vec3 fragNorm;
layout(location = 2) in vec2 fragUV;

layout(location = 0) out vec4 outColor;

layout(set = 0, binding = 0) uniform GlobalUniformBufferObject {
    int dayTime;
    int shift;
    vec3 DlightDir;// direction of the direct light
    vec3 DlightColor;// color of the direct light
    float DlightIntensity;// intensity of the direct light
    vec3 AmbLightColor;// ambient light
    vec3 eyePos;// position of the viewer
    vec3 motoPos;
    vec3 motoDir;
} gubo;

layout(set = 1, binding = 0) uniform UniformBufferObject {
    int palette;
    float amb;
    float gamma;
    vec3 sColor;
    mat4 mvpMat;
    mat4 mMat;
    mat4 nMat;
} ubo;

layout(set = 1, binding = 1) uniform sampler2D tex_0;
layout(set = 1, binding = 2) uniform sampler2D tex_1;
layout(set = 1, binding = 3) uniform sampler2D tex_2;
layout(set = 1, binding = 4) uniform sampler2D tex_3;
layout(set = 1, binding = 5) uniform sampler2D tex_4;
layout(set = 1, binding = 6) uniform sampler2D tex_emissive;

const float cos_out = 0.85f;
const float cos_in = 0.95f;
const float beta = 3.0f;
const float g = 0.5f;

vec3 streetlights() {
    vec3 lightColor = vec3(0.0f);
    vec3 lightPos;
    vec3 lightDir1;
    vec3 diff;
    vec3 lightDir;

    if (gubo.dayTime != 0){
        lightDir1 = vec3(0.0f, -1.0f, 0.0f);
        // right-side streetlights
        for (int i = 0; i < 8; i++){
            lightPos = vec3(5.0f, 3.0f, 10.0f - 30.0f - i * 60.0f + gubo.shift * 120.0f);
            diff = lightPos - fragPos;
            lightDir = diff / length(diff);
            lightColor = lightColor + pow(g / length(diff), beta) * gubo.DlightColor.rgb * clamp(dot(lightDir, lightDir1), cos_out, cos_in);
        }
        // left-side streetlights
        for (int i = 0; i < 8; i++){
            lightPos = vec3(-5.0f, 3.0f, 10.0f - i * 60.0f + gubo.shift * 120.0f);
            diff = lightPos - fragPos;
            lightDir = diff / length(diff);
            lightColor = lightColor + pow(g / length(diff), beta) * gubo.DlightColor.rgb * clamp(dot(lightDir, lightDir1), cos_out, cos_in);
        }
    }

    return lightColor;
}

void main() {
    vec3 N = normalize(fragNorm);// surface normal
    vec3 V = normalize(gubo.eyePos - fragPos);// viewer direction
    vec3 L = normalize(gubo.DlightDir);// light direction

    vec3 albedo;// main color
    if (ubo.palette == 1){
        albedo = texture(tex_1, fragUV).rgb;
    } else if (ubo.palette == 2){
        albedo = texture(tex_2, fragUV).rgb;
    } else if (ubo.palette == 3){
        albedo = texture(tex_3, fragUV).rgb;
    } else if (ubo.palette == 4){
        albedo = texture(tex_4, fragUV).rgb;
    } else {
        albedo = texture(tex_0, fragUV).rgb;
    }
    vec3 MD = albedo;
    vec3 MS = ubo.sColor;
    vec3 MA = albedo * ubo.amb;
    vec3 LA = gubo.AmbLightColor;
    float DI = gubo.DlightIntensity;
    vec3 ME;
    if (gubo.dayTime == 0){
        ME = vec3(0.0f);
    } else {
        ME = texture(tex_emissive, fragUV).rgb;
    }

    vec3 lightColor = streetlights();

    // output color
    outColor = vec4(clamp(MD * clamp(dot(L, N), 0.0f, 1.0f) * DI
                    +MS * DI * pow(clamp(dot(N, normalize(L + V)), 0.0f, 1.0f), ubo.gamma)
                    +LA * MA + ME + (MD + MS) * lightColor, 0.0f, 1.0f), 1.0f);
}