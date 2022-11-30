#include"EBO.h"

// Konstruktor generuj�cy Element Array Object i linkuj�cy go do indeks�w
EBO::EBO(std::vector<GLuint>& indices)
{
	glGenBuffers(1, &ID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
}

// Zbinduj Element Buffer Object okre�laj�c go jako GL_ELEMENT_ARRAY_BUFFER
void EBO::Bind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

// Zbinduj EBO do 0 aby p�niej jej przypadkowo nie zmodyfikowa�
void EBO::Unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// Usu� Element Buffer Object
void EBO::Delete()
{
	glDeleteBuffers(1, &ID);
}

