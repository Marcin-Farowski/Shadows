#include"VBO.h"

// Konstruktor generujący Vertex Buffer Object i linkujący go do wierzchołków
VBO::VBO(std::vector<Vertex>& vertices)
{
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
}

// Zbinduj Vertex Buffer Object określając go jako GL_ARRAY_BUFFER
void VBO::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

// Zbinduj VBO do 0 aby później jej przypadkowo nie zmodyfikować
void VBO::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// Usuń Vertex Buffer Object
void VBO::Delete()
{
	glDeleteBuffers(1, &ID);
}

