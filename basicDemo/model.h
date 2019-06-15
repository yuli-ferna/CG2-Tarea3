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
	unsigned int VBO[3];
	unsigned int VAO[1];

	model();
	~model();

	model loadObj(std::string path);
private:

};

