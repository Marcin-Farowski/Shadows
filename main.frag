#version 330 core

// zwraca kolory w RGBA
out vec4 FragColor;

// aktualna pozycja z Vertex Shadera
in vec3 crntPos;
// normalna z Vertex Shadera
in vec3 Normal;
// color z Vertex Shadera
in vec3 color;
// po³o¿enie tekstury z Vertex Shadera
in vec2 texCoord;
// pozycja oœwietlenia
in vec4 fragPosLight;

// Texture Unit z funkcji main
uniform sampler2D diffuse0;

// Mapa specular light dla tekstury z funkcji main
uniform sampler2D specular0;

uniform sampler2D shadowMap;
uniform samplerCube shadowCubeMap;

// Pobiera kolor oœwietlenia z funkcji main
uniform vec4 lightColor;
// Pobiera pozycje oœwietlenia z funkcji main
uniform vec3 lightPos;
// Pobiera pozycje kamery z funkcji main
uniform vec3 camPos;
uniform float farPlane;


vec4 pointLight()
{	
	vec3 lightVec = lightPos - crntPos;

	// Sntensywnoœæ œwiat³a w zale¿noœci od dystansu do powierzchni
	float dist = length(lightVec);
	float a = 0.0003f;
	float b = 0.00002f;
	float inten = 1.0f / (a * dist * dist + b * dist + 1.0f);

	// Oœwietlenie otoczenia
	float ambient = 0.20f;

	// Diffuse lighting
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightVec);
	float diffuse = max(dot(normal, lightDirection), 0.0f);

	// Specular lighting
	float specular = 0.0f;
	if (diffuse != 0.0f)
	{
		float specularLight = 0.50f;
		vec3 viewDirection = normalize(camPos - crntPos);
		vec3 halfwayVec = normalize(viewDirection + lightDirection);
		float specAmount = pow(max(dot(normal, halfwayVec), 0.0f), 16);
		specular = specAmount * specularLight;
	};

	// Wartoœæ cienia
	float shadow = 0.0f;
	vec3 fragToLight = crntPos - lightPos;
	float currentDepth = length(fragToLight);
	float bias = max(0.5f * (1.0f - dot(normal, lightDirection)), 0.0005f); 

	int sampleRadius = 2;
	float offset = 0.02f;
	for(int z = -sampleRadius; z <= sampleRadius; z++)
	{
		for(int y = -sampleRadius; y <= sampleRadius; y++)
		{
		    for(int x = -sampleRadius; x <= sampleRadius; x++)
		    {
		        float closestDepth = texture(shadowCubeMap, fragToLight + vec3(x, y, z) * offset).r;
				closestDepth *= farPlane;
				if (currentDepth > closestDepth + bias)
					shadow += 1.0f;     
		    }    
		}
	}
	
	shadow /= pow((sampleRadius * 2 + 1), 3);

	return (texture(diffuse0, texCoord) * (diffuse * (1.0f - shadow) * inten + ambient) + texture(specular0, texCoord).r * specular * (1.0f - shadow) * inten) * lightColor;
}

void main()
{
	FragColor = pointLight();
}