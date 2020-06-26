#version 330 core
layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aNormal;
layout (location=2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out VS_OUT
{
    vec3 Normal;
    vec2 TexCoords;
    vec3 WorldPos;
} vs_out;

void main()
{
    mat3 inverse_transpose = transpose(inverse(mat3(model)));
    vs_out.Normal = inverse_transpose * aNormal;
    vs_out.TexCoords = aTexCoords;
    vec4 pos = model * vec4(aPos,1.0);
    vs_out.WorldPos = vec3(pos);
    gl_Position = projection * view * pos;
}
