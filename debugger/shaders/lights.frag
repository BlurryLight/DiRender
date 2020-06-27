#version 330 core
out vec4 FragColor;
uniform vec3 lightColor = vec3(1.0);
void main()
{
    FragColor = vec4(lightColor,1.0);
}
