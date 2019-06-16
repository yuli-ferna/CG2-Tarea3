#pragma once
#include "light.h"

class pointLight : public light
{
public:
	pointLight(glm::vec3 lightP);
	~pointLight();
	glm::vec3 k; //Coeficientes de atenuación
	void setKAttenuation(glm::vec3 K);
	glm::vec3 getKAttenuation();
private:

};
