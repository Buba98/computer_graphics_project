#version 450#extension GL_ARB_separate_shader_objects : enablelayout(location = 0) in vec3 fragPos;layout(location = 1) in vec3 fragNorm;layout(location = 2) in vec2 fragUV;layout(location = 0) out vec4 outColor;layout(set = 0, binding = 0) uniform GlobalUniformBufferObject {    int dayTime;    int shift;    vec3 DlightDir;// direction of the direct light    vec3 DlightColor;// color of the direct light    float DlightIntensity;// intensity of the direct light    vec3 AmbLightColor;// ambient light    vec3 eyePos;// position of the viewer} gubo;layout(set = 1, binding = 0) uniform UniformBufferObject {    float amb;    float gamma;    vec3 sColor;    mat4 mvpMat;    mat4 mMat;    mat4 nMat;} ubo;layout(set = 1, binding = 1) uniform sampler2D tex;layout(set = 1, binding = 2) uniform sampler2D tex_emissive;const float cos_out = 0.85f;const float beta = 3.0f;const float cos_in = 0.95f;const float g = 1.5f;//            lightPos = vec3(5.0f, 3.0f, 10.0f - 30.0f - i * 60.0f + gubo.shift * 120.0f);//            lightPos = vec3(-5.0f, 3.0f, 10.0f - i * 60.0f + gubo.shift * 120.0f);vec3 Phong(vec3 L, vec3 N, vec3 V, vec3 specularColor, float specularPower) {    vec3 R = reflect(-L, N);    float specularTerm = clamp(pow(max(dot(R, V), 0.0f), specularPower), 0.0f, 1.0f);    return specularColor * specularTerm;}vec3 Lambert(vec3 L, vec3 N, vec3 diffuseColor) {    float diffuseTerm = max(dot(L, N), 0.0f);    return diffuseColor * diffuseTerm;}vec3 SpotLightModel(vec3 fragPos, vec3 spotlight_pos, vec3 spotlight_light_dir, vec3 spotlight_light_color) {    float spotlight_cone = clamp((dot(normalize(fragPos - spotlight_pos), spotlight_light_dir) - cos_out) / (cos_in - cos_out), 0.0f, 1.0f);    return spotlight_light_color * spotlight_cone;}void main() {    vec3 N = normalize(fragNorm);// surface normal    vec3 V = normalize(gubo.eyePos - fragPos);// viewer direction    vec3 L = normalize(gubo.DlightDir);// light direction    vec3 albedo = texture(tex, fragUV).rgb;// main color    // Light models    // Direct light    vec3 direct_light_color = gubo.DlightColor;    vec3 direct_light_dir = L;    vec3 direct_light = direct_light_color * gubo.DlightIntensity;    // Spot light    vec3 spotlight_light_color = vec3(1.0f);    vec3 spotlight_light_streetlight = vec3(0.0f);    vec3 spotlight_light_moto = vec3(0.0f);    vec3 spotlight_light_dir_streetlight = vec3(0.0f, -1.0f, 0.0f);    vec3 spotlight_light_dir_moto = vec3(0.0f, 0.0f, -1.0f);    if (gubo.dayTime != 0){        vec3 spotlight_pos = gubo.eyePos;        spotlight_light_moto = SpotLightModel(fragPos, spotlight_pos, spotlight_light_dir_moto, spotlight_light_color);        for (int i = 0; i < 8; i++){            spotlight_pos = vec3(5.0f, 3.0f, 10.0f - 30.0f - i * 60.0f + gubo.shift * 120.0f);            spotlight_light_streetlight += SpotLightModel(fragPos, spotlight_pos, spotlight_light_dir_streetlight, spotlight_light_color);        }        for (int i = 0; i < 8; i++){            spotlight_pos = vec3(-5.0f, 3.0f, 10.0f - i * 60.0f + gubo.shift * 120.0f);            spotlight_light_streetlight += SpotLightModel(fragPos, spotlight_pos, spotlight_light_dir_streetlight, spotlight_light_color);        }    }    // BRDF - Diffuse    // Direct light    vec3 direct_diffuse = Lambert(direct_light_dir, N, albedo);    // Spot light    vec3 spotlight_diffuse_moto;    vec3 spotlight_diffuse_streetlight;    if (gubo.dayTime != 0){        spotlight_diffuse_moto = Lambert(spotlight_light_dir_moto, N, albedo);        spotlight_diffuse_streetlight = Lambert(spotlight_light_dir_streetlight, N, albedo);    }    // BRDF - Specular    // Direct light    vec3 direct_specular = Phong(direct_light_dir, N, V, ubo.sColor, ubo.gamma);    // Spot light    vec3 spotlight_specular_moto;    vec3 spotlight_specular_streetlight;    if (gubo.dayTime != 0){        spotlight_specular_moto = Phong(spotlight_light_dir_moto, N, V, ubo.sColor, ubo.gamma);        spotlight_specular_streetlight = Phong(spotlight_light_dir_streetlight, N, V, ubo.sColor, ubo.gamma);    }    // Ambient light    vec3 ambient_tot = albedo * gubo.AmbLightColor;    // Emissive light    vec3 emissive_tot = vec3(0.0f);    if (gubo.dayTime != 0){        emissive_tot = texture(tex_emissive, fragUV).rgb;    }    // Final color    vec3 direct_tot = direct_light * (direct_diffuse + direct_specular);    vec3 spotlight_tot = vec3(0.0f);    if (gubo.dayTime != 0){        spotlight_tot = spotlight_light_moto * (spotlight_diffuse_moto + spotlight_specular_moto) + spotlight_light_streetlight * (spotlight_diffuse_streetlight + spotlight_specular_streetlight);    }    outColor = vec4(clamp(direct_tot + spotlight_tot + emissive_tot + ambient_tot, 0.0f, 1.0f), 1.0f);}