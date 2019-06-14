#pragma once
#include <glad/glad.h> // Glad has to be include before glfw
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

class camera
{
	glm::mat4 View;

	bool cameraMode;
	float mouseSpeed = 0.05f;
	float keyboardSpeed = 0.5f;

	//Keyboard
	glm::vec3 worldUp;
	glm::vec3 position;
	glm::vec3 front;
	glm::vec3 right;
	glm::vec3 up;

	//mouse
	float pitch;
	float yaw;
	float roll;

	void updateCamera();
public:
	camera(glm::vec3 position, glm::vec3 direction, glm::vec3 worldUp);
	~camera();
	glm::mat4 getView();
	glm::vec3 getPosition();
	void updateMouseInput(const float& dt, const double& offsetX, const double& offsetY);
	void updateKeyboardInput(const float& dt, const int direction);
	void updateInput(const float& dt,const int direction, const double& offsetX, const double& offsetY);
	bool getCameraMode();
	void setCameraMode(bool cameraMod);
private:

};

