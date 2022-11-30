#include"Texture.h"

Texture::Texture(const char* image, const char* texType, GLuint slot)
{
	// Przypisz typ tekstury do obiektu tekstury
	type = texType;

	// Rozmiary tekstury oraz liczba kana��w koloru
	int widthImg, heightImg, numColCh;
	// Odwr�� vertykalnie tak aby pozycja grafiki zgadza�a si� z t� z OpenGL
	stbi_set_flip_vertically_on_load(true);
	// Wczytaj grafike z pliku
	unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, 0);

	// Wygeneruj teksture jako obiekt texture OpenGL
	glGenTextures(1, &ID);
	// Przypisz teksture do Texture Unit
	glActiveTexture(GL_TEXTURE0 + slot);
	unit = slot;
	glBindTexture(GL_TEXTURE_2D, ID);

	// Konfiguracja typu algorytmu do pomniejszania i powi�kszania grafiki
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// Konfiguracja sposobu powtarzania tekstury
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Sprawd� jaki typ kana��w koloru posiada tekstura i za�aduj odpowiednio
	if (numColCh == 4)
		glTexImage2D
		(
			GL_TEXTURE_2D,
			0,
			GL_SRGB_ALPHA,
			widthImg,
			heightImg,
			0,
			GL_RGBA,
			GL_UNSIGNED_BYTE,
			bytes
		);
	else if (numColCh == 3)
		glTexImage2D
		(
			GL_TEXTURE_2D,
			0,
			GL_SRGB,
			widthImg,
			heightImg,
			0,
			GL_RGB,
			GL_UNSIGNED_BYTE,
			bytes
		);
	else if (numColCh == 1)
		glTexImage2D
		(
			GL_TEXTURE_2D,
			0,
			GL_SRGB,
			widthImg,
			heightImg,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			bytes
		);
	else
		throw std::invalid_argument("Automatic Texture type recognition failed");

	// Wygeneruj MipMapy tekstury (tekstury o mniejszej rozdzielczo�ci)
	glGenerateMipmap(GL_TEXTURE_2D);

	// Usu� dane grafiki
	stbi_image_free(bytes);

	// Odbinduj teksture
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::texUnit(Shader& shader, const char* uniform, GLuint unit)
{
	// Pobierz ID zmiennej uniform z Shadera
	GLuint texUni = glGetUniformLocation(shader.ID, uniform);
	// Shader musi by� aktywowany przez zmian� warto�ci zmiennej uniform, kt�ra jest w nim zadeklarowana
	shader.Activate();
	// Ustaw warto�� zmiennej uniform
	glUniform1i(texUni, unit);
}

void Texture::Bind()
{
	// Przypisz teksture do Texture Unit
	glActiveTexture(GL_TEXTURE0 + unit);
	// Zbinduj teksture
	glBindTexture(GL_TEXTURE_2D, ID);
}

void Texture::Unbind()
{
	// Odbinduj teksture
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::Delete()
{
	// Usu� teksture
	glDeleteTextures(1, &ID);
}