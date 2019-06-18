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
	float shinniness;
	float roughness;
	glm::vec3 ambientColorMtl;
	glm::vec3 diffuseColorMtl;
	glm::vec3 specularColorMtl;
	
	//Light Directional
	glm::vec3 lightDir;
	bool onLightDir;
	glm::vec3 ambientColor;
	glm::vec3 diffuseColor;
	glm::vec3 specularColor;
	
	//Point Light
	int nPointLight; // Numero de luz seleccionada
	bool onLightPoint;
	glm::vec3 lightPointPos;
	glm::vec3 lightAttPoint;
	glm::vec3 ambientColorPoint;
	glm::vec3 diffuseColorPoint;
	glm::vec3 specularColorPoint;

private:
	userInterface();
};