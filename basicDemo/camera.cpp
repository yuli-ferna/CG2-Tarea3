#include "camera.h"

void camera::updateCamera()
{
}

camera::camera(glm::vec3 position, glm::vec3 direction, glm::vec3 worldUp)
{
}

camera::~camera()
{
}

glm::mat4 camera::getView()
{
	this->updateCamera();
	this->View = glm::lookAt(this->position, this->position + this->front, this->up);
	return this->View;
}

glm::vec3 camera::getPosition()
{
	return glm::vec3();
}

void camera::updateMouseInput(const float& dt, const double& offsetX, const double& offsetY)
{
}

void camera::updateKeyboardInput(const float& dt, const int direction)
{
}

void camera::updateInput(const float& dt, const int direction, const double& offsetX, const double& offsetY)
{
}
