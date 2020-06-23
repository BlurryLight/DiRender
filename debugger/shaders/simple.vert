#version 330 core
layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aNormal;
layout (location=2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    mat3 inverse_transpose = transpose(inverse(mat3(model)));
    vec3 Normal = inverse_transpose * aNormal;
    vec2 TexCoords = aTexCoords;
    vec4 Pos = projection * view * model * vec4(aPos,1.0);
    gl_Position = Pos;
}
