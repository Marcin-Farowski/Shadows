#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include<glad/glad.h>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>

// Wczytaj pliki tekstowe shader�w - zwraca zawarto�� plik�w tekstowych jako string
std::string get_file_contents(const char* filename);

// Deklaracja klasy Shader, kt�ra jest Program Shaderem OpenGLa
class Shader
{
public:
	// Wska�nik na Shader Program
	GLuint ID;
	// Konstruktor, kt�ry tworzy Shader Program z vertex shadera i fragment shadera
	Shader(const char* vertexFile, const char* fragmentFile);
	Shader(const char* vertexFile, const char* fragmentFile, const char* geometryFile);

	// Aktywuj Shader Program
	void Activate();
	// Usu� Shader Program
	void Delete();
private:
	void compileErrors(unsigned int shader, const char* type);
};

#endif