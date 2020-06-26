#version 330 core
out vec4 FragColor;

in VS_OUT
{
    vec3 Normal;
    vec2 TexCoords;
    vec3 WorldPos;
} fs_in;

uniform vec3 ambient_color;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor = vec3(0.5);
uniform float shininess  = 8.0;

vec4 BlinnPhongCalc()
{
    //ambient
    vec3 res = 0.3 * ambient_color;
    //diffuse
    vec3 lightDir = normalize(lightPos - fs_in.WorldPos);
    vec3 normal = normalize(fs_in.Normal);
    float LdotN = max(dot(lightDir,normal),0.0);
    res += LdotN * ambient_color;

    //specular
    vec3 viewDir = normalize(viewPos - fs_in.WorldPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float HdotN = max(dot(normal,halfwayDir),0.0);
    float spec_index = 0.0;
    if(HdotN > 0.0)
    {
        spec_index = pow(HdotN,shininess);
    }
    res+= spec_index * lightColor;
    return vec4(res,1.0);
}

void main()
{
    FragColor = BlinnPhongCalc();
}
