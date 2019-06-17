#pragma once
#include <AntTweakBar.h>
#include <glm/glm.hpp>
#include <iostream>
#include <string>


class userInterface {
public:
	static userInterface* mInterface; //Holds the instance of the class
	TwBar* mUserInterface;
	///Method to obtain the only instance of the calls
	static userInterface* Instance();
	~userInterface(); 
	void reshape();
	void show();
	void hide();
	glm::vec3 getLightDir();
	void setLightDir(glm::vec3 lightD);
	glm::vec3 floatpointToVec3(float* arr);
	//Models
	int nModel;
	//Light Directional
	glm::vec3 lightDir;
	bool onLightDir;
	float shinniness;
	float roughness;
	glm::vec3 ambientColor;
	glm::vec3 diffuseColor;
	glm::vec3 specularColor;
	glm::vec3 ambientColorMtl;
	glm::vec3 diffuseColorMtl;
	glm::vec3 specularColorMtl;
private:
	userInterface();
};