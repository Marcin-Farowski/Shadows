#include"VAO.h"

// Konstruktor generuj�cy Vertex Array Object ID
VAO::VAO()
{
	glGenVertexArrays(1, &ID);
}

// Zkonfiguruj Vertex Attribute po to aby OpenGL wiedzia� w jaki spos�b odczytywa� VBO
// W��cz Vertex Attribute po to aby OpenGL wiedzia� �e ma go u�y�
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

// Zbinduj VAO do 0 aby p�niej go przypadkowo nie zmodyfikowa�
void VAO::Unbind()
{
	glBindVertexArray(0);
}

// Usu� Vertex Array Object
void VAO::Delete()
{
	glDeleteVertexArrays(1, &ID);
}