

#include"Model.h"


const unsigned int width = 1280;
const unsigned int height = 720;

// Liczba pr�bek na piksel dla MSAA
unsigned int samples = 8;

// Gamma
float gamma = 2.2f;


// Atrybuty wierzcho�k�w dla czworok�ta, kt�ry wype�nia ca�y ekran w znormalizowanych wsp�rz�dnych urz�dzenia
// Wyg�adzanie Kraw�dzi / Anti Aliasing
float rectangleVertices[] =
{
	//Wsp�rz�dne// Wsp�rz�dne tekstury
	 1.0f, -1.0f,  1.0f, 0.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	-1.0f,  1.0f,  0.0f, 1.0f,

	 1.0f,  1.0f,  1.0f, 1.0f,
	 1.0f, -1.0f,  1.0f, 0.0f,
	-1.0f,  1.0f,  0.0f, 1.0f
};


int main()
{
	// Inicjalizacja Graphics Library Framework (lightweight utility library for use with OpenGL)
	// Allows to create and manage windows and OpenGL contexts, as well as handle joystick, keyboard and mouse input
	glfwInit();

	// Hint - wskaz�wka dla GLFW, �e b�dziemy wykorzystywa� wersj� 3.3 OpenGL
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// B�dziemy u�ywa� profilu CORE, kt�ry zawiera nowocze�niejsze wersje funkcji
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Tworzenie okna o rozmiarze 1280 x 720 i tytule "Shadows"
	GLFWwindow* window = glfwCreateWindow(width, height, "Shadows", NULL, NULL);
	// Je�li nie uda�o si� utworzy� okna
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// GLFW ustaw okno jako cz�� bie��cego kontekstu
	glfwMakeContextCurrent(window);

	// glad wczytaj wymagane konfiguracje OpenGL
	gladLoadGL();
	// viewport naszego okna czyli obszar okna, na kt�rym chcemy �eby OpenGL renderowa�
	glViewport(0, 0, width, height);

	// Wygeneruj Shadery
	Shader shaderProgram("main.vert", "main.frag");
	Shader framebufferProgram("framebuffer.vert", "framebuffer.frag");
	Shader shadowMapProgram("shadowMap.vert", "shadowMap.frag");
	Shader shadowCubeMapProgram("shadowCubeMap.vert", "shadowCubeMap.frag", "shadowCubeMap.geom");

	// Kolor �wiat�a
	glm::vec4 lightColor = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	// Pozycja �wiat�a
	glm::vec3 lightPos = glm::vec3(0.0f, 4.0f, 0.0f);

	// Aktywuj program sk�adaj�cy si� z shader�w "main" vertex i fragment
	shaderProgram.Activate();
	// Ustaw warto�� zmiennej uniform "lightColor" odpowiadaj�cej za kolor o�wietlenia w Shaderze main.frag
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	// Ustaw warto�� zmiennej uniform "lightColor" odpowiadaj�cej za pozycj� o�wietlenia w Shaderze main.frag
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
	// Aktywuj program sk�adaj�cy si� z shader�w "framebuffer" vertex i fragment
	framebufferProgram.Activate();
	// Ustaw warto�� zmiennej uniform "screenTexture" odpowiadaj�cej za tekstur� w Shaderze framebuffer.frag
	glUniform1i(glGetUniformLocation(framebufferProgram.ID, "screenTexture"), 0);
	// Ustaw warto�� zmiennej uniform "gamma" odpowiadaj�cej za gamm� w Shaderze framebuffer.frag
	glUniform1f(glGetUniformLocation(framebufferProgram.ID, "gamma"), gamma);




	// W��cz depth testing
	glEnable(GL_DEPTH_TEST);
	// W��cz Multisampling
	glEnable(GL_MULTISAMPLE);
	// W��cz Face Culling
	glEnable(GL_CULL_FACE);
	// Okre�l kt�r� strone tr�jk�ta zachowa�
	glCullFace(GL_FRONT);
	// Okre�l standard, kt�ry chcemy u�ywa� - Counter-Clockwise
	glFrontFace(GL_CCW);


	// Utw�rz obiekt Camera
	Camera camera(width, height, glm::vec3(0.0f, 10.0f, 60.0f));

	// Wczytaj model
	Model camping("models/camping/scene.gltf");


	// Przygotowanie prostok�tu framebuffera VBO i VAO
	unsigned int rectVAO, rectVBO;
	glGenVertexArrays(1, &rectVAO);
	glGenBuffers(1, &rectVBO);
	glBindVertexArray(rectVAO);
	glBindBuffer(GL_ARRAY_BUFFER, rectVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVertices), &rectangleVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));


	// Pomocnicze zmienne do obliczania FPS
	double prevTime = 0.0;
	double crntTime = 0.0;
	double timeDiff;
	unsigned int counter = 0;


	// Stw�rz Frame Buffer Object
	unsigned int FBO;
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	// Stw�rz Framebuffer Texture
	unsigned int framebufferTexture;
	glGenTextures(1, &framebufferTexture);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, framebufferTexture);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, GL_RGB16F, width, height, GL_TRUE);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // Zapobiega edge bleedingowi
	glTexParameteri(GL_TEXTURE_2D_MULTISAMPLE, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Zapobiega edge bleedingowi
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, framebufferTexture, 0);

	// Stw�rz Render Buffer Object
	unsigned int RBO;
	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);


	// Sprawdzanie b��d�w framebuffera
	auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer error: " << fboStatus << std::endl;

	// Stw�rz Frame Buffer Object
	unsigned int postProcessingFBO;
	glGenFramebuffers(1, &postProcessingFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, postProcessingFBO);

	// Stw�rz Framebuffer Texture
	unsigned int postProcessingTexture;
	glGenTextures(1, &postProcessingTexture);
	glBindTexture(GL_TEXTURE_2D, postProcessingTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, postProcessingTexture, 0);

	// Sprawdzanie b��d�w framebuffera
	fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Post-Processing Framebuffer error: " << fboStatus << std::endl;


	// Framebuffer dla Shadow Map
	unsigned int shadowMapFBO;
	glGenFramebuffers(1, &shadowMapFBO);

	// Texture dla Shadow Map FBO
	unsigned int shadowMapWidth = 2048, shadowMapHeight = 2048;
	unsigned int shadowMap;
	glGenTextures(1, &shadowMap);
	glBindTexture(GL_TEXTURE_2D, shadowMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowMapWidth, shadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	// Zapobiega ciemnno�ci poza frustum (�ci�tym sto�kiem)
	float clampColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, clampColor);

	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap, 0);
	// Wy��cza zapis do bufora koloru
	glDrawBuffer(GL_NONE);
	// Wy��cza odczyt z bufora koloru
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	// Matryce potrzebne do perspektywy o�wietlenia
	float farPlane = 1000.0f;
	glm::mat4 orthgonalProjection = glm::ortho(-35.0f, 35.0f, -35.0f, 35.0f, 0.1f, farPlane);
	glm::mat4 perspectiveProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, farPlane);
	glm::mat4 lightView = glm::lookAt(lightPos, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	glm::mat4 lightProjection = perspectiveProjection * lightView;

	shadowMapProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shadowMapProgram.ID, "lightProjection"), 1, GL_FALSE, glm::value_ptr(lightProjection));



	// Framebuffer dla Cubemap Shadow Map
	unsigned int pointShadowMapFBO;
	glGenFramebuffers(1, &pointShadowMapFBO);

	// Texture dla Cubemap Shadow Map FBO
	unsigned int depthCubemap;
	glGenTextures(1, &depthCubemap);

	glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);

	for (unsigned int i = 0; i < 6; ++i) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
			shadowMapWidth, shadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glBindFramebuffer(GL_FRAMEBUFFER, pointShadowMapFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	// Macierze potrzebne do perspektywy o�wietlenia na wszystkich stronach cubemap
	glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, farPlane);
	glm::mat4 shadowTransforms[] =
	{
	shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)),
	shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0)),
	shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0)),
	shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0)),
	shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0)),
	shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0))
	};
	// Wyeksportuj wszystkie macierze do shadera
	shadowCubeMapProgram.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shadowCubeMapProgram.ID, "shadowMatrices[0]"), 1, GL_FALSE, glm::value_ptr(shadowTransforms[0]));
	glUniformMatrix4fv(glGetUniformLocation(shadowCubeMapProgram.ID, "shadowMatrices[1]"), 1, GL_FALSE, glm::value_ptr(shadowTransforms[1]));
	glUniformMatrix4fv(glGetUniformLocation(shadowCubeMapProgram.ID, "shadowMatrices[2]"), 1, GL_FALSE, glm::value_ptr(shadowTransforms[2]));
	glUniformMatrix4fv(glGetUniformLocation(shadowCubeMapProgram.ID, "shadowMatrices[3]"), 1, GL_FALSE, glm::value_ptr(shadowTransforms[3]));
	glUniformMatrix4fv(glGetUniformLocation(shadowCubeMapProgram.ID, "shadowMatrices[4]"), 1, GL_FALSE, glm::value_ptr(shadowTransforms[4]));
	glUniformMatrix4fv(glGetUniformLocation(shadowCubeMapProgram.ID, "shadowMatrices[5]"), 1, GL_FALSE, glm::value_ptr(shadowTransforms[5]));
	glUniform3f(glGetUniformLocation(shadowCubeMapProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
	glUniform1f(glGetUniformLocation(shadowCubeMapProgram.ID, "farPlane"), farPlane);



	// P�tla g��wna
	// Dop�ki nie klikniemy przycisku zamknij okno lub inna funkcja nie za��da zamkni�cia okna
	while (!glfwWindowShouldClose(window))
	{
		// Licznik FPS
		crntTime = glfwGetTime();
		timeDiff = crntTime - prevTime;
		counter++;
		if (timeDiff >= 1.0 / 144.0)
		{
			std::string FPS = std::to_string((1.0 / timeDiff) * counter);
			std::string ms = std::to_string((timeDiff / counter) * 1000);
			//std::string Title = FPS + "FPS | " + ms + "ms";
			//glfwSetWindowTitle(window, Title.c_str());

			// Zresetuj timer i counter
			prevTime = crntTime;
			counter = 0;

			// Obs�uga sterowania kamer�. Gdy mamy wy��czony VSync powinna by� tutaj aby czu�o�� kamery nie zale�a�a od liczby FPS
			camera.Inputs(window);
		}		

		framebufferProgram.Activate();

		// Depth testing potrzebny do Shadow Map
		glEnable(GL_DEPTH_TEST);

		// Przygotowanie dla Shadow Map
		glViewport(0, 0, shadowMapWidth, shadowMapHeight);

		// Point Lights
		glBindFramebuffer(GL_FRAMEBUFFER, pointShadowMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);

		// Narysuj model dla shadow mapy
		camping.Draw(shadowCubeMapProgram, camera);


		// Prze��cz na domy�lny Framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		// Prze��cz na domy�lny Viewport
		glViewport(0, 0, width, height);
		// Zbinduj niestandardowy framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, FBO);
		// Okre�l kolor t�a
		glClearColor(pow(0.07f, gamma), pow(0.13f, gamma), pow(0.17f, gamma), 1.0f);
		// Wyczy�� back buffer i depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// W��cz depth desting. Jest wy��czony od momentu rysowania prostok�ta Framebuffera
		glEnable(GL_DEPTH_TEST);

		// Aktualizuje i eksportuje matryce kamery do Vertex Shadera
		camera.updateMatrix(45.0f, 0.1f, farPlane);


		// Wy�lij matrycje o�wietlenia do Shadera
		shaderProgram.Activate();
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "lightProjection"), 1, GL_FALSE, glm::value_ptr(lightProjection));
		glUniform1f(glGetUniformLocation(shaderProgram.ID, "farPlane"), farPlane);

		// Zbinduj Cubemap Shadow Map
		glActiveTexture(GL_TEXTURE0 + 2);
		glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
		glUniform1i(glGetUniformLocation(shaderProgram.ID, "shadowCubeMap"), 2);

		// Narysuj model
		camping.Draw(shaderProgram, camera);

		// Wczytaj FBO multisampling
		glBindFramebuffer(GL_READ_FRAMEBUFFER, FBO);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, postProcessingFBO);
		// Zako�cz multisampling i skopuj do post-processing FBO
		glBlitFramebuffer(0, 0, width, height, 0, 0, width, height, GL_COLOR_BUFFER_BIT, GL_NEAREST);


		// Zbinduj domy�lny framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		// Narysuj prostok�t framebuffera
		framebufferProgram.Activate();
		glBindVertexArray(rectVAO);
		glDisable(GL_DEPTH_TEST); // zapobiega odrzucaniu prostok�ta framebuffera
		glActiveTexture(GL_TEXTURE0 + 0);
		glBindTexture(GL_TEXTURE_2D, postProcessingTexture);
		glDrawArrays(GL_TRIANGLES, 0, 6);


		// zamie� bufory klatek obrazu (front buffer - back buffer)
		glfwSwapBuffers(window);
		// GLFW przetwarzaj wszystkie "polled events" takie jak pojawienie si� okna, zmiana jego rozmiaru itd
		glfwPollEvents();
	}



	// usu� stworzone obiekty
	shaderProgram.Delete();
	glDeleteFramebuffers(1, &FBO);
	glDeleteFramebuffers(1, &postProcessingFBO);

	// Usu� okno
	glfwDestroyWindow(window);
	// Zako�cz GLFW
	glfwTerminate();
	return 0;
}