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

void brdf_impl(
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
