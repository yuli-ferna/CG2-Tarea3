#include "spotLight.h"

spotLight::spotLight(glm::vec3 lightP)
{
	lightPos = lightP;
	setKAttenuation(glm::vec3(1.0f, 0.1f, 0.01f));
	setDiffuseColor(glm::vec3(0.0f, 0.0f, 0.75f));
	ON = true;
	cuttof = 0.8f;
	outerCuttof = 0.1f;
}

spotLight::~spotLight()
{
}

void spotLight::setKAttenuation(glm::vec3 K)
{
	k = K;
}

glm::vec3 spotLight::getKAttenuation()
{
	return k;
}

void spotLight::setCuttof(float K)
{
	cuttof = K;
}

float spotLight::getCuttof()
{
	return cuttof;
}

void spotLight::setOuterCuttof(float K)
{
	outerCuttof = K;
}

float spotLight::getOuterCuttof()
{
	return outerCuttof;
}
