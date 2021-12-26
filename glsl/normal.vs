#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;

out VF {
  vec2 TexCoords;
  vec3 TangentLightPos0;
  vec3 TangentLightPos1;
  vec3 TangentViewPos;
  vec3 TangentFragPos;
  vec4 FragPosLightSpace;
}
vs_out;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat3 normalMatrix;
uniform mat4 lightSpaceMatrix;

uniform vec3 light0_dir = vec3(0);
uniform vec3 light1_pos = vec3(0);
uniform vec3 viewPos;

void main() {
  vec3 fragPos = vec3(model * vec4(aPos, 1.0));
  vs_out.TexCoords = aTexCoords;

  vec3 T = normalize(normalMatrix * aTangent);
  vec3 N = normalize(normalMatrix * aNormal);
  vec3 B = normalize(normalMatrix * aBitangent);
  T = normalize(T - dot(T, N) * N);
  B = normalize(B - dot(B, N) * N - dot(B, T) * T);

  if (dot(cross(N, T), B) < 0.0)
    T = T * -1.0;

  mat3 TBN = transpose(mat3(T, B, N));
  vs_out.TangentLightPos0 = TBN * (fragPos - light0_dir);
  vs_out.TangentLightPos1 = TBN * light1_pos;
  vs_out.TangentViewPos = TBN * viewPos;
  vs_out.TangentFragPos = TBN * fragPos;
  vs_out.FragPosLightSpace = lightSpaceMatrix * vec4(fragPos, 1.0);

  gl_Position = projection * view * model * vec4(aPos, 1.0);
}