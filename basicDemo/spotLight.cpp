#include "spotLight.h"

spotLight::spotLight()
{
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
