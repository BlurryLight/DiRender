#version 330 core
layout (points) in;
layout (line_strip, max_vertices = 2) out;
uniform mat4 projection;
in VS_OUT {
    vec4 rayPos;
    vec4 rayIsectCoords;
 //   vec3 rayIsectNormal;
} gs_in[];
void main() {
    gl_Position = projection *gs_in[0].rayPos;
    EmitVertex();
    gl_Position = projection * gs_in[0].rayIsectCoords;
    EmitVertex();
    EndPrimitive();
}

