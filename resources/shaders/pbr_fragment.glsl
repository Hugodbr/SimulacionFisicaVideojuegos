#version 330 core

in vec2 TexCoord;
in vec3 FragPosView;
in mat3 TBN;

out vec4 FragColor;

uniform sampler2D uAlbedo;
uniform sampler2D uNormal;
uniform sampler2D uRoughness;
uniform sampler2D uMetallic;
uniform sampler2D uAO;

// LIGHT DIRECTION ALREADY IN VIEW SPACE
uniform vec3 lightDirView;

void main()
{
    // Sample maps
    vec3 albedo = texture(uAlbedo, TexCoord).rgb;
    float rough = texture(uRoughness, TexCoord).r;
    float metal = texture(uMetallic, TexCoord).r;
    float ao    = texture(uAO, TexCoord).r;

    // Normal map
    vec3 nmap = texture(uNormal, TexCoord).rgb;
    nmap = nmap * 2.0 - 1.0;  // to range [-1,1]

    vec3 normal = normalize(TBN * nmap);

    // lighting
    vec3 L = normalize(-lightDirView);
    float NdotL = max(dot(normal, L), 0.0);

    vec3 diffuse = albedo * NdotL;
    vec3 ambient = albedo * 0.1 * ao;

    // simple specular
    vec3 V = normalize(-FragPosView);
    vec3 R = reflect(-L, normal);
    float spec = pow(max(dot(V, R), 0.0), mix(64.0, 4.0, rough));

    vec3 result = ambient + diffuse + spec * (1.0 - rough);
    FragColor = vec4(result, 1.0);
}
