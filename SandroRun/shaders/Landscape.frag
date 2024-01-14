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
    vec3 AmbLightColor;// ambient light
    vec3 eyePos;// position of the viewer
    vec3 motoPos;
    vec3 motoDir;
} gubo;

layout(set = 1, binding = 0) uniform UniformBufferObject {
    float gamma;
    vec3 sColor;
    mat4 mvpMat;
    mat4 mMat;
    mat4 nMat;
} ubo;

layout(set = 1, binding = 1) uniform sampler2D tex;
layout(set = 1, binding = 2) uniform sampler2D tex_emissive;

const float cos_out_street = 0.1f;
const float cos_out_moto = 0.85f;
const float beta = 3.0f;
const float cos_in = 0.95f;
const float g_moto = 80.0f;
const float g_streetlight = 10.0f;

const float sigma = 0.1f;

vec3 OrenNayar(vec3 V, vec3 N, vec3 L, vec3 Md, float sigma) {
    //vec3 V  - direction of the viewer
    //vec3 N  - normal vector to the surface
    //vec3 L  - light vector (from the light model)
    //vec3 Md - main color of the surface
    //float sigma - Roughness of the model

    vec3 lambertDiffuse = Md * clamp(dot(N, L), 0, 1);

    float A = 1.0f - 0.5f * ((sigma*sigma) / (sigma*sigma+0.33f));
    float B = 0.45 * (sigma*sigma) / (sigma*sigma + 0.09);
    vec3 vi = normalize(L - dot(L, N) * N);
    vec3 vr = normalize(V - dot(V, N) * N);
    float G = max(0, dot(vi, vr));

    float thetai = acos(dot(L, N));
    float thetar = acos(dot(V, N));

    float alpha = max(thetai, thetar);
    float beta = min(thetai, thetar);

    vec3 orenNayar = lambertDiffuse * (A + B * G * sin(alpha) * tan(beta));

    return orenNayar;
}

vec3 SpotLightModel(vec3 fragPos, vec3 spotlight_pos, vec3 spotlight_light_dir, vec3 spotlight_light_color, bool is_streetlight) {
    vec3 diff = spotlight_pos - fragPos;

    float cos_out = is_streetlight ? cos_out_street : cos_out_moto;
    float g = is_streetlight ? g_streetlight : g_moto;

    float spotlight_cone = clamp(pow(g / length(diff), beta) * clamp((dot(normalize(diff), spotlight_light_dir) - cos_out) / (cos_in - cos_out), 0.0f, 1.0f), 0.0f, 1.0f);
    return spotlight_light_color * spotlight_cone;
}

void main() {
    vec3 N = normalize(fragNorm);// surface normal
    vec3 V = normalize(gubo.eyePos - fragPos);// viewer direction
    vec3 L = gubo.DlightDir;// light direction

    vec3 albedo = texture(tex, fragUV).rgb;// main color

    // Light models
    // Direct light
    vec3 direct_light_color = gubo.DlightColor;
    vec3 direct_light_dir = L;
    vec3 direct_light = direct_light_color;

    // Spot light
    vec3 spotlight_light_color = vec3(1.0f);

    vec3 spotlight_light_streetlight = vec3(0.0f);
    vec3 spotlight_light_moto = vec3(0.0f);

    vec3 spotlight_light_dir_streetlight = vec3(0.0f, 1.0f, 0.0f); // direction pointing downwards
    vec3 spotlight_light_dir_moto = gubo.motoDir;

    if (gubo.dayTime != 0){
        vec3 spotlight_pos = gubo.motoPos;
        spotlight_light_moto = SpotLightModel(fragPos, spotlight_pos, spotlight_light_dir_moto, spotlight_light_color, false);

        vec3 spotlight_pos_right = vec3(5.0f, 5.0f, -20.0f + gubo.shift * 120.0f);
        vec3 spotlight_pos_left = vec3(-5.0f, 5.0f, 10.0f + gubo.shift * 120.0f);

        for (int i = 0; i < 8; i++){
            spotlight_light_streetlight += SpotLightModel(fragPos, spotlight_pos_right, spotlight_light_dir_streetlight, spotlight_light_color, true);
            spotlight_light_streetlight += SpotLightModel(fragPos, spotlight_pos_left, spotlight_light_dir_streetlight, spotlight_light_color, true);
            spotlight_pos_right.z -= 60.0f;
            spotlight_pos_left.z -= 60.0f;
        }
    }

    // BRDF - Oren-Nayar
    // Direct light
    vec3 direct_BRDF = OrenNayar(V, N, L, albedo, sigma);
    //Spot light
    vec3 spotlight_BRDF_moto;
    vec3 spotlight_BRDF_streetlight;
    if(gubo.dayTime != 0) {
        spotlight_BRDF_moto = OrenNayar(V, N, spotlight_light_dir_moto, albedo, sigma);
        spotlight_BRDF_streetlight = OrenNayar(V, N, spotlight_light_dir_streetlight, albedo, sigma);
    }

    // Ambient light
    vec3 ambient_tot = albedo * gubo.AmbLightColor;

    // Final color
    vec3 direct_tot = direct_light * direct_BRDF;
    vec3 spotlight_tot = vec3(0.0f);

    if (gubo.dayTime != 0){
        spotlight_tot = spotlight_light_moto * spotlight_BRDF_moto + spotlight_light_streetlight * spotlight_BRDF_streetlight;
    }

    outColor = vec4(clamp(ambient_tot + direct_tot + spotlight_tot, 0.0f, 1.0f), 1.0f);
}