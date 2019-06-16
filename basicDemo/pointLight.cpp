#include "pointLight.h"

pointLight::pointLight(glm::vec3 lightP)
{
	setLightPos(lightP);
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
