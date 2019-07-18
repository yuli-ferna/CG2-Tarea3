#pragma once
#include <AntTweakBar.h>
#include <glm/glm.hpp>
#include <iostream>
#include <string>


class userInterface {
public:
	//static userInterface* mInterface; //Holds the instance of the class
	TwBar* mUserInterface;
	~userInterface(); 
	void reshape();
	void show();
	void hide();
	glm::vec3 getLightDir();
	void setLightDir(glm::vec3 lightD);
	glm::vec3 floatpointToVec3(float* arr);
	
	//Shaders
	typedef enum { BLINN, COOK, OREN, NORM, PARAL, REFL, REFR, TRANS } Shader;
	TwType shaderType;
	Shader shader;

	//Models
	int nModel;
	float shinniness;
	float roughness;
	float indexMaterial;
	float indexAmbient;
	float intensityParalax;
	float percentAmbient;
	bool albedo;
	bool lightView;
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

	//Spot Light
	bool onLightSpot;
	glm::vec3 lightAttSpot;
	glm::vec3 ambientColorSpot;
	glm::vec3 diffuseColorSpot;
	glm::vec3 specularColorSpot;
	float cuttof;
	float outerCuttof;

	glm::vec3 direction;
	userInterface();
	char getShader();
	void setShader(char type);

private:
};