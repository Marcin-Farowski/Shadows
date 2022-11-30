#include"VBO.h"

// Konstruktor generuj¹cy Vertex Buffer Object i linkuj¹cy go do wierzcho³ków
VBO::VBO(std::vector<Vertex>& vertices)
{
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ARRAY_BUFFER, ID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);
}

// Zbinduj Vertex Buffer Object okreœlaj¹c go jako GL_ARRAY_BUFFER
void VBO::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, ID);
}

// Zbinduj VBO do 0 aby póŸniej jej przypadkowo nie zmodyfikowaæ
void VBO::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

// Usuñ Vertex Buffer Object
void VBO::Delete()
{
	glDeleteBuffers(1, &ID);
}

