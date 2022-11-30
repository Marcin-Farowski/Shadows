#ifndef MODEL_CLASS_H
#define MODEL_CLASS_H

#include<json/json.h>
#include"Mesh.h"

using json = nlohmann::json;


class Model
{
public:
	// Wczytuje model z pliku
	Model(const char* file);

	void Draw(Shader& shader, Camera& camera);

private:
	const char* file;
	std::vector<unsigned char> data;
	json JSON;

	// Wszystkie meshe i transformacje
	std::vector<Mesh> meshes;
	std::vector<glm::vec3> translationsMeshes;
	std::vector<glm::quat> rotationsMeshes;
	std::vector<glm::vec3> scalesMeshes;
	std::vector<glm::mat4> matricesMeshes;

	// Zapobiega wczytaniu tekstury dwa razy
	std::vector<std::string> loadedTexName;
	std::vector<Texture> loadedTex;

	// Wczytuje pojedynczy mesh wykorzystuj¹c jego index
	void loadMesh(unsigned int indMesh);

	// Trawersuje rekurencyjnie node
	void traverseNode(unsigned int nextNode, glm::mat4 matrix = glm::mat4(1.0f));

	// Pobiera dane binarne z pliku
	std::vector<unsigned char> getData();
	// Interpretuje dane binarne na floaty, indeksy i tekstury
	std::vector<float> getFloats(json accessor);
	std::vector<GLuint> getIndices(json accessor);
	std::vector<Texture> getTextures();

	// £¹czy floaty w wierzcho³ki
	std::vector<Vertex> assembleVertices
	(
		std::vector<glm::vec3> positions,
		std::vector<glm::vec3> normals,
		std::vector<glm::vec2> texUVs
	);

	// Grupuje powy¿sze floaty
	std::vector<glm::vec2> groupFloatsVec2(std::vector<float> floatVec);
	std::vector<glm::vec3> groupFloatsVec3(std::vector<float> floatVec);
	std::vector<glm::vec4> groupFloatsVec4(std::vector<float> floatVec);
};
#endif