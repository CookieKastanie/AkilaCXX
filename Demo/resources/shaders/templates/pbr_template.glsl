#akila_vertex

layout(location = a_position_loc) in vec4 a_position;
layout(location = a_normal_loc) in vec4 a_normal;
layout(location = a_uv_loc) in vec4 a_uv;

out vec3 normal;
out vec2 uv;
out vec3 worldPos;

//out vec3 sunDir;

uniform mat4 u_modelMatrix;

struct Camera {
	mat4 projection;
	mat4 view;
	mat4 pv;
	vec3 position;
	float ratio;
};

layout(std140, binding = u_camera_loc) uniform camera_ubo {
    Camera u_camera;
};

void main() {
    /*/
    //mat4 VM = u_camera.view * u_modelMatrix;
    mat4 VM = u_camera.pv * u_modelMatrix;
    
    //mat3 normalMatrix = transpose(inverse(mat3(VM)));
    mat3 normalMatrix = transpose(inverse(mat3(u_modelMatrix)));
    normal = normalMatrix * a_normal.xyz;
    
    uv = a_uv.xy;

    //vec4 viewPos = VM * a_position;
    vec4 viewPos = u_modelMatrix * a_position;
    fragPos = viewPos.xyz; 

    //sunDir = (u_camera.view * vec4(3, 2, 1, 0)).xyz;

    //gl_Position = u_camera.projection * viewPos;
    //gl_Position = viewPos;


    gl_Position = u_camera.pv * u_modelMatrix * a_position;
    //*/

    vec4 W = u_modelMatrix * a_position;

    worldPos = W.xyz;

    mat3 normalMatrix = transpose(inverse(mat3(u_modelMatrix)));
    normal = normalMatrix * a_normal.xyz;

    uv = a_uv.xy;

    gl_Position = u_camera.pv * W;
}


#akila_fragment

in vec3 normal;
in vec2 uv;
in vec3 worldPos;

//in vec3 sunDir;
const vec3 sunDir = vec3(3, 2, 1);

#akila_file shaders/common.glsl
#akila_file shaders/templates/pbr_functions.glsl

uniform float exposure = 1.;



struct Camera {
	mat4 projection;
	mat4 view;
	mat4 pv;
	vec3 position;
	float ratio;
};

layout(std140, binding = u_camera_loc) uniform camera_ubo {
    Camera u_camera;
};



layout(binding = 10) uniform sampler2D brdfLUT;
layout(binding = 11) uniform samplerCube irradianceMap;
layout(binding = 12) uniform samplerCube prefilterMap;

vec3 brdf(vec3 albedo, float roughness, float metallic) {
    roughness = max(roughness, 0.01);

    vec3 N = normalize(normal);
    vec3 V = normalize(u_camera.position - worldPos);


    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

    vec3 Lo = vec3(0.0);


    // sunlight
    {
        vec3 L = normalize(sunDir);
        vec3 radiance = vec3(1, 0.996, 0.949) * 3.;        
        
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
    //vec3 ambient = vec3(0.259, 0.647, 0.961) * albedo;// * ao;
    //vec3 ambient = vec3(0.159) * albedo;

    // IBL
    //*/
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
