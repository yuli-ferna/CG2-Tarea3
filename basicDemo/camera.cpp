#include "camera.h"

void camera::updateCamera()
{
	glm::mat4 Rotation = glm::yawPitchRoll(glm::radians(this->yaw), glm::radians(this->pitch), 0.0f);
	this->front = glm::vec3(Rotation * glm::vec4(0, 0, -1, 0));
	this->up = glm::vec3(Rotation * glm::vec4(0, 1, 0, 0));
}

glm::mat4 camera::getRotation()
{
	glm::mat4 Rotation = glm::yawPitchRoll(glm::radians(this->yaw), glm::radians(this->pitch), 0.0f);
	return Rotation;
}

camera::camera()
{
	cameraMode = false;
	speed = 5.5f;
	speedMouse = 0.05f;
	position = glm::vec3(0.0f, 0.5f, 17.0f);
	front = glm::vec3(0.0f, 0.0f, -1.0f);
	up = glm::vec3(0.0f, 1.0f, 0.0f);
	yaw = 0;
	pitch = 0;
}

camera::~camera()
{
}

glm::mat4 camera::getView()
{
	glm::mat4 Rotation = glm::yawPitchRoll(glm::radians(this->yaw), glm::radians(this->pitch), 0.0f);
	this->front = glm::vec3(Rotation * glm::vec4(0, 0, -1, 0));
	this->up = glm::vec3(Rotation * glm::vec4(0, 1, 0, 0));

	this->View = glm::lookAt(this->position, this->position + this->front, this->up);
	
	return this->View;
}

glm::vec3 camera::getPosition()
{
	return this->position;
}

glm::vec3 camera::getFront()
{
	return this->front;
}

void camera::updateInputMouse(GLfloat offsetX, GLfloat offsetY)
{

	yaw += offsetX;
	pitch += offsetY;
	//std::cout << "camara:\n" << yaw << ' ' << pitch << std::endl;
}

void camera::updateInputKeyboard(const char direction)
{
	if (direction == 'w')		
		position += speed * deltaTime * front;
	if (direction == 's')		
		position -= speed * deltaTime * front;
	if (direction == 'a')		
		position -= glm::normalize(glm::cross(front, up)) * speed * deltaTime;
	if (direction == 'd')		
		position += glm::normalize(glm::cross(front, up)) * speed * deltaTime;
}

bool camera::getCameraMode()
{
	return cameraMode;
}

void camera::changeCameraMode()
{
	cameraMode = !cameraMode;
}

void camera::setCameraMode(bool cameraMod)
{
	cameraMode = cameraMod;
}

float camera::getSpeedMouse()
{
	return speedMouse;
}

void camera::setSpeedMouse(float speed)
{
	speedMouse = speed;
}

float camera::getSpeed()
{
	return speed;
}

void camera::setSpeed(float speedK)
{
	speed = speedK;
}

float camera::getDeltatime()
{
	return deltaTime;
}

void camera::setDeltatime(float speed)
{
	deltaTime = speed;
}
