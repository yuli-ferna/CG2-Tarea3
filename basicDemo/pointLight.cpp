#include "pointLight.h"

pointLight::pointLight(glm::vec3 lightP)
{
	setLightPos(lightP);
	setKAttenuation(glm::vec3(1.0f, 0.1f, 0.01f));
	ON = true;
}

pointLight::~pointLight()
{
}

void pointLight::setKAttenuation(glm::vec3 K)
{
	k = K;
}

glm::vec3 pointLight::getKAttenuation()
{
	return k;
}
