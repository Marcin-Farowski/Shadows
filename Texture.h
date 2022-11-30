#ifndef TEXTURE_CLASS_H
#define TEXTURE_CLASS_H

#include<glad/glad.h>
#include<stb/stb_image.h>

#include"shaderClass.h"

class Texture
{
public:
	GLuint ID;
	const char* type;
	GLuint unit;

	Texture(const char* image, const char* texType, GLuint slot);

	// Przypisz Texture Unit do tekstury
	void texUnit(Shader& shader, const char* uniform, GLuint unit);
	// Binduje teksture
	void Bind();
	// Odbindowuje teksture
	void Unbind();
	// Usuwa teksture
	void Delete();
};
#endif