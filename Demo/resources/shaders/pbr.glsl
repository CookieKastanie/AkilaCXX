#AKILA_VERTEX

out vec3 worldPos;
out vec3 normal;
out vec3 tangent;
out vec2 uv;

const mat4 M = mat4(1.);

void main() {
    vec4 W = M * a_position;

    worldPos = W.xyz;

    mat3 Mn = mat3(M[0].xyz, M[1].xyz, M[2].xyz); // instead of mat3(M), because c'est de la merde
    normal = Mn * a_normal.xyz;
    tangent = Mn * a_tangent.xyz;
    uv = a_uv.xy;

    gl_Position = u_camera.pv * W;
}

#AKILA_FRAGMENT

layout(location = 0) out vec4 FragColor;

in vec3 worldPos;
in vec3 normal;
in vec3 tangent;
in vec2 uv;

// material parameters
uniform sampler2D albedoSampler;
uniform sampler2D normalSampler;
uniform sampler2D metallicSampler;
uniform sampler2D roughnessSampler;
uniform sampler2D aoSampler;

// IBL
uniform samplerCube irradianceMap;
uniform samplerCube prefilterMap;
uniform sampler2D brdfLUT;

// lights
vec3 lightPositions[1];
vec3 lightColors[1];



uniform float gamma;
uniform float exposure;



float DistributionGGX(vec3 N, vec3 H, float roughness) {
    float a      = roughness * roughness;
    float a2     = a * a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;
	
    float num   = a2;
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
    
    // cook-torrance brdf
    float NDF = DistributionGGX(N, H, roughness);        
    float G   = GeometrySmith(NdotV, NdotL, roughness);      
    vec3 F    = fresnelSchlick(HdotV, F0);       
    
    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic;	  
    
    vec3 numerator    = NDF * G * F;
    float denominator = 4.0 * NdotV * NdotL;
    vec3 specular     = numerator / max(denominator, 0.001);  

    Lo += (kD * albedo / PI + specular) * radiance * NdotL; 
}


/*
vec3 aces(vec3 x) {
    const float a = 2.51;
    const float b = 0.03;
    const float c = 2.43;
    const float d = 0.59;
    const float e = 0.14;

    x *= 0.7;
    return (x * (a * x + b)) / (x * (c * x + d) + e);
}*/

vec3 aces(vec3 x) {
    //0.454545 = 1./2.2
    return pow((1.03292 * x * (x + 0.0170746)) / (x * x + 0.346855 * x + 0.1175780), vec3(0.454545));
}

vec3 acesAprox(vec3 x) {
    return x / (x + 0.238) * 1.064;
}

vec3 u3(vec3 x) {
    return x / (x + 0.155) * 1.019;
}

uniform int tonemapping;

void main() {
    lightPositions[0] = vec3(-5., 5., 5.);
    lightColors[0] = vec3(500., 0., 0.);

    // mat infos
    vec3 albedo = texture(albedoSampler, uv).rgb;
    float metallic = texture(metallicSampler, uv).r;
    float roughness = max(texture(roughnessSampler, uv).r, 0.00001);
    float ao = texture(aoSampler, uv).r;

    // normal
    vec3 nNormal = normalize(normal);
    vec3 nTangent = normalize(tangent);
    mat3 TBN = mat3(nTangent, cross(nTangent, nNormal), nNormal);

    vec3 N = normalize(TBN * (texture(normalSampler, uv).rgb * 2.0 - 1.0));
    vec3 V = normalize(u_camera.position - worldPos);

    ////
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

    vec3 Lo = vec3(0.0);

    // sunlight
    {
        vec3 L = normalize(vec3(1.));
        vec3 radiance = vec3(1, 0.996, 0.949) * 2.;        
        
        brdf(Lo, N, V, L, radiance, albedo, metallic, roughness, F0);
    }
    
    // points lights
    for(int i = 0; i < 1; ++i) {
        vec3 L = normalize(lightPositions[i] - worldPos);

        // calculate per-light radiance
        float distance    = length(lightPositions[i] - worldPos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance     = lightColors[i] * attenuation;        
        
        brdf(Lo, N, V, L, radiance, albedo, metallic, roughness, F0);
    }   
  
    // static
    //vec3 ambient = vec3(0.259, 0.647, 0.961) * albedo * ao;

    // IBL
    vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);
    
    vec3 kS = F;
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - metallic;	  
    
    vec3 irradiance = texture(irradianceMap, N).rgb;
    vec3 diffuse    = irradiance * albedo;
    
    const float MAX_REFLECTION_LOD = 4.0;
    vec3 R = reflect(-V, N);
    vec3 prefilteredColor = textureLod(prefilterMap, R,  roughness * MAX_REFLECTION_LOD).rgb;    
    vec2 brdf  = texture(brdfLUT, vec2(max(dot(N, V), 0.0), roughness)).rg;
    vec3 specular = prefilteredColor * (F * brdf.x + brdf.y);

    vec3 ambient = (kD * diffuse + specular) * ao;

    vec3 color = ambient + Lo;

	// exposition
    color = vec3(1.0) - exp(-color * exposure);

    // tone mapping
    if(tonemapping == 0) color = pow(color, vec3(1.0 / gamma));
    else if(tonemapping == 1) color = aces(color);
    else if(tonemapping == 2) color = acesAprox(color);
    else if(tonemapping == 3) color = u3(color);

   
    FragColor = vec4(color, 1.0);
}
