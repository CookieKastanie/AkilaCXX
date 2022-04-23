#AKILA_VERTEX

out vec3 worldPos;
out vec3 normal;

//uniform mat4 PV;
uniform mat4 model;

void main() {
	vec4 W = model * a_position;

    worldPos = W.xyz;
    normal = mat3(model) * a_normal.xyz;

    gl_Position = u_camera.pv * W;
}

#AKILA_FRAGMENT

in vec3 worldPos;
in vec3 normal;
out vec4 fragColor;

// Parametres du materiau
uniform vec3 albedo;
uniform float metallic;
uniform float roughness;

// lumieres
const int LIGHT_COUNT = 3;
uniform vec3 lightPositions[LIGHT_COUNT];
uniform vec3 lightColors[LIGHT_COUNT];
uniform float lightPowers[LIGHT_COUNT];

uniform vec3 ambiantColor;

// Camera
uniform float exposure;
uniform vec3 camPosition;

//////////////////////////////////////////////////////////////////////////////////////////
// Fonctions PBR
//////////////////////////////////////////////////////////////////////////////////////////

float DistributionGGX(vec3 N, vec3 H, float roughness) {
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;
	
    float num = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness) {
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}

float GeometrySmith(float NdotV, float NdotL, float roughness) {
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0) {
    return F0 + (1.0 - F0) * pow(max(1.0 - cosTheta, 0.0), 5.0);
}

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness) {
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(max(1.0 - cosTheta, 0.0), 5.0);
}

void brdf(
    inout vec3 Lo,
    in vec3 N, in vec3 V,
    in vec3 L, in vec3 radiance,
    in vec3 albedo, in float metallic, in float roughness, in vec3 F0
) {
    vec3 H = normalize(V + L);

    float HdotV = max(dot(H, V), 0.0);
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);    
    
    float NDF = DistributionGGX(N, H, roughness);        
    float G = GeometrySmith(NdotV, NdotL, roughness);      
    vec3 F = fresnelSchlick(HdotV, F0);       
    
    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic;	  
    
    vec3 numerator = NDF * G * F;
    float denominator = 4.0 * NdotV * NdotL;
    vec3 specular = numerator / max(denominator, 0.001);  

    Lo += (kD * albedo / PI + specular) * radiance * NdotL; 
}

//////////////////////////////////////////////////////////////////////////////////////////
// Programme principal
//////////////////////////////////////////////////////////////////////////////////////////

// Une approximation du tonemappeur ACES (fusionné à une correction gamma de 2.2)
vec3 acesAprox(vec3 x) {
    return x / (x + 0.238) * 1.064;
}

void main() {
    vec3 N = normalize(normal);
    vec3 V = normalize(u_camera.position - worldPos);

    vec3 F0 = mix(vec3(0.04), albedo, metallic);

    vec3 Lo = vec3(0.0);
    for(int i = 0; i < LIGHT_COUNT; ++i) {
        vec3 L = normalize(lightPositions[i] - worldPos);

        float distance = length(lightPositions[i] - worldPos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = lightColors[i] * lightPowers[i] * attenuation;        
        
        brdf(Lo, N, V, L, radiance, albedo, metallic, max(roughness, 0.0001), F0);
    }   

    vec3 color = ambiantColor * albedo + Lo;

	// exposition
    color = vec3(1.0) - exp(-color * exposure);

    // tonemapping
    color = acesAprox(color);

    fragColor = vec4(color, 1.0);
}
