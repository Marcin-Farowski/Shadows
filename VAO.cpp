#include"VAO.h"

// Konstruktor generuj¹cy Vertex Array Object ID
VAO::VAO()
{
	glGenVertexArrays(1, &ID);
}

// Zkonfiguruj Vertex Attribute po to aby OpenGL wiedzia³ w jaki sposób odczytywaæ VBO
// W³¹cz Vertex Attribute po to aby OpenGL wiedzia³ ¿e ma go u¿yæ
void VAO::LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset)
{
	VBO.Bind();
	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layout);
	VBO.Unbind();
}

// Ustaw VAO jako aktualny Vertex Array Object poprzez zbindowanie go
void VAO::Bind()
{
	glBindVertexArray(ID);
}

// Zbinduj VAO do 0 aby póŸniej go przypadkowo nie zmodyfikowaæ
void VAO::Unbind()
{
	glBindVertexArray(0);
}

// Usuñ Vertex Array Object
void VAO::Delete()
{
	glDeleteVertexArrays(1, &ID);
}