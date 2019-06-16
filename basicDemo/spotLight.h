#pragma once
#include "light.h"

class spotLight : public light
{
public:
	spotLight();
	~spotLight();
	glm::vec3 k; //Coeficientes de atenuación
	float cuttof; //Angulo de apertura
	float outerCuttof; //Angulo del segundo cono para la atenuación
	void setKAttenuation(glm::vec3 K);
	glm::vec3 getKAttenuation();
private:

};