#pragma once
#include <glad/glad.h> // Glad has to be include before glfw
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>

class model
{

public:
	//string name;
	std::vector<glm::vec3> vertex, normal, color;
	std::vector<glm::vec2> uv;
	// Index (GPU) of the geometry buffer
	unsigned int VBO[3];
	// Index (GPU) vertex array object
	unsigned int VAO[1];
	//Propiedades de color del modelo
	glm::vec3 kamb; //Componente ambiental 
	glm::vec3 kdif; //Componente difusa
	glm::vec3 kspec;//Componente especular
	glm::vec3 position;//position (default=vec3(0.0f))
	
	bool albedo;
	float shinniness;
	float roughness;
	
	//Textures 
	struct Texture
	{
		unsigned int diffuse;
		unsigned int specular;
	};
	Texture texture;
	model();
	~model();

	model* loadObj(std::string path);
	void loadMTL(std::string path);
	Texture getTexture();
	void setTexture(unsigned int diffID);
	void setTexture(unsigned int diffID, unsigned int specID);
	glm::vec3 getPosition();
	void setPosition(glm::vec3 pos);
	glm::vec3 getKAmbient();
	void setKAmbient(glm::vec3 ambC);
	glm::vec3 getKDiffuse();
	void setKDiffuse(glm::vec3 difC);
	glm::vec3 getKSpecular();
	void setKSpecular(glm::vec3 speC);
	float getShinniness();
	void setShinniness(float n);
	float getRoughness();
	void setRoghness(float n);
	bool getAlbedo();
	void setAlbedo(bool n);

private:

};

