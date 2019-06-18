#include "userInterface.h"
#include <iostream>
using namespace std;

extern unsigned int windowWidth, windowHeight;

// Global static pointer used to ensure a single instance of the class.
userInterface* userInterface::mInterface = NULL;

/**
* Creates an instance of the class
*
* @return the instance of this class
*/
userInterface* userInterface::Instance()
{
	if (!mInterface)   // Only allow one instance of class to be generated.
		mInterface = new userInterface();

	return mInterface;
}

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

	TwAddVarRW(mUserInterface, "nModel", TW_TYPE_UINT32, &nModel, " group='Modelselect' min=0 label='Select model' step=1 max=2 ");
	TwAddVarRW(mUserInterface, "ambientalColorMtlDir", TW_TYPE_COLOR3F, &ambientColorMtl, "label='Ambiental Mtl' group='Modelselect'");
	TwAddVarRW(mUserInterface, "diffuseColorMtlDir", TW_TYPE_COLOR3F, &diffuseColorMtl, "label='Diffuse Mtl' group='Modelselect'");
	TwAddVarRW(mUserInterface, "specularColorMtlDir", TW_TYPE_COLOR3F, &specularColorMtl, "label='Specular Mtl' group='Modelselect'");
	TwAddVarRW(mUserInterface, "shinniness", TW_TYPE_FLOAT, &shinniness, "label='Shinniness' group='Modelselect' step=0.01 ");
	TwAddVarRW(mUserInterface, "roughness", TW_TYPE_FLOAT, &roughness, "label='Roughness' group='Modelselect' step=0.01 ");
	//TwDefine("Tarea/MaterialColor group='Modelselect' label='Material Color'");

	TwAddSeparator(mUserInterface, "sep1", NULL);
	//TwAddVarRW(mUserInterface, "LightDir", TW_TYPE_DIR3D, &lightDir, "");
	TwAddVarRW(mUserInterface, "ON/OFF 1", TW_TYPE_BOOLCPP, &onLightDir, "label='ON/OFF' group='DirectionalLigth'");
	TwAddVarRW(mUserInterface, "Ligth X", TW_TYPE_FLOAT, &lightDir[0], " group='DirectionLigth' step=0.01 ");
	TwAddVarRW(mUserInterface, "Ligth Y", TW_TYPE_FLOAT, &lightDir[1], " group='DirectionLigth' step=0.01 ");
	TwAddVarRW(mUserInterface, "Ligth Z", TW_TYPE_FLOAT, &lightDir[2], " group='DirectionLigth' step=0.01 ");
	TwDefine("Tarea/DirectionLigth group='DirectionalLigth' label='Direction'");
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
	TwAddVarRW(mUserInterface, "AttenPointX", TW_TYPE_FLOAT, &lightAttPoint[0], " group='AttenuantionLigth' step=0.01 ");
	TwAddVarRW(mUserInterface, "AttenPointY", TW_TYPE_FLOAT, &lightAttPoint[1], " group='AttenuantionLigth' step=0.01 ");
	TwAddVarRW(mUserInterface, "AttenPointZ", TW_TYPE_FLOAT, &lightAttPoint[2], " group='AttenuantionLigth' step=0.01 ");
	TwDefine("Tarea/AttenuantionLigth group='PointLight' label='Attenuation'");

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
