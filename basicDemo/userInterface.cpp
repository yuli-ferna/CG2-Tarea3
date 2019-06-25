#include "userInterface.h"
#include <iostream>
using namespace std;

extern unsigned int windowWidth, windowHeight;

userInterface::userInterface()
{
	mUserInterface = TwNewBar("Tarea");

	TwDefine("Tarea refresh = '0.0001f'");
	TwDefine("Tarea resizable = true");
	TwDefine("Tarea fontresizable = false");
	TwDefine("Tarea movable = true");
	TwDefine("Tarea visible = true");
	TwDefine("Tarea position = '10 10'");
	TwDefine("Tarea size = '200 300'");
	TwDefine("Tarea color='143 66 244'");

	direction = glm::vec3(3.0f, -4.0f, -2.0f);

	TwAddVarRW(mUserInterface, "nModel", TW_TYPE_UINT32, &nModel, " group='Modelselect' min=0 label='Select model' step=1 ");
	TwAddVarRW(mUserInterface, "albedo", TW_TYPE_BOOLCPP, &albedo, " group='Modelselect' label='Diffuse map' ");
	TwAddVarRW(mUserInterface, "ambientalColorMtlDir", TW_TYPE_COLOR3F, &ambientColorMtl, "label='Ambiental Mtl' group='Modelselect'");
	TwAddVarRW(mUserInterface, "diffuseColorMtlDir", TW_TYPE_COLOR3F, &diffuseColorMtl, "label='Diffuse Mtl' group='Modelselect'");
	TwAddVarRW(mUserInterface, "specularColorMtlDir", TW_TYPE_COLOR3F, &specularColorMtl, "label='Specular Mtl' group='Modelselect'");
	TwAddVarRW(mUserInterface, "shinniness", TW_TYPE_FLOAT, &shinniness, "label='Shinniness' min=0 group='Modelselect' step=0.01 ");
	TwAddVarRW(mUserInterface, "roughness", TW_TYPE_FLOAT, &roughness, "label='Roughness' min=0 group='Modelselect' step=0.01 ");
	//TwDefine("Tarea/MaterialColor group='Modelselect' label='Material Color'");

	TwAddSeparator(mUserInterface, "sep1", NULL);
	TwAddVarRW(mUserInterface, "ON/OFF 1", TW_TYPE_BOOLCPP, &onLightDir, "label='ON/OFF' group='DirectionalLigth'");
	TwAddVarRW(mUserInterface, "dirLight", TW_TYPE_DIR3F, &direction, "group='DirectionLigth' label='Direction' opened= true");
	//TwDefine("Tarea/DirectionLigth group='DirectionalLigth' label='Direction'");
	TwAddVarRW(mUserInterface, "ambientalColorDir", TW_TYPE_COLOR3F, &ambientColor, "label='Ambiental Color' group='DirectionalLigth'");
	TwAddVarRW(mUserInterface, "diffuseColorDir", TW_TYPE_COLOR3F, &diffuseColor,"label='Diffuse Color' group='DirectionalLigth'");
	TwAddVarRW(mUserInterface, "specularColorDir", TW_TYPE_COLOR3F, &specularColor, "label='Specular Color' group='DirectionalLigth'");
	//TwDefine("Tarea/DirectionalLigth label='Directional Ligth'");
	TwAddSeparator(mUserInterface, "sep2", NULL);
	TwAddVarRW(mUserInterface, "nLightPoint", TW_TYPE_UINT32, &nPointLight, " group='PointLight' min=0 label='Select light' step=1 max=1 ");
	TwAddVarRW(mUserInterface, "ON/OFF 2", TW_TYPE_BOOLCPP, &onLightPoint, "label='ON/OFF' group='PointLight'");
	TwAddVarRW(mUserInterface, "LigthPX", TW_TYPE_FLOAT, &lightPointPos[0], " group='DirectionLigthPoint' step=0.01 ");
	TwAddVarRW(mUserInterface, "LigthPY", TW_TYPE_FLOAT, &lightPointPos[1], " group='DirectionLigthPoint' step=0.01 ");
	TwAddVarRW(mUserInterface, "LigthPZ", TW_TYPE_FLOAT, &lightPointPos[2], " group='DirectionLigthPoint' step=0.01 ");
	TwDefine("Tarea/DirectionLigthPoint group='PointLight' label='Position'");
	TwAddVarRW(mUserInterface, "ambientalColorPoint", TW_TYPE_COLOR3F, &ambientColorPoint, "label='Ambiental Color' group='PointLight'");
	TwAddVarRW(mUserInterface, "diffuseColorPoint", TW_TYPE_COLOR3F, &diffuseColorPoint, "label='Diffuse Color' group='PointLight'");
	TwAddVarRW(mUserInterface, "specularColorPoint", TW_TYPE_COLOR3F, &specularColorPoint, "label='Specular Color' group='PointLight'");
	TwAddVarRW(mUserInterface, "Constant", TW_TYPE_FLOAT, &lightAttPoint[0], " group='AttenuantionLigth' min=0 label = 'Constant' step=0.01 ");
	TwAddVarRW(mUserInterface, "Linear", TW_TYPE_FLOAT, &lightAttPoint[1], " group='AttenuantionLigth' min=0 label = 'Lineal' step=0.01 ");
	TwAddVarRW(mUserInterface, "Quadratic", TW_TYPE_FLOAT, &lightAttPoint[2], " group='AttenuantionLigth' min=0 label = 'Quadratic' step=0.01 ");
	TwDefine("Tarea/AttenuantionLigth group='PointLight' label='Attenuation'");
	//Spot light
	TwAddSeparator(mUserInterface, "sep3", NULL);
	TwAddVarRW(mUserInterface, "ON/OFF 3", TW_TYPE_BOOLCPP, &onLightSpot, "label='ON/OFF' group='SpotLight'");
	TwAddVarRW(mUserInterface, "ambientalColorSpot", TW_TYPE_COLOR3F, &ambientColorSpot, "label='Ambiental Color' group='SpotLight'");
	TwAddVarRW(mUserInterface, "diffuseColorSpot", TW_TYPE_COLOR3F, &diffuseColorSpot, "label='Diffuse Color' group='SpotLight'");
	TwAddVarRW(mUserInterface, "specularColorSpot", TW_TYPE_COLOR3F, &specularColorSpot, "label='Specular Color' group='SpotLight'");
	TwAddVarRW(mUserInterface, "Constant1", TW_TYPE_FLOAT, &lightAttSpot[0], " group='AttenuantionLigthPoint' min=0 label = 'Constant' step=0.01 ");
	TwAddVarRW(mUserInterface, "Linear1", TW_TYPE_FLOAT, &lightAttSpot[1], " group='AttenuantionLigthPoint' min=0 label = 'Lineal' step=0.01 ");
	TwAddVarRW(mUserInterface, "Quadratic1", TW_TYPE_FLOAT, &lightAttSpot[2], " group='AttenuantionLigthPoint' min=0 label = 'Quadratic' step=0.01 ");
	TwDefine("Tarea/AttenuantionLigthPoint group='SpotLight' label='Attenuation'");
	TwAddVarRW(mUserInterface, "cuttof", TW_TYPE_FLOAT, &cuttof, " group='SpotLight' min=0 step=0.01 ");
	TwAddVarRW(mUserInterface, "outerCuttof", TW_TYPE_FLOAT, &outerCuttof, " group='SpotLight' min=0 step=0.01 ");

}

glm::vec3 userInterface::getLightDir()
{
	return lightDir;
}

void userInterface::setLightDir(glm::vec3 lightD)
{
	lightDir = lightD;
}

glm::vec3 userInterface::floatpointToVec3(float* arr)
{
	glm::vec3 aux;
	aux.x = arr[0];
	aux.y = arr[1];
	aux.z = arr[2];
	return glm::vec3();
}

userInterface::~userInterface()
{
}


void userInterface::reshape()
{
	TwWindowSize(windowHeight, windowHeight);
}

void userInterface::show()
{
	TwDefine("Tarea visible = true");
}

void userInterface::hide()
{
	TwDefine("Tarea visible = false");
}
