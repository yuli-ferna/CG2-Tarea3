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
	std::vector<glm::vec3> vertex, normal, color;
	std::vector<glm::vec2> uv;
	// Index (GPU) of the geometry buffer
	unsigned int VBO[3];
	// Index (GPU) vertex array object
	unsigned int VAO[1];
	//Propiedades de color del modelo
	glm::vec3 kamb; //Componente ambiental 
	glm::vec3 kdif; //Componente 
	glm::vec3 kspec;//Componente 

	model();
	~model();

	model loadObj(std::string path);
private:

};

