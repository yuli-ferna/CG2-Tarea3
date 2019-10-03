#include "userInterface.h"
#include <iostream>
using namespace std;

extern unsigned int windowWidth, windowHeight;
void funcTransfer();
void TW_CALL RunCB(void*)
{
	funcTransfer();
	
}
userInterface::userInterface()
{
	mUserInterface = TwNewBar("Tarea");
	shinniness = 0;
	color = glm::vec4(1.0f);
	TwDefine("Tarea refresh = '0.0001f'");
	TwDefine("Tarea resizable = true");
	TwDefine("Tarea fontresizable = false");
	TwDefine("Tarea movable = true");
	TwDefine("Tarea position = '10 10'");
	TwDefine("Tarea size = '200 200'");
	TwDefine("Tarea color='143 66 244'");

	TwAddVarRW(mUserInterface, "shinniness", TW_TYPE_FLOAT, &shinniness, "label='Puntos' min=0 max=255 group='Punto funcion' step=1 ");
	TwAddVarRW(mUserInterface, "Ambiental", TW_TYPE_COLOR4F, &color[0], "label = 'Color' group='Punto funcion'");
	TwAddButton(mUserInterface, "Run", RunCB, NULL, " label='Seleccionar' group='Punto funcion'");
}

char userInterface::getShader()
{

	if (shader == BLINN) return 'b';
	if (shader == COOK) return 'c';
	if (shader == OREN) return 'o';
	if (shader == NORM) return 'n';
	if (shader == PARAL) return 'p';
	if (shader == REFR) return 'r';
	if (shader == REFL) return 'l';
	if (shader == TRANS) return 't';
	return NULL;
}

void userInterface::setShader(char type)
{

	if      (type == 'b') shader = BLINN;
	else if (type == 'c') shader = COOK;
	else if (type == 'o') shader = OREN;
	else if (type == 'n') shader = NORM;
	else if (type == 'p') shader = PARAL;
	else if (type == 'r') shader = REFR;
	else if (type == 'l') shader = REFL;
	else if (type == 't') shader = TRANS;
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
