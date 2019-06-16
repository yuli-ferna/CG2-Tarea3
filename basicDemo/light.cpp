#include "light.h"

light::light(glm::vec3 lightP)
{
	lightPos = lightP;
	lightDir = lightP;
	ambientColor = glm::vec3(0.1f);
	diffuseColor = glm::vec3(0.35f);
	specularColor = glm::vec3(0.75f);
	ON = true;

}

light::light()
{
	lightPos = glm::vec3(0.0f, -5.0f, 5.0f);
	lightDir = glm::vec3(0.0f, -5.0f, 5.0f);
	ambientColor = glm::vec3(0.1f);
	diffuseColor = glm::vec3(0.35f);
	specularColor = glm::vec3(0.75f);
	ON = true;
}
light::~light()
{
}

glm::vec3 light::getLightDir()
{
	return lightDir;
}

void light::setLightDir(glm::vec3 lightD)
{
	lightDir = lightD;
}

glm::vec3 light::getLightPos()
{
	return lightPos;
}

void light::setLightPos(glm::vec3 lightP)
{
	lightPos = lightP;
}

glm::vec3 light::getAmbientColor()
{
	return ambientColor;
}

void light::setAmbientColor(glm::vec3 ambC)
{
	ambientColor = ambC;
}

glm::vec3 light::getDiffuseColor()
{
	return diffuseColor;
}

void light::setDiffuseColor(glm::vec3 difC)
{
	diffuseColor = difC;
}

glm::vec3 light::getSpecularColor()
{
	return specularColor;
}

void light::setSpecularColor(glm::vec3 speC)
{
	specularColor = speC;
}

bool light::getONOFF()
{
	return ON;
}

void light::changeONOFF()
{
	ON = !ON;
}

void light::leftLightDir()
{
	lightDir.x -= 0.1f;
	
}

void light::rightLightDir()
{
	lightDir.x += 0.1f;
}
