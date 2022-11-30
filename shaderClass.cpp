#include"shaderClass.h"

// Wczytuje plik tekstowy i zwraca jego zawartoœæ jako string
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
	// Pobierz zawartoœæ plików vertex i fragment shaderów
	std::string vertexCode = get_file_contents(vertexFile);
	std::string fragmentCode = get_file_contents(fragmentFile);

	// Przekonwertuj je i zapisz w tablicy znaków
	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();

	// Stwórz Vetex Shader Object i zapisz wskaŸnik do niego
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Przypisanie kodu Vertex Shadera do Vertex Shader Object
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	// Skompiluj kod vertexShadera do kodu maszynowego
	glCompileShader(vertexShader);
	// SprawdŸ czy Shader zosta³ pomyœlnie skompilowany
	compileErrors(vertexShader, "VERTEX");

	// Stwórz Fragment Shader Object i zapisz wskaŸnik do niego
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// Przypisanie kodu Fragment Shadera do Fragment Shader Object
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	// Skompiluj kod fragmentShadera do kodu maszynowego
	glCompileShader(fragmentShader);
	// SprawdŸ czy Shader zosta³ pomyœlnie skompilowany
	compileErrors(fragmentShader, "FRAGMENT");

	// Stwórz Shader Program Object i zapisz wskaŸnik do niego
	ID = glCreateProgram();

	// Do³¹cz Vertex Shader do Shader Program
	glAttachShader(ID, vertexShader);
	// D³¹cz Fragment Shader do Shader Program
	glAttachShader(ID, fragmentShader);
	// Zlinkuj wszystkie shadery w Shader Program
	glLinkProgram(ID);
	// SprawdŸ czy Shadery zosta³y pomyœlnie zlinkowane
	compileErrors(ID, "PROGRAM");

	// Usuñ vertexShader
	glDeleteShader(vertexShader);
	// Usuñ fragmentShader
	glDeleteShader(fragmentShader);
}

Shader::Shader(const char* vertexFile, const char* fragmentFile, const char* geometryFile)
{
	// Zapisz zawartoœci vertexFile, fragmentFile i geometryFile jako zmienne typu string
	std::string vertexCode = get_file_contents(vertexFile);
	std::string fragmentCode = get_file_contents(fragmentFile);
	std::string geometryCode = get_file_contents(geometryFile);

	// Przekonwertuj stringi na tablice znagów
	const char* vertexSource = vertexCode.c_str();
	const char* fragmentSource = fragmentCode.c_str();
	const char* geometrySource = geometryCode.c_str();

	// Stwórz Vertex Shader Object
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Do³¹cz Vertex Shader do Vertex Shader Object
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	// Skompiluj Vertex Shader na kod maszynowy
	glCompileShader(vertexShader);
	// Sprawdza czy Shader zosta³ skompilowany pomyœlnie
	compileErrors(vertexShader, "VERTEX");

	// Stwórz Fragment Shader Object
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// Do³¹cz Fragment Shader source do Fragment Shader Object
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	// Skompiluj Fragment Shader na kod maszynowy
	glCompileShader(fragmentShader);
	// Sprawdza czy Shader zosta³ skompilowany pomyœlnie
	compileErrors(fragmentShader, "FRAGMENT");

	// Stwórz Geometry Shader Object
	GLuint geometryShader = glCreateShader(GL_GEOMETRY_SHADER);
	// Do³¹cz Geometry Shader source do Fragment Shader Object
	glShaderSource(geometryShader, 1, &geometrySource, NULL);
	// Skompiluj Geometry Shader na kod maszynowy
	glCompileShader(geometryShader);
	// Sprawdza czy Shader zosta³ skompilowany pomyœlnie
	compileErrors(geometryShader, "GEOMETRY");

	// Stwórz Shader Program Object
	ID = glCreateProgram();
	// Do³¹cz Vertex Shader i Fragment Shader do Shader Program
	glAttachShader(ID, vertexShader);
	glAttachShader(ID, fragmentShader);
	glAttachShader(ID, geometryShader);
	// Zlinkuj wszystkie shadery razem w Shader Program
	glLinkProgram(ID);
	// SprawdŸ czy Shadery zosta³y zlinkowane pomyœlnie
	compileErrors(ID, "PROGRAM");

	// Usuñ niepotrzebne ju¿ obiekty
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

// SprawdŸ czy shadery zosta³y skompilowane poprawnie
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