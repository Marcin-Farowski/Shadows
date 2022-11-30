#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include<glad/glad.h>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<cerrno>

// Wczytaj pliki tekstowe shaderów - zwraca zawartoœæ plików tekstowych jako string
std::string get_file_contents(const char* filename);

// Deklaracja klasy Shader, która jest Program Shaderem OpenGLa
class Shader
{
public:
	// WskaŸnik na Shader Program
	GLuint ID;
	// Konstruktor, który tworzy Shader Program z vertex shadera i fragment shadera
	Shader(const char* vertexFile, const char* fragmentFile);
	Shader(const char* vertexFile, const char* fragmentFile, const char* geometryFile);

	// Aktywuj Shader Program
	void Activate();
	// Usuñ Shader Program
	void Delete();
private:
	void compileErrors(unsigned int shader, const char* type);
};

#endif