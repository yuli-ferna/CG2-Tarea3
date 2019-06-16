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
	light();
	~light();
	glm::vec3 getLightDir();
	void setLightDir(glm::vec3 lightD);
	glm::vec3 getLightPos();
	void setLightPos(glm::vec3 lightP);
	glm::vec3 getAmbientColor();
	void setAmbientColor(glm::vec3 ambC);
	glm::vec3 getDiffuseColor();
	void setDiffuseColor(glm::vec3 difC); 
	glm::vec3 getSpecularColor();
	void setSpecularColor(glm::vec3 speC);
	bool getONOFF();
	void changeONOFF();
	void leftLightDir();
	void rightLightDir();

	glm::vec3 lightPos;
	glm::vec3 lightDir;
	glm::vec3 ambientColor;
	glm::vec3 diffuseColor;
	glm::vec3 specularColor;
	bool ON;
private:
};


