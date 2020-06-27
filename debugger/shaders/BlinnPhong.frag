#version 330 core
out vec4 FragColor;

in VS_OUT
{
    vec3 Normal;
    vec2 TexCoords;
    vec3 WorldPos;
} fs_in;
struct Light
{
  vec3 Position;
  vec3 Color;
  vec3 Normal;
  //The magic number is from www.ogre3d.org/tikiwiki/tiki-index.php?page=-Point+Light+Attenuation
  //Which allows the light to cover about 600 range
  //Linear = 0.007
  //Quadratic = 0.0002;
  float Linear;
  float Quadratic;
};

uniform vec3 ambient_color;
uniform vec3 viewPos;
const int MAX_LIGHTS = 8;
uniform Light lights[MAX_LIGHTS];
uniform int lightNum = MAX_LIGHTS;
uniform float shininess  = 8.0;

vec4 BlinnPhongCalc(Light light)
{
    float r = distance(light.Position,fs_in.WorldPos);
    float attenuation = 1.0 / (1.0 + light.Linear * r + light.Quadratic * r * r);
    //ambient
    vec3 res = 0.3 * ambient_color;
    //diffuse
    vec3 lightDir = normalize(light.Position - fs_in.WorldPos);
    if(dot(lightDir,light.Normal) > 0.0) //object is behind the light
    {
        return vec4(res,1.0);
    }
    vec3 normal = normalize(fs_in.Normal);
    float LdotN = max(dot(lightDir,normal),0.0);
    res += LdotN * ambient_color * attenuation;

    //specular
    vec3 viewDir = normalize(viewPos - fs_in.WorldPos);
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float HdotN = max(dot(normal,halfwayDir),0.0);
    float spec_index = 0.0;
    if(HdotN > 0.0)
    {
        spec_index = pow(HdotN,shininess);
    }
    res+= spec_index * light.Color * attenuation;
    return vec4(res,1.0);
}

void main()
{
    vec4 res = vec4(0.0);
    for(int i =0;i<lightNum;i++)
{
    res+= BlinnPhongCalc(lights[i]);
    }
    FragColor = res;
}
