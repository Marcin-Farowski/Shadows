#version 330 core

// po³o¿enia wierzcho³ków
layout (location = 0) in vec3 aPos;
// normalne
layout (location = 1) in vec3 aNormal;
// kolory
layout (location = 2) in vec3 aColor;
// po³o¿enie tekstury
layout (location = 3) in vec2 aTex;


// zwraca aktualn¹ pozycje dla Fragment Shadera
out vec3 crntPos;
// zwraca normaln¹ dla Fragment Shadera
out vec3 Normal;
// zwraca kolor dla Fragment Shadera
out vec3 color;
// zwraca po³o¿enie tekstury dla Fragment Shadera
out vec2 texCoord;
// zwraca pozycje oœwietlenia
out vec4 fragPosLight;


// wejœcie matrycy kamery z funkcji main
uniform mat4 camMatrix;
uniform mat4 model;
uniform mat4 translation;
uniform mat4 rotation;
uniform mat4 scale;
// pobiera matryce oœwietlenia
uniform mat4 lightProjection;

void main()
{
    // oblicza aktualn¹ pozycje
    crntPos = vec3(model * translation * rotation * scale * vec4(aPos, 1.0f));
    // przypisuje normaln¹ z Vertex Data do "Normal"
    Normal = aNormal;
    // przypisuje kolory z Vertex Data do color
    color = aColor;
    // przypisuje po³o¿enie tekstury z Vertex Data
    texCoord = mat2(0.0, -1.0, 1.0, 0.0) * aTex;
    // oblicza pozycje oœwietlenia dla Fragment Shadera
	fragPosLight = lightProjection * vec4(crntPos, 1.0f);

    // zwraca po³o¿enie wierzcho³ków
    gl_Position = camMatrix * vec4(crntPos, 1.0);
}