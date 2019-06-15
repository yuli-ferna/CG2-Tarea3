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
	speed = 0.05f;
	speedMouse = 0.05f;
	position = glm::vec3(0.0f, 0.5f, 3.0f);
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
	return glm::vec3();
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
		position += speed * front;
	if (direction == 's')		
		position -= speed * front;
	if (direction == 'a')		
		position -= glm::normalize(glm::cross(front, up)) * speed;
	if (direction == 'd')		
		position += glm::normalize(glm::cross(front, up)) * speed;
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
