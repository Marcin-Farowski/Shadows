#include"EBO.h"

// Konstruktor generuj¹cy Element Array Object i linkuj¹cy go do indeksów
EBO::EBO(std::vector<GLuint>& indices)
{
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
}

// Zbinduj Element Buffer Object okreœlaj¹c go jako GL_ELEMENT_ARRAY_BUFFER
void EBO::Bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

// Zbinduj EBO do 0 aby póŸniej jej przypadkowo nie zmodyfikowaæ
void EBO::Unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// Usuñ Element Buffer Object
void EBO::Delete()
{
	glDeleteBuffers(1, &ID);
}

