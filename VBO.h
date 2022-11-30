#ifndef VBO_CLASS_H
#define VBO_CLASS_H

#include<glm/glm.hpp>
#include<glad/glad.h>
#include<vector>

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 color;
	glm::vec2 texUV; // wspó³rzêdne tekstur
};

class VBO
{
public:
	GLuint ID;
	VBO(std::vector<Vertex>& vertices); // konstruktor przyjmuje wierzcho³ki i ich rozmiar w bajtach

	void Bind();
	void Unbind();
	void Delete();
};

#endif