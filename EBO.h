#ifndef EBO_CLASS_H
#define EBO_CLASS_H

#include<glad/glad.h>
#include<vector>

class EBO
{
public:
	GLuint ID;
	EBO(std::vector<GLuint>& indices); // konstruktor przyjmuje indeksy i ich rozmiar w bajtach

	void Bind();
	void Unbind();
	void Delete();
};

#endif