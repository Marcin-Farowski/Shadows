#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include<glad/glad.h>
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#include<glm/gtx/rotate_vector.hpp>
#include<glm/gtx/vector_angle.hpp>

#include"shaderClass.h"

class Camera
{
public:
	// przechowuje trzy g�owne wektory kamery
	glm::vec3 Position;
	glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 cameraMatrix = glm::mat4(1.0f);

	// zapobiega skakaniu kamery przy pierwszym klikni�ciu
	bool firstClick = true;


	// szeroko�� i wysoko�� okna
	int width;
	int height;

	// pr�dko�� przemieszczania si� kamery za pomoc� klawiatury
	float speed = 10.0f;
	// czu�o�� obracania kamery mysz�
	float sensitivity = 100.0f;

	// konstruktor kamery - ustawia pocz�tkowe warto�ci
	Camera(int width, int height, glm::vec3 position);

	// aktualizuje i eksportuje matryce kamery do Vertex Shadera
	void updateMatrix(float FOVdeg, float nearPlane, float farPlane);
	void Matrix(Shader& shader, const char* uniform);
	// obs�uga sterowania kamer�
	void Inputs(GLFWwindow* window);
};


#endif
