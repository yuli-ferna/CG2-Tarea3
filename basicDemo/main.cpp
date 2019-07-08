#define N_POINTLIGHTS 2
#define NVBOS 5
#include <glad/glad.h> // Glad has to be include before glfw
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm\gtx\euler_angles.hpp> // glm::yawPitchRoll

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>

#include <stb_image.h>
#include "userInterface.h"
#include "model.h"
#include "camera.h"
#include "light.h"
#include "pointLight.h"
#include "spotLight.h"

#include "Shader.h"
#include <vector>

#define px(x) x  
// Window current width
unsigned int windowWidth = 800;
// Window current height
unsigned int windowHeight = 600;
// Window title
const char *windowTitle = "Yuliana Fernandez";
// Window pointer
GLFWwindow *window;
// Shader object
Shader *shader, *shaderDirLight, *shaderPointLight, 
*shaderAllLight, *shaderAllLightOrenayer, *shaderEnviroment, *shaderAllLightCookTorrence,
*shaderSpecMap, *shaderSkybox, *shaderRefraction, *shadernormalMapping;

//Textures
unsigned int textureID;
unsigned int textureID1, specularMap, normalMap, dispMap;
unsigned int cubemapTexture;
std::vector<unsigned int> textures;

//skybox
unsigned int skyboxVAO, skyboxVBO;


//MVP Matrix
glm::mat4 Model;
glm::mat4 View;
glm::mat4 Proj;

//Camera
camera *Camara = new camera();
float speedMouse = Camara->getSpeedMouse();

//Lights
enum modeLight {dir, point};
modeLight mode = dir;
light* directionalLight = new light(glm::vec3(0.0f, 5.0f, 5.0f));
std::vector < pointLight* > PointLight;
spotLight* SpotLight = new spotLight(Camara->getPosition());

//tweakBar
userInterface *Interface;

//Models
model *object;
std::vector< model* > modelsObj, lightsObj;
int lightAnt, modelAnt;
/* *
 * Handles the window resize
 * @param{GLFWwindow} window pointer
 * @param{int} new width of the window
 * @param{int} new height of the window
 * */
void resize(GLFWwindow *window, int width, int height)
{
    windowWidth = width;
    windowHeight = height;
    // Sets the OpenGL viewport size and position
    glViewport(0, 0, windowWidth, windowHeight);
	Interface->reshape();

}

/**
 * Calback the window resize
 * @param{GLFWwindow} window pointer
 * @param{int} new width of the window
 * @param{int} new height of the window
 * */
void onResizeWindow(GLFWwindow* window, int width, int height) {
	TwWindowSize(width, height);
}

/**
 * Calback key press
 * */
void onKeyPress(GLFWwindow* window, int key, int scancode, int action, int mods) {
	/*if (action == GLFW_PRESS)
	{
		TwKeyPressed(key, TW_KMOD_NONE);
		return;
	}*/
	//Important!
	if (TwEventKeyGLFW(key, action))
		return;

	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GL_TRUE);
			break;
		case GLFW_KEY_C:
			//Changes camera mode
			if (!Camara->getCameraMode())
			{
				//cameraMode = true;
				Interface->hide();
			}
			else {
				//cameraMode = false;
				Interface->show();

			}
			Camara->changeCameraMode();
			//cameraMode = !cameraMode;
			break;

		}
	}

}

void onMouseButton(GLFWwindow* window, int button, int action, int mods)
{
	auto a = action == GLFW_PRESS ? TW_MOUSE_PRESSED : TW_MOUSE_RELEASED;
	auto b = TW_MOUSE_LEFT;

	TwMouseButton(a, b);
}

void onMouseMotion(GLFWwindow* window, double xpos, double ypos) 
{
	TwMouseMotion(px(static_cast<int>(xpos)), px(static_cast<int>(ypos)));
	if (Camara->getCameraMode()) {

		glfwSetCursorPos(window, windowWidth / 2.0, windowHeight / 2.0);
		GLfloat xoffset = ((windowWidth / 2.0) - xpos) * speedMouse;
		GLfloat yoffset = ((windowHeight / 2.0) - ypos) * speedMouse;
		Camara->updateInputMouse(xoffset, yoffset);

		/*yaw += xoffset;
		pitch += yoffset;
		std::cout << "main:\n" << yaw << ' ' << pitch << std::endl;*/
	}
}
void onCharacter(GLFWwindow* window, unsigned int codepoint) {
	TwKeyPressed(codepoint, TW_KMOD_NONE);

}

void initUserInterfaceValues()
{
	//Model
	Interface->nModel = 0;
	modelAnt = 0;
	Interface->shinniness = modelsObj[0]->getShinniness();
	Interface->indexMaterial = modelsObj[0]->getindexMaterial();
	Interface->intensityParalax = modelsObj[0]->getintensityParalax();
	Interface->indexAmbient = modelsObj[0]->getindexAmbient();
	Interface->percentAmbient = modelsObj[0]->getpercentAmbient();
	Interface->roughness = modelsObj[0]->getRoughness();
	Interface->albedo = modelsObj[0]->getAlbedo();
	Interface->ambientColorMtl = modelsObj[0]->getKAmbient();
	Interface->diffuseColorMtl = modelsObj[0]->getKDiffuse();
	Interface->specularColorMtl = modelsObj[0]->getKSpecular();
	//Light direction
	Interface->lightDir = (directionalLight->getLightDir());
	Interface->ambientColor = directionalLight->getAmbientColor();
	Interface->diffuseColor = directionalLight->getDiffuseColor();
	Interface->specularColor = directionalLight->getSpecularColor();
	Interface->onLightDir = directionalLight->getONOFF();
	// Point Light
	Interface->nPointLight = 0;
	lightAnt = 0;
	Interface->onLightPoint = PointLight[0]->getONOFF();
	Interface->ambientColorPoint = PointLight[0]->getAmbientColor();
	Interface->diffuseColorPoint = PointLight[0]->getDiffuseColor();
	Interface->specularColorPoint = PointLight[0]->getSpecularColor();
	Interface->lightPointPos = PointLight[0]->getLightPos();
	Interface->lightAttPoint = PointLight[0]->getKAttenuation();

	//Spot Light
	Interface->onLightSpot = SpotLight->getONOFF();
	Interface->ambientColorSpot = SpotLight->getAmbientColor();
	Interface->diffuseColorSpot = SpotLight->getDiffuseColor();
	Interface->specularColorSpot = SpotLight->getSpecularColor();
	Interface->lightAttSpot = SpotLight->getKAttenuation();
	Interface->cuttof = SpotLight->getCuttof();
	Interface->outerCuttof = SpotLight->getOuterCuttof();
	
}
/**
 * initialize the user interface
 * @returns{bool} true if everything goes ok
* */
bool initUserInterface()
{
	if (!TwInit(TW_OPENGL_CORE, NULL))
		return false;

	Interface = new userInterface();
	TwWindowSize(windowHeight, windowHeight);
	//initUserInterfaceValues();
	return true;
}
/**
 * Initialize the glfw library
 * @returns{bool} true if everything goes ok
 * */
bool initWindow()
{
    // Initialize glfw
    glfwInit();
    // Sets the Opegl context to Opengl 3.0
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    // Creates the window
    window = glfwCreateWindow(windowWidth, windowHeight, windowTitle, NULL, NULL);
	
    // The window couldn't be created
    if (!window)
    {
        std::cout << "Failed to create the glfw window" << std::endl;
        glfwTerminate(); // Stops the glfw program
        return false;
    }

    // Creates the glfwContext, this has to be made before calling initGlad()
    glfwMakeContextCurrent(window);
	
    // Window resize callback
	glfwSetFramebufferSizeCallback(window, resize);

	glfwSetCursorPosCallback(window, onMouseMotion);
	glfwSetMouseButtonCallback(window, onMouseButton);
	glfwSetKeyCallback(window, onKeyPress);
	glfwSetCharCallback(window, onCharacter);
	glfwSetWindowSizeCallback(window, onResizeWindow);
	
	return true;
}

/**
 * Initialize the glad library
 * @returns{bool} true if everything goes ok
 * */
bool initGlad()
{
    // Initialize glad
    int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    // If something went wrong during the glad initialization
    if (!status)
    {
        std::cout << status << std::endl;
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }
    return true;
}
/**
 * Initialize the opengl context
 * */
void initGL()
{
    // Enables the z-buffer test
    glEnable(GL_DEPTH_TEST);
	glEnable(GL_FRAMEBUFFER_SRGB);
    // Sets the ViewPort
    glViewport(0, 0, windowWidth, windowHeight);
    // Sets the clear color
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
}

/*
** Init MVP Matrix 
*/

void initMVP() 
{
	Model = glm::mat4(1.0f);
	View = Camara->getView();
	Proj = glm::perspective(45.0f, 800.0f / 600.0f, 0.1f, 190.0f);

}

void buildGeom(std::string path, std::vector< glm::vec3 > position)
{
	std::vector< glm::vec3 > Vert;
	std::vector< glm::vec3 > Normal;
	std::vector< glm::vec3 > Tangent;
	std::vector< glm::vec3 > Bitangent;
	std::vector< glm::vec2 > UV;
	//Positions

	model *obj = new model(position[0]);
	obj->loadObj(path, Vert, Normal, UV);
	obj->getTangentBitanget(Vert, UV, Normal, Tangent, Bitangent);
	
	//int nMod = 3;
	//Load 3 models
	unsigned int VAOForm[1],  VBOForm[NVBOS], numVertex = Vert.size();
	// Creates on GPU the vertex array
	glGenVertexArrays(1, &VAOForm[0]);
	// Creates on GPU the vertex buffer obj
	glGenBuffers(NVBOS, VBOForm);
	// Binds the vertex array to set all the its properties
	glBindVertexArray(VAOForm[0]);

	//vexter position VBOForm
	// Sets the buffer geometry data
	glBindBuffer(GL_ARRAY_BUFFER, VBOForm[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * Vert.size(), &Vert[0], GL_STATIC_DRAW);
	//vertex position position VAO
	// Sets the vertex attributes
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	//uv VBOForm
	// Sets the buffer geometry data
	glBindBuffer(GL_ARRAY_BUFFER, VBOForm[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * UV.size(), &UV[0], GL_STATIC_DRAW);

	//uv VAO
	// Sets the vertex attributes
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	
	//normal VBOForm
	// Sets the buffer geometry data
	glBindBuffer(GL_ARRAY_BUFFER, VBOForm[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * Normal.size(), &Normal[0], GL_STATIC_DRAW);

	//normal object.VAO
	// Sets the vertex attributes
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	//Tangent VBOForm
	// Sets the buffer geometry data
	glBindBuffer(GL_ARRAY_BUFFER, VBOForm[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * Tangent.size(), &Tangent[0], GL_STATIC_DRAW);

	//Tangent object.VAO
	// Sets the vertex attributes
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	//Bitangent VBOForm
	// Sets the buffer geometry data
	glBindBuffer(GL_ARRAY_BUFFER, VBOForm[4]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * Bitangent.size(), &Bitangent[0], GL_STATIC_DRAW);

	//Bitangent object.VAO
	// Sets the vertex attributes
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glBindVertexArray(0);
	
	
	//Se encolan los modelos que tienen el mismo obj
	model* obj1;
	for (size_t i = 0; i < position.size(); i++)
	{
		model* obj1 = new model(position[i]);
		//obj1->setPosition(pos[i]);
		obj1->VBO[0] = VBOForm[0];
		obj1->VBO[1] = VBOForm[1];
		obj1->VBO[2] = VBOForm[2];
		obj1->VBO[3] = VBOForm[3];
		obj1->VBO[4] = VBOForm[4];
		obj1->VAO[0] = VAOForm[0];
		obj1->numVertex = numVertex;
		obj1->setPosition(position[i]);
		modelsObj.push_back(obj1);
	}
	
	position.clear();
}

void buildSkyBox() 
{
	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	// skybox VAO
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

}
/**
 * Builds all the geometry buffers and
 * loads them up into the GPU
 * (Builds a simple triangle)
 * */
void buildGeometry()
{
	std::vector< std::string > paths;
	std::vector < glm::vec3 > pos;
	
	
	
	//Positions
	pos.push_back(glm::vec3(0.0f));
	pos.push_back(glm::vec3(4.0f, 0.0f, 0.0f));
	pos.push_back(glm::vec3(-4.0f, 0.0f, 0.0f));
	//Carga el mismo modelo en las distintas posiciones que tiene el arreglo
	buildGeom(".\\assets\\models\\cube1.obj", pos);

	//Paths
	paths.push_back(".\\assets\\models\\planeS.obj");
	paths.push_back(".\\assets\\models\\cube2.obj");

	pos.clear();
	pos.push_back(glm::vec3(0.0f));
	pos.push_back(glm::vec3(4.0f));

	//Load models
	for (size_t i = 0; i < paths.size(); i++)
	{
		object = object->loadObj(paths[i], pos[i]);

		// Creates on GPU the vertex array
		glGenVertexArrays(1, &object->VAO[0]);
		// Creates on GPU the vertex buffer object
		glGenBuffers(NVBOS, object->VBO);
		// Binds the vertex array to set all the its properties
		glBindVertexArray(object->VAO[0]);

		//vexter position object->VBO
		// Sets the buffer geometry data
		glBindBuffer(GL_ARRAY_BUFFER, object->VBO[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * object->vertex.size(), &object->vertex[0], GL_STATIC_DRAW);
		//vertex position position object->VAO
		// Sets the vertex attributes
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	
		//uv object->VBO
		// Sets the buffer geometry data
		glBindBuffer(GL_ARRAY_BUFFER, object->VBO[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * object->uv.size(), &object->uv[0], GL_STATIC_DRAW);

		//uv object->VAO
		// Sets the vertex attributes
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
		//glBindVertexArray(0);
		//color object->VBO
		// Sets the buffer geometry data
		glBindBuffer(GL_ARRAY_BUFFER, object->VBO[2]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * object->normal.size(), &object->normal[0], GL_STATIC_DRAW);

		//color object->VAO
		// Sets the vertex attributes
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		//Tangent object->VBO
		// Sets the buffer geometry data
		glBindBuffer(GL_ARRAY_BUFFER, object->VBO[3]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * object->tangent.size(), &object->tangent[0], GL_STATIC_DRAW);

		//Tangent object.VAO
		// Sets the vertex attributes
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		//Bitangent object->VBO
		// Sets the buffer geometry data
		glBindBuffer(GL_ARRAY_BUFFER, object->VBO[4]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * object->bitangent.size(), &object->bitangent[0], GL_STATIC_DRAW);

		//Bitangent object.VAO
		// Sets the vertex attributes
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		glBindVertexArray(0);
	
		//object->setPosition(pos[i]);
		modelsObj.push_back(object);
	}
	
	object = object->loadObj(".\\assets\\models\\pointlight.obj", glm::vec3(0.0f));
	//Load Lights
	for (size_t i = 0; i < N_POINTLIGHTS; i++)
	{

		// Creates on GPU the vertex array
		glGenVertexArrays(1, &object->VAO[0]);
		// Creates on GPU the vertex buffer object
		glGenBuffers(3, object->VBO);
		// Binds the vertex array to set all the its properties
		glBindVertexArray(object->VAO[0]);

		//vexter position object->VBO
		// Sets the buffer geometry data
		glBindBuffer(GL_ARRAY_BUFFER, object->VBO[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * object->vertex.size(), &object->vertex[0], GL_STATIC_DRAW);
		//vertex position position object->VAO
		// Sets the vertex attributes
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		//uv object->VBO
		// Sets the buffer geometry data
		glBindBuffer(GL_ARRAY_BUFFER, object->VBO[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * object->uv.size(), &object->uv[0], GL_STATIC_DRAW);

		//uv object->VAO
		// Sets the vertex attributes
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
		//glBindVertexArray(0);
		//color object->VBO
		// Sets the buffer geometry data
		glBindBuffer(GL_ARRAY_BUFFER, object->VBO[2]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * object->normal.size(), &object->normal[0], GL_STATIC_DRAW);

		//color object.VAO
		// Sets the vertex attributes
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glBindVertexArray(0);
		object->setPosition(pos[i]);
		lightsObj.push_back(object);
	}

	//Load skybox
	buildSkyBox();
}
/**
 * Loads a texture into the GPU
 * @param{const char} path of the texture file
 * @returns{unsigned int} GPU texture index
 * */
unsigned int loadTexture(const char *path)
{
    unsigned int id;
    // Creates the texture on GPU
    glGenTextures(1, &id);
    // Loads the texture
    int textureWidth, textureHeight, numberOfChannels;
    // Flips the texture when loads it because in opengl the texture coordinates are flipped
    stbi_set_flip_vertically_on_load(true);
    // Loads the texture file data
    unsigned char *data = stbi_load(path, &textureWidth, &textureHeight, &numberOfChannels, 0);
    if (data)
    {
        // Gets the texture channel format
        GLenum format;
        switch (numberOfChannels)
        {
        case 1:
            format = GL_RED;
            break;
        case 3:
            format = GL_RGB;
            break;
        case 4:
            format = GL_RGBA;
            break;
        }

        // Binds the texture
        glBindTexture(GL_TEXTURE_2D, id);
        // Creates the texture
        glTexImage2D(GL_TEXTURE_2D, 0, format, textureWidth, textureHeight, 0, format, GL_UNSIGNED_BYTE, data);
        // Creates the texture mipmaps
        glGenerateMipmap(GL_TEXTURE_2D);

        // Set the filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
        std::cout << "ERROR:: Unable to load texture " << path << std::endl;
        glDeleteTextures(1, &id);
    }
    // We dont need the data texture anymore because is loaded on the GPU
    stbi_image_free(data);

    return id;
}

void initLights() 
{
	pointLight *light1 = new pointLight(glm::vec3(0.0f, 0.0f, 0.0f));
	light1->setDiffuseColor(glm::vec3(0.2f));
	pointLight *light2 = new pointLight(glm::vec3(2.0f, 0.0f, -2.0f));
	light2->setDiffuseColor(glm::vec3(0.2f));

	PointLight.push_back(light1);
	PointLight.push_back(light2);
}

unsigned int loadCubemap(std::vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	stbi_set_flip_vertically_on_load(false);
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

void initTexture() {

	textureID1 = loadTexture("assets/textures/bricks2.jpg");
	textures.push_back(textureID1);
	textureID = loadTexture("assets/textures/grass.png");
	textures.push_back(textureID);
	textureID1 = loadTexture("assets/textures/container2.png");
	normalMap = loadTexture("assets/textures/bricks2_normal.jpg");
	dispMap = loadTexture("assets/textures/bricks2_disp.jpg");

	textures.push_back(textureID);
	textures.push_back(textureID1);
	textures.push_back(textureID1);
	specularMap = loadTexture("assets/textures/container2_specular.png");
	//Load skybox
	std::vector<std::string> faces
	{
		"assets/textures/right.jpg",
		"assets/textures/left.jpg",
		"assets/textures/top.jpg",
		"assets/textures/bottom.jpg",
		"assets/textures/front.jpg",
		"assets/textures/back.jpg"
	};
	cubemapTexture = loadCubemap(faces);
	//textures.push_back(cubemapTexture);

}

/**
 * Initialize everything
 * @returns{bool} true if everything goes ok
 * */
bool init()
{
    // Initialize the window, the interface and the glad components
    if (!initWindow() || !initGlad() || !initUserInterface())
        return false;

    // Initialize the opengl context
    initGL();

    // Loads the shader
	shader = new Shader("assets/shaders/basic.vert", "assets/shaders/basic.frag");
	/*shaderDirLight = new Shader("assets/shaders/lightDirectional.vert", "assets/shaders/lightDirectional.frag");
	shaderPointLight = new Shader("assets/shaders/lightPoint.vert", "assets/shaders/lightPoint.frag");*/
	shaderEnviroment = new Shader("assets/shaders/enviromentMapp.vert", "assets/shaders/enviromentMapp.frag");
	shaderAllLight = new Shader("assets/shaders/allLight.vert", "assets/shaders/allLight.frag");
	shaderAllLightOrenayer = new Shader("assets/shaders/allLightOrenayer.vert", "assets/shaders/allLightOrenayer.frag");
	shaderRefraction = new Shader("assets/shaders/refraction.vert", "assets/shaders/refraction.frag");
	shaderAllLightCookTorrence = new Shader("assets/shaders/allLightCookTorrence.vert", "assets/shaders/allLightCookTorrence.frag");
	shadernormalMapping = new Shader("assets/shaders/normalMapping.vert", "assets/shaders/normalMapping.frag");
	shaderSpecMap = new Shader("assets/shaders/specMap.vert", "assets/shaders/specMap.frag");
	shaderSkybox = new Shader("assets/shaders/skybox.vert", "assets/shaders/skybox.frag");
	//shaderAllLight = new Shader("assets/shaders/lightDirectional.vert", "assets/shaders/lightDirectional.frag");

    // Loads all the geometry into the GPU
    buildGeometry();
    
	// Loads the texture into the GPU
	initTexture();

	//Initializate MVP values
	initMVP();
	
	//Initializate lights
	initLights();

	//Init values of tweakbar
	initUserInterfaceValues();

    return true;
}
/**
 * Process the keyboard input
 * There are ways of implementing this function through callbacks provide by
 * the GLFW API, check the GLFW documentation to find more
 * @param{GLFWwindow} window pointer
 * */
void processKeyboardInput(GLFWwindow *window)
{
    // Checks if the escape key is pressed
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        // Tells glfw to close the window as soon as possible
        glfwSetWindowShouldClose(window, true);

    // Checks if the r key is pressed
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        // Reloads the shader
		delete shader;
	/*	delete shaderDirLight;
		delete shaderPointLight;*/
		delete shaderEnviroment;
		delete shaderAllLight;
		delete shaderAllLightOrenayer;
		delete shaderRefraction;
		delete shaderAllLightCookTorrence;
		delete shaderSpecMap;
		delete shaderSkybox;
		delete shadernormalMapping;

		shader = new Shader("assets/shaders/basic.vert", "assets/shaders/basic.frag");
		/*shaderDirLight = new Shader("assets/shaders/lightDirectional.vert", "assets/shaders/lightDirectional.frag");*/
		shaderEnviroment = new Shader("assets/shaders/enviromentMapp.vert", "assets/shaders/enviromentMapp.frag");
		/*shaderPointLight = new Shader("assets/shaders/lightPoint.vert", "assets/shaders/lightPoint.frag");*/
		shaderAllLight = new Shader("assets/shaders/allLight.vert", "assets/shaders/allLight.frag");
		shaderRefraction = new Shader("assets/shaders/refraction.vert", "assets/shaders/refraction.frag");
		shaderAllLightOrenayer = new Shader("assets/shaders/allLightOrenayer.vert", "assets/shaders/allLightOrenayer.frag");
		shaderSkybox = new Shader("assets/shaders/skybox.vert", "assets/shaders/skybox.frag");
		shaderSpecMap = new Shader("assets/shaders/specMap.vert", "assets/shaders/specMap.frag");
		shaderAllLightCookTorrence = new Shader("assets/shaders/allLightCookTorrence.vert", "assets/shaders/allLightCookTorrence.frag");
		shadernormalMapping = new Shader("assets/shaders/normalMapping.vert", "assets/shaders/normalMapping.frag");
		//shaderAllLight = new Shader("assets/shaders/lightDirectional.vert", "assets/shaders/lightDirectional.frag");

    }
	
	//Move camera
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		Camara->updateInputKeyboard('w');
		
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		Camara->updateInputKeyboard('s');
		
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		Camara->updateInputKeyboard('a');
		
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		Camara->updateInputKeyboard('d');
	
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
		directionalLight->changeONOFF();

}

/*
* Update MVP
*/
void updateMVP(int i)
{
	Model = glm::mat4(1.0f);

	Model = glm::translate(glm::mat4(1.0f), modelsObj[i]->getPosition());
	
	View = Camara->getView();
	//Proj = glm::perspective(45.0f, (float)windowHeight / (float)windowWidth, 0.1f, 100.0f);
}

void drawLights()
{

	//Light models
	// Use the shader
	shader->use();

	//MVP trnasformations

	//Draw models of the scene
	for (size_t i = 0; i < lightsObj.size(); i++)
	{
		updateMVP(i);
		Model = glm::translate(glm::mat4(1.0f), PointLight[i]->getLightPos());
		glm::mat4 MVP = (Proj * View * Model);
		shader->setMat4("MVP", MVP);
		//Material
		shader->setVec3("ka", lightsObj[i]->getKAmbient());
		shader->setVec3("kd", lightsObj[i]->getKDiffuse());
		shader->setVec3("ks", lightsObj[i]->getKSpecular());
		shader->setFloat("n", lightsObj[i]->getShinniness());
		shader->setVec3("lColor", PointLight[i]->getDiffuseColor());
		//Texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);
		shader->setInt("text", 0);

		// Binds the vertex array to be drawn
		glBindVertexArray(lightsObj[i]->VAO[0]);

		// Renders the triangle gemotry
		glDrawArrays(GL_TRIANGLES, 0, lightsObj[i]->vertex.size());

	}


	glBindVertexArray(0);
}

void renderObj(Shader* shaderActual, int i)
{
	glm::mat4 MVP;
	// Use the shader
	shaderActual->use();

	//Light directional
	shaderActual->setVec3("viewPos", Camara->getPosition());
	//glm::vec3 lightD = glm::vec3(/*Model * View * */glm::vec4(directionalLight->getLightDir(), 0));
	shaderActual->setVec3("lightDir", directionalLight->getLightDir());
	shaderActual->setVec3("lightPos", directionalLight->getLightPos());
	shaderActual->setVec3("ambientColor", directionalLight->getAmbientColor());
	shaderActual->setVec3("diffuseColor", directionalLight->getDiffuseColor());
	shaderActual->setVec3("specularColor", directionalLight->getSpecularColor());
	shaderActual->setBool("on", directionalLight->getONOFF());

	//Point Light

	for (int i = 0; i < N_POINTLIGHTS; i++)
	{
		std::string it = std::to_string(i);
		//shaderActual->setVec3("pointLights[" + it + "].position", glm::vec3(2.0f, 0.0f, -2.0f));
		shaderActual->setVec3("pointLights[" + it + "].position", PointLight[i]->getLightPos());
		shaderActual->setVec3("pointLights[" + it + "].ambientColor", PointLight[i]->getAmbientColor());
		shaderActual->setVec3("pointLights[" + it + "].diffuseColor", PointLight[i]->getDiffuseColor());
		shaderActual->setVec3("pointLights[" + it + "].specularColor", PointLight[i]->getSpecularColor());
		shaderActual->setVec3("pointLights[" + it + "].attenuationK", PointLight[i]->getKAttenuation());
		shaderActual->setBool("pointLights[" + it + "].on", PointLight[i]->getONOFF());
	}
	//////std::cout << PointLight[0]->getLightPos().x << ' ' << PointLight[0]->getLightPos().y << ' ' << PointLight[0]->getLightPos().z << std::endl;

	//Spot Light
	shaderActual->setVec3("SpotLight.position", Camara->getPosition());
	shaderActual->setVec3("SpotLight.direction", Camara->getFront());
	shaderActual->setVec3("SpotLight.ambientColor", SpotLight->getAmbientColor());
	shaderActual->setVec3("SpotLight.diffuseColor", SpotLight->getDiffuseColor());
	shaderActual->setVec3("SpotLight.specularColor", SpotLight->getSpecularColor());
	shaderActual->setVec3("SpotLight.attenuationK", SpotLight->getKAttenuation());
	shaderActual->setFloat("SpotLight.cuttof", SpotLight->getCuttof());
	shaderActual->setFloat("SpotLight.outerCuttof", SpotLight->getOuterCuttof());
	shaderActual->setBool("SpotLight.on", SpotLight->getONOFF());

	//Draw models of the scene
	/*for (size_t i = 0; i < modelsObj.size(); i++)
	{
	*/	//MVP trnasformations
		updateMVP(i);
		shaderActual->setMat4("Model", Model);
		shaderActual->setMat4("View", View);
		shaderActual->setMat4("Proj", Proj);
		
		//Material
		shaderActual->setVec3("ka", modelsObj[i]->getKAmbient());
		shaderActual->setVec3("kd", modelsObj[i]->getKDiffuse());
		shaderActual->setVec3("ks", modelsObj[i]->getKSpecular());
		shaderActual->setFloat("n", modelsObj[i]->getShinniness());
		shaderActual->setFloat("indexMaterial", modelsObj[i]->getindexMaterial());
		shaderActual->setFloat("indexMaterial", modelsObj[i]->getindexMaterial());
		shaderActual->setFloat("indexMaterial", modelsObj[i]->getindexMaterial());
		shaderActual->setFloat("roughness", modelsObj[i]->getRoughness());
		shaderActual->setBool("albedo", modelsObj[i]->getAlbedo());

		//Texture
		
		glActiveTexture(GL_TEXTURE0 + textures[i] - 1);
		glBindTexture(GL_TEXTURE_2D, textures[i]);
		shaderActual->setInt("text", textures[i] - 1);
		
		glActiveTexture(GL_TEXTURE0 + specularMap - 1);
		glBindTexture(GL_TEXTURE_2D, specularMap);
		shaderActual->setInt("specMap", specularMap - 1);
		
		glActiveTexture(GL_TEXTURE0 + normalMap - 1);
		glBindTexture(GL_TEXTURE_2D, normalMap);
		shaderActual->setInt("normalMap", normalMap - 1);
		
		glActiveTexture(GL_TEXTURE0 + dispMap - 1);
		glBindTexture(GL_TEXTURE_2D, dispMap);
		shaderActual->setInt("dispMap", dispMap - 1);

		// Binds the vertex array to be drawn
		glBindVertexArray(modelsObj[i]->VAO[0]);
    
		// Renders the triangle gemotry
		glDrawArrays(GL_TRIANGLES, 0, modelsObj[i]->numVertex);
		
	/*}
*/
	glBindVertexArray(0);


}

void updateUserInterface() 
{
	//Model
	int nMod = Interface->nModel;
	if (nMod >= modelsObj.size()) nMod = modelsObj.size() - 1;
	if (modelAnt != nMod)
	{
		//Nueva selección 
		//(actualizamos tweakbar con los atributos del modelo seleccionado)
		Interface->ambientColorMtl = modelsObj[nMod]->getKAmbient();
		Interface->diffuseColorMtl = modelsObj[nMod]->getKDiffuse();
		Interface->specularColorMtl = modelsObj[nMod]->getKSpecular();
		Interface->shinniness = modelsObj[nMod]->getShinniness();
		Interface->roughness = modelsObj[nMod]->getRoughness();
		Interface->albedo = modelsObj[nMod]->getAlbedo();
		Interface->setShader(modelsObj[nMod]->getShader());
		Interface->indexAmbient = modelsObj[nMod]->getindexAmbient();
		Interface->indexMaterial = modelsObj[nMod]->getindexMaterial();
		Interface->percentAmbient = modelsObj[nMod]->getpercentAmbient();
		Interface->intensityParalax = modelsObj[nMod]->getintensityParalax();

		modelAnt = nMod;
	}
	modelsObj[nMod]->setKAmbient(Interface->ambientColorMtl);
	modelsObj[nMod]->setKDiffuse(Interface->diffuseColorMtl);
	modelsObj[nMod]->setKSpecular(Interface->specularColorMtl);
	modelsObj[nMod]->setShinniness(Interface->shinniness);
	modelsObj[nMod]->setRoghness(Interface->roughness);
	modelsObj[nMod]->setindexMaterial(Interface->indexMaterial);
	modelsObj[nMod]->setindexAmbient(Interface->indexAmbient);
	modelsObj[nMod]->setpercentAmbient(Interface->percentAmbient);
	modelsObj[nMod]->setintensityParalax(Interface->intensityParalax);

	modelsObj[nMod]->setAlbedo(Interface->albedo);
	modelsObj[nMod]->setShader(Interface->getShader());
	
	//Light dir
	directionalLight->setLightDir(Interface->direction);
	directionalLight->setAmbientColor(Interface->ambientColor);
	directionalLight->setDiffuseColor(Interface->diffuseColor);
	directionalLight->setSpecularColor(Interface->specularColor);
	directionalLight->setONOFF(Interface->onLightDir);

	//Point lights
	int nLight = Interface->nPointLight;
	
	if (lightAnt != nLight)
	{
		Interface->onLightPoint = PointLight[nLight]->getONOFF();
		Interface->ambientColorPoint = PointLight[nLight]->getAmbientColor();
		Interface->diffuseColorPoint = PointLight[nLight]->getDiffuseColor();
		Interface->specularColorPoint = PointLight[nLight]->getSpecularColor();
		Interface->lightPointPos = PointLight[nLight]->getLightPos();
		Interface->lightAttPoint = PointLight[nLight]->k;
		lightAnt = nLight;
	}

	PointLight[nLight]->setLightPos(Interface->lightPointPos);
	PointLight[nLight]->setONOFF(Interface->onLightPoint);
	PointLight[nLight]->setAmbientColor(Interface->ambientColorPoint);
	PointLight[nLight]->setDiffuseColor(Interface->diffuseColorPoint);
	PointLight[nLight]->setSpecularColor(Interface->specularColorPoint);
	PointLight[nLight]->setKAttenuation(Interface->lightAttPoint);

	//Spot Light;
	SpotLight->setAmbientColor(Interface->ambientColorSpot);
	SpotLight->setDiffuseColor(Interface->diffuseColorSpot);
	SpotLight->setSpecularColor(Interface->specularColorSpot);
	SpotLight->setCuttof(Interface->cuttof);
	SpotLight->setOuterCuttof(Interface->outerCuttof);
	SpotLight->setONOFF(Interface->onLightSpot);
	SpotLight->setKAttenuation(Interface->lightAttSpot);

	}

void drawSkybox()
{
	// Draw skybox as last
	shaderSkybox->use();
	glm::mat4 view = glm::mat4(glm::mat3(Camara->getView()));	// Remove any translation component of the view matrix

	shaderSkybox->setMat4("View", view);
	shaderSkybox->setMat4("Proj", Proj);
	shaderSkybox->setInt("skybox", 0);

	// skybox cube
	glDepthFunc(GL_LEQUAL);  
	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
	glDepthFunc(GL_LESS);
}
/**
 * Render Function
 * */
void render()
{
    // Clears the color and depth buffers from the frame buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    /** Draws code goes here **/
    // Use the shader
	Shader* shaderActual = shaderAllLight;
	
	for (size_t i = 0; i < modelsObj.size(); i++)
	{
		char shad = modelsObj[i]->getShader();
		switch (shad)
		{
			//Blinn
			case 'b':
			renderObj(shaderAllLight, i);
				break;
			//Cook torrance
			case 'c':
			renderObj(shaderAllLightCookTorrence, i);
				break;
			//Oren nayar
			case 'o':
			renderObj(shaderAllLightOrenayer, i);
				break;
			//Normal mapping
			case 'n':
				renderObj(shadernormalMapping, i);
				break;
			//Occlussion parallax mapping
			case 'p':
				renderObj(shaderAllLight, i);
				break;
			//Refraction
			case 'r':
				renderObj(shaderRefraction, i);
				break;
			//Reflection
			case 'l':
				renderObj(shaderEnviroment, i);
				break;
			//Objetos semitransparents
			case 't':
				renderObj(shaderAllLight, i);
				break;
			default:
				break;
		}
	}

	//Lights
	drawLights();
	
	//Skybox
	drawSkybox();

	//tweakbar
	TwDraw();

	//Update
	updateUserInterface();

    // Swap the buffer
    glfwSwapBuffers(window);
}
/**
 * App main loop
 * */
void update()
{
    // Loop until something tells the window, that it has to be closed
    while (!glfwWindowShouldClose(window))
    {
        // Checks for keyboard inputs
        processKeyboardInput(window);

        // Renders everything
        render();

		
        // Check and call events
        glfwPollEvents();
    }
}


/**
 * App starting point
 * @param{int} number of arguments
 * @param{char const *[]} running arguments
 * @returns{int} app exit code
 * */
int main(int argc, char const *argv[])
{
    // Initialize all the app components
    if (!init())
    {
        // Something went wrong
        std::cin.ignore();
        return -1;
    }

    std::cout << "=====================================================" << std::endl
              << "        Press Escape to close the program            " << std::endl
              << "=====================================================" << std::endl;

    // Starts the app main loop
    update();

    // Deletes the texture from the gpu
	glDeleteTextures(1, &textureID);
	glDeleteTextures(1, &textureID1);
	glDeleteTextures(1, &normalMap);
	

	for (size_t i = 0; i < modelsObj.size(); i++)
	{
		// Deletes the vertex array from the GPU
		glDeleteVertexArrays(1, &modelsObj[i]->VAO[0]);
		// Deletes the vertex object from the GPU
		glDeleteBuffers(1, &modelsObj[i]->VBO[0]);
		glDeleteBuffers(1, &modelsObj[i]->VBO[1]);
		glDeleteBuffers(1, &modelsObj[i]->VBO[2]);
		glDeleteBuffers(1, &modelsObj[i]->VBO[3]);
		glDeleteBuffers(1, &modelsObj[i]->VBO[4]);
	}

	// Destroy the shader
	delete shader;
	//delete shaderDirLight;
	//delete shaderPointLight;
	delete shaderEnviroment;
	delete shaderAllLight;
	delete shaderAllLightOrenayer;
	delete shaderRefraction;
	delete shaderAllLightCookTorrence;
	delete shadernormalMapping;
	delete shaderSpecMap;
	delete shaderSkybox;
	delete Camara;
	delete SpotLight;
	delete directionalLight;
	delete object;
	delete Interface;
	TwTerminate();
    // Stops the glfw program
    glfwTerminate();

    return 0;
}
