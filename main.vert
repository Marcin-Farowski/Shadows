#version 330 core

// położenia wierzchołków
layout (location = 0) in vec3 aPos;
// normalne
layout (location = 1) in vec3 aNormal;
// kolory
layout (location = 2) in vec3 aColor;
// położenie tekstury
layout (location = 3) in vec2 aTex;


// zwraca aktualną pozycje dla Fragment Shadera
out vec3 crntPos;
// zwraca normalną dla Fragment Shadera
out vec3 Normal;
// zwraca kolor dla Fragment Shadera
out vec3 color;
// zwraca położenie tekstury dla Fragment Shadera
out vec2 texCoord;
// zwraca pozycje oświetlenia
out vec4 fragPosLight;


// wejście matrycy kamery z funkcji main
uniform mat4 camMatrix;
uniform mat4 model;
uniform mat4 translation;
uniform mat4 rotation;
uniform mat4 scale;
// pobiera matryce oświetlenia
uniform mat4 lightProjection;

void main()
{
    // oblicza aktualną pozycje
    crntPos = vec3(model * translation * rotation * scale * vec4(aPos, 1.0f));
    // przypisuje normalną z Vertex Data do "Normal"
    Normal = aNormal;
    // przypisuje kolory z Vertex Data do color
    color = aColor;
    // przypisuje położenie tekstury z Vertex Data
    texCoord = mat2(0.0, -1.0, 1.0, 0.0) * aTex;
    // oblicza pozycje oświetlenia dla Fragment Shadera
	fragPosLight = lightProjection * vec4(crntPos, 1.0f);

    // zwraca położenie wierzchołków
    gl_Position = camMatrix * vec4(crntPos, 1.0);
}