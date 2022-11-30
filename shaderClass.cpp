#include"shaderClass.h"

// Wczytuje plik tekstowy i zwraca jego zawarto�� jako string
std::string get_file_contents(const char* filename)
{
	std::ifstream in(filename, std::ios::binary);
	if (in)
	{
		std::string contents;
		in.seekg(0, std::ios::end);
		contents.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&contents[0], contents.size());
		in.close();
		return(contents);
	}
	throw(errno);
}

// Konstruktor Shadera
Shader::Shader(const char* vertexFile, const char* fragmentFile)
{
	// Pobierz zawarto�� plik�w vertex i fragment shader�w
	std::string vertexCode = get_file_contents(vertexFile);
	std::string fragmentCode = get_file_contents(fragmentFile);

	// Przekonwertuj je i zapisz w tablicy znak�w
	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();

	// Stw�rz Vetex Shader Object i zapisz wska�nik do niego
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Przypisanie kodu Vertex Shadera do Vertex Shader Object
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	// Skompiluj kod vertexShadera do kodu maszynowego
	glCompileShader(vertexShader);
	// Sprawd� czy Shader zosta� pomy�lnie skompilowany
	compileErrors(vertexShader, "VERTEX");

	// Stw�rz Fragment Shader Object i zapisz wska�nik do niego
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// Przypisanie kodu Fragment Shadera do Fragment Shader Object
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	// Skompiluj kod fragmentShadera do kodu maszynowego
	glCompileShader(fragmentShader);
	// Sprawd� czy Shader zosta� pomy�lnie skompilowany
	compileErrors(fragmentShader, "FRAGMENT");

	// Stw�rz Shader Program Object i zapisz wska�nik do niego
	ID = glCreateProgram();

	// Do��cz Vertex Shader do Shader Program
	glAttachShader(ID, vertexShader);
	// D��cz Fragment Shader do Shader Program
	glAttachShader(ID, fragmentShader);
	// Zlinkuj wszystkie shadery w Shader Program
	glLinkProgram(ID);
	// Sprawd� czy Shadery zosta�y pomy�lnie zlinkowane
	compileErrors(ID, "PROGRAM");

	// Usu� vertexShader
	glDeleteShader(vertexShader);
	// Usu� fragmentShader
	glDeleteShader(fragmentShader);
}

Shader::Shader(const char* vertexFile, const char* fragmentFile, const char* geometryFile)
{
	// Zapisz zawarto�ci vertexFile, fragmentFile i geometryFile jako zmienne typu string
	std::string vertexCode = get_file_contents(vertexFile);
	std::string fragmentCode = get_file_contents(fragmentFile);
	std::string geometryCode = get_file_contents(geometryFile);

	// Przekonwertuj stringi na tablice znag�w
	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();
	const char* geometrySource = geometryCode.c_str();

	// Stw�rz Vertex Shader Object
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Do��cz Vertex Shader do Vertex Shader Object
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	// Skompiluj Vertex Shader na kod maszynowy
	glCompileShader(vertexShader);
	// Sprawdza czy Shader zosta� skompilowany pomy�lnie
	compileErrors(vertexShader, "VERTEX");

	// Stw�rz Fragment Shader Object
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// Do��cz Fragment Shader source do Fragment Shader Object
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	// Skompiluj Fragment Shader na kod maszynowy
	glCompileShader(fragmentShader);
	// Sprawdza czy Shader zosta� skompilowany pomy�lnie
	compileErrors(fragmentShader, "FRAGMENT");

	// Stw�rz Geometry Shader Object
	GLuint geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
	// Do��cz Geometry Shader source do Fragment Shader Object
	glShaderSource(geometryShader, 1, &geometrySource, NULL);
	// Skompiluj Geometry Shader na kod maszynowy
	glCompileShader(geometryShader);
	// Sprawdza czy Shader zosta� skompilowany pomy�lnie
	compileErrors(geometryShader, "GEOMETRY");

	// Stw�rz Shader Program Object
	ID = glCreateProgram();
	// Do��cz Vertex Shader i Fragment Shader do Shader Program
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glAttachShader(ID, geometryShader);
	// Zlinkuj wszystkie shadery razem w Shader Program
	glLinkProgram(ID);
	// Sprawd� czy Shadery zosta�y zlinkowane pomy�lnie
	compileErrors(ID, "PROGRAM");

	// Usu� niepotrzebne ju� obiekty
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glDeleteShader(geometryShader);
}

void Shader::Activate()
{
	glUseProgram(ID);
}

void Shader::Delete()
{
	glDeleteProgram(ID);
}

// Sprawd� czy shadery zosta�y skompilowane poprawnie
void Shader::compileErrors(unsigned int shader, const char* type)
{
	GLint hasCompiled;
	char infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER_COMPILATION_ERROR for:" << type << "\n" << std::endl;
		}
	}
	else
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &hasCompiled);
		if (hasCompiled == GL_FALSE)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cout << "SHADER_LINKING_ERROR for:" << type << "\n" << std::endl;
		}
	}
}