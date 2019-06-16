#pragma once
#include <glad/glad.h> // Glad has to be include before glfw
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

class light
{
public:
	light(glm::vec3 lightP);
	~light();
	glm::vec3 getLightDir();
	void setLightDir(glm::vec3 lightD);
	void leftLightDir();
	void rightLightDir();

private:
	glm::vec3 lightPos;
	glm::vec3 lightDir;
	glm::vec3 ambientColor;
	glm::vec3 diffuseColor;
	glm::vec3 specularColor;
};


