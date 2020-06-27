#version 330 core
layout (location=0) in vec3 rayPos;
layout (location=1) in vec3 rayIsectCoords;
layout (location=2) in vec3 rayIsectNormal;

uniform mat4 model;
uniform mat4 view;
out VS_OUT
{
    vec4 rayPos;
    vec4 rayIsectCoords;
  //  vec3 rayIsectNormal;
} vs_out;

void main()
{
    vs_out.rayPos = view * model * vec4(rayPos,1.0);
   // vs_out.rayIsectNormal = rayIsectNormal;
    vs_out.rayIsectCoords = view * model * vec4(rayIsectCoords,1.0);
    gl_Position = vs_out.rayPos;
}
