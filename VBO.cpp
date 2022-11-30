#include"VBO.h"

// Konstruktor generuj�cy Vertex Buffer Object i linkuj�cy go do wierzcho�k�w
VBO::VBO(std::vector<Vertex>& vertices)
{
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
}

// Zbinduj Vertex Buffer Object okre�laj�c go jako GL_ARRAY_BUFFER
void VBO::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

// Zbinduj VBO do 0 aby p�niej jej przypadkowo nie zmodyfikowa�
void VBO::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// Usu� Vertex Buffer Object
void VBO::Delete()
{
	glDeleteBuffers(1, &ID);
}

