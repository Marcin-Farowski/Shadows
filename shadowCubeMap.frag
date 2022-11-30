#version 330 core
in vec4 FragPos;

// pozycja oœwietlenia w œwiecie
uniform vec3 lightPos;
uniform float farPlane;

void main()
{
    gl_FragDepth = length(FragPos.xyz - lightPos) / farPlane;
}