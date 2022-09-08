#akila_vertex

layout(location = a_position_loc) in vec4 a_position;
layout(location = a_normal_loc) in vec4 a_normal;
layout(location = a_uv_loc) in vec4 a_uv;

out vec3 normal;
out vec2 uv;
out vec3 fragPos;

out vec3 sunDir;

uniform mat4 modelMatrix;

struct akila_camera {
	mat4 projection;
	mat4 view;
	mat4 pv;
	vec3 position;
	float ratio;
};

uniform akila_camera u_camera;

void main() {
    mat4 VM = u_camera.view * modelMatrix;
    
    mat3 normalMatrix = transpose(inverse(mat3(VM)));
    normal = normalMatrix * a_normal.xyz;
    
    uv = a_uv.xy;

    vec4 viewPos = VM * a_position;
    fragPos = viewPos.xyz; 

    sunDir = (u_camera.view * vec4(1, 1, 1, 0)).xyz;

    gl_Position = u_camera.projection * viewPos;
}


#akila_fragment

in vec3 normal;
in vec2 uv;
in vec3 fragPos;

in vec3 sunDir;

#akila_file shaders/common.glsl
#akila_file shaders/pbr_functions.glsl

uniform float exposure = 2.;

vec3 brdf(vec3 albedo, float roughness, float metallic) {
    roughness = max(roughness, 0.01);

    vec3 N = normalize(normal);
    vec3 V = normalize(-fragPos);


    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

    vec3 Lo = vec3(0.0);


    // sunlight
    {
        vec3 L = normalize(sunDir);
        vec3 radiance = vec3(1, 0.996, 0.949) * 2.;        
        
        brdf_impl(Lo, N, V, L, radiance, albedo, metallic, roughness, F0);
    }


    // points lights
    /*/
    for(int i = 0; i < 1; ++i) {
        vec3 L = normalize(lightPositions[i] - fragPos);

        // calculate per-light radiance
        float distance    = length(lightPositions[i] - fragPos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance     = lightColors[i] * attenuation;        
        
        brdf_impl(Lo, N, V, L, radiance, albedo, metallic, roughness, F0);
    }
    //*/


    // static
    vec3 ambient = vec3(0.259, 0.647, 0.961) * albedo;// * ao;

    // IBL
    /*/
    vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);
    
    vec3 kS = F;
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - metallic; 
    
    vec3 irradiance = texture(irradianceMap, N).rgb;
    vec3 diffuse    = irradiance * albedo;
    
    const float MAX_REFLECTION_LOD = 4.0;
    vec3 R = reflect(-V, N);
    vec3 prefilteredColor = textureLod(prefilterMap, R,  roughness * MAX_REFLECTION_LOD).rgb;    
    vec2 brdf = texture(brdfLUT, vec2(max(dot(N, V), 0.0), roughness)).rg;
    vec3 specular = prefilteredColor * (F * brdf.x + brdf.y);

    vec3 ambient = (kD * diffuse + specular);// * ao;
    //*/

    vec3 color = ambient + Lo;

    // exposition
    color = vec3(1.0) - exp(-color * exposure);
    return aces(color);
}

#akila_user_code
