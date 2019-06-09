#pragma once
#include <glad/glad.h> // Glad has to be include before glfw
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>

class model
{

public:
	std::vector<glm::vec4> vertex, uv, vt, color;
	model();
	~model();

private:

};
