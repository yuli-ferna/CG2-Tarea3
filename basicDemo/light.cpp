#include "light.h"

light::light(glm::vec3 lightP)
{
	lightPos = lightP;
	lightDir = lightP;
	ambientColor = glm::vec3(0.1f);
	diffuseColor = glm::vec3(0.75f);
	specularColor = glm::vec3(0.75f);

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

void light::leftLightDir()
{
	lightDir.x -= 0.1f;
}

void light::rightLightDir()
{
	lightDir.x += 0.1f;
}
