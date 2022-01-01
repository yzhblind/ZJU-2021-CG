#version 330 core
out vec4 FragColor;

in VF {
  vec2 TexCoords;
  vec3 TangentLightPos0;
  vec3 TangentLightPos1;
  vec3 TangentViewPos;
  vec3 TangentFragPos;
  vec4 FragPosLightSpace;
}
fs_in;

struct dirLight {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

struct pointLight {
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;

  float constant;
  float linear;
  float quadratic;
};

uniform bool en_light0;
uniform bool en_light1;
uniform dirLight light0;
uniform pointLight light1;

uniform sampler2D shadowMap;

uniform bool en_diffuse;
uniform bool en_specular;
uniform bool en_normal;
uniform sampler2D diffuse_texture;
uniform sampler2D specular_texture;
uniform sampler2D normal_texture;
uniform vec3 diffuse_material = vec3(0.0, 0.0, 0.8);
uniform vec3 specular_material = vec3(0.2);

uniform float shininess = 64.0;

float ShadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir) {
  vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
  projCoords = projCoords * 0.5 + 0.5;
  float closestDepth = texture(shadowMap, projCoords.xy).r;
  float currentDepth = projCoords.z;
  float shadow = 0.0;
  vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
  for (int x = -2; x <= 2; ++x) {
    for (int y = -2; y <= 2; ++y) {
      float pcfDepth =
          texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
      shadow += currentDepth > pcfDepth ? 1.0 : 0.0;
    }
  }
  shadow /= 25.0;

  if (projCoords.z > 1.0)
    shadow = 0.0;

  return shadow;
}

void main() {
  vec3 normal;
  if (en_normal) {
    normal = texture(normal_texture, fs_in.TexCoords).rgb;
    normal = normalize(normal * 2.0 - 1.0);
  } else {
    normal = vec3(0.0, 0.0, 1.0);
  }

  vec3 diff_color = en_diffuse ? texture(diffuse_texture, fs_in.TexCoords).rgb
                               : diffuse_material;
  // ambient
  vec3 ambient = diff_color;
  // diffuse
  vec3 lightDir0 = normalize(fs_in.TangentLightPos0 - fs_in.TangentFragPos);
  vec3 lightDir1 = normalize(fs_in.TangentLightPos1 - fs_in.TangentFragPos);
  float diff0 = max(dot(lightDir0, normal), 0.0);
  float diff1 = max(dot(lightDir1, normal), 0.0);
  vec3 diffuse0 = diff0 * diff_color;
  vec3 diffuse1 = diff1 * diff_color;
  // specular
  vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
  vec3 halfwayDir0 = normalize(lightDir0 + viewDir);
  vec3 halfwayDir1 = normalize(lightDir1 + viewDir);
  float spec0 = pow(max(dot(normal, halfwayDir0), 0.0), shininess);
  float spec1 = pow(max(dot(normal, halfwayDir1), 0.0), shininess);
  vec3 spec_color = en_specular ? texture(specular_texture, fs_in.TexCoords).rgb
                                : specular_material;
  vec3 specular0 = spec_color * spec0;
  vec3 specular1 = spec_color * spec1;

  vec3 result0 = vec3(0);
  vec3 result1 = vec3(0);

  if (en_light0) {
    float shadowFactor = ShadowCalculation(fs_in.FragPosLightSpace,
                                           vec3(0.0, 0.0, 1.0), lightDir0);
    result0 = ambient * light0.ambient +
              (diffuse0 * light0.diffuse + specular0 * light0.specular) *
                  (1.0 - shadowFactor);
  }
  if (en_light1) {
    float dis = length(fs_in.TangentLightPos1 - fs_in.TangentFragPos);
    float atten = 1.0 / (light1.constant + light1.linear * dis +
                         light1.quadratic * (dis * dis));
    result1 = atten * (ambient * light1.ambient + diffuse1 * light1.diffuse +
                       specular1 * light1.specular);
  }

  FragColor = vec4(result0 + result1, 1.0);
}