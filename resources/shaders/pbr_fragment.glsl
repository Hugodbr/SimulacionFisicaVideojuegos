#version 330 core

in vec2 TexCoord;
in vec3 FragPosView;
in mat3 TBN;

out vec4 FragColor;

// -----------------------------------------------------------------------------
// Material flags (provided by your PBRMaterial::bindAll)
uniform bool hasAlbedoTex;
uniform bool hasNormalTex;
uniform bool hasMetalTex;
uniform bool hasRoughTex;
uniform bool hasAoTex;
uniform bool hasEmissiveTex;
uniform bool hasMetallicRoughnessTex;

// -----------------------------------------------------------------------------
// Texture samplers
uniform sampler2D uAlbedo;
uniform sampler2D uNormal;
uniform sampler2D uMetallic;
uniform sampler2D uRoughness;
uniform sampler2D uMetallicRoughness;  // glTF combined texture (G=roughness, B=metallic)
uniform sampler2D uAO;
uniform sampler2D uEmissive;

// -----------------------------------------------------------------------------
// Fallback scalar material values
uniform vec3  uBaseColor;
uniform float uMetallicValue;
uniform float uRoughnessValue;
uniform float uAoValue;
uniform vec3  uEmissiveColor;

// -----------------------------------------------------------------------------
// Light direction already in view space
uniform vec3 lightDirView;

// -----------------------------------------------------------------------------
// Main shader function
void main()
{
    // ----------------------------------------------------------
    // 1. ALBEDO
    // ----------------------------------------------------------
    vec3 albedo = hasAlbedoTex ?
        texture(uAlbedo, TexCoord).rgb :
        uBaseColor;


    // ----------------------------------------------------------
    // 2. METALLIC + ROUGHNESS
    // glTF may store both in ONE TEXTURE:
    //   MetallicRoughness.rgb = (occlusion?, roughness, metallic)
    // ----------------------------------------------------------
    float metallic;
    float roughness;

    if (hasMetallicRoughnessTex)
    {
        vec3 mr = texture(uMetallicRoughness, TexCoord).rgb;
        roughness = mr.g;   // Green  = roughness
        metallic  = mr.b;   // Blue   = metallic
    }
    else
    {
        metallic = hasMetalTex ?
            texture(uMetallic, TexCoord).r :
            uMetallicValue;

        roughness = hasRoughTex ?
            texture(uRoughness, TexCoord).r :
            uRoughnessValue;
    }


    // ----------------------------------------------------------
    // 3. Ambient Occlusion
    // ----------------------------------------------------------
    float ao = hasAoTex ?
        texture(uAO, TexCoord).r :
        uAoValue;


    // ----------------------------------------------------------
    // 4. Normal Map (fallback if missing)
    // ----------------------------------------------------------
    vec3 normal;
    if (hasNormalTex)
    {
        vec3 nmap = texture(uNormal, TexCoord).rgb;
        nmap = nmap * 2.0 - 1.0;   // Convert from [0,1] to [-1,1]
        normal = normalize(TBN * nmap);
    }
    else
    {
        normal = normalize(TBN[2]);  // If no normal map: use interpolated normal
    }


    // ----------------------------------------------------------
    // 5. Basic Lighting (view space)
    // ----------------------------------------------------------
    vec3 L = normalize(-lightDirView);
    float NdotL = max(dot(normal, L), 0.0);

    // Diffuse term
    vec3 diffuse = albedo * NdotL;

    // Ambient term
    vec3 ambient = albedo * 0.1 * ao;


    // ----------------------------------------------------------
    // 6. Specular
    // ----------------------------------------------------------
    vec3 V = normalize(-FragPosView);
    vec3 H = normalize(V + L);         // Half-vector
    float NdotH = max(dot(normal, H), 0.0);
    float NdotV = max(dot(normal, V), 0.0);

    // Fresnel Schlick approximation
    vec3 F0 = mix(vec3(0.04), albedo, metallic);  // Non-metals → 0.04, metals → albedo
    vec3 F  = F0 + (1.0 - F0) * pow(1.0 - NdotV, 5.0);

    // Blinn-Phong-like shininess mapped to roughness
    float shininess = mix(4.0, 128.0, 1.0 - roughness);
    float spec = pow(NdotH, shininess);

    vec3 specular = F * spec * NdotL;


    // ----------------------------------------------------------
    // 7. Emissive
    // ----------------------------------------------------------
    vec3 emissive = hasEmissiveTex ?
        texture(uEmissive, TexCoord).rgb :
        uEmissiveColor;


    // ----------------------------------------------------------
    // 8. Final Color
    // ----------------------------------------------------------
    vec3 result = ambient + diffuse + specular + emissive;

    FragColor = vec4(result, 1.0);
}
