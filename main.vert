#version 330 core

// po�o�enia wierzcho�k�w
layout (location = 0) in vec3 aPos;
// normalne
layout (location = 1) in vec3 aNormal;
// kolory
layout (location = 2) in vec3 aColor;
// po�o�enie tekstury
layout (location = 3) in vec2 aTex;


// zwraca aktualn� pozycje dla Fragment Shadera
out vec3 crntPos;
// zwraca normaln� dla Fragment Shadera
out vec3 Normal;
// zwraca kolor dla Fragment Shadera
out vec3 color;
// zwraca po�o�enie tekstury dla Fragment Shadera
out vec2 texCoord;
// zwraca pozycje o�wietlenia
out vec4 fragPosLight;


// wej�cie matrycy kamery z funkcji main
uniform mat4 camMatrix;
uniform mat4 model;
uniform mat4 translation;
uniform mat4 rotation;
uniform mat4 scale;
// pobiera matryce o�wietlenia
uniform mat4 lightProjection;

void main()
{
    // oblicza aktualn� pozycje
    crntPos = vec3(model * translation * rotation * scale * vec4(aPos, 1.0f));
    // przypisuje normaln� z Vertex Data do "Normal"
    Normal = aNormal;
    // przypisuje kolory z Vertex Data do color
    color = aColor;
    // przypisuje po�o�enie tekstury z Vertex Data
    texCoord = mat2(0.0, -1.0, 1.0, 0.0) * aTex;
    // oblicza pozycje o�wietlenia dla Fragment Shadera
	fragPosLight = lightProjection * vec4(crntPos, 1.0f);

    // zwraca po�o�enie wierzcho�k�w
    gl_Position = camMatrix * vec4(crntPos, 1.0);
}