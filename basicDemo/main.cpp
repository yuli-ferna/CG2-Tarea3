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

#include "Shader.h"
#include <vector>
#include <map>

#define px(x) x  
// Window current width
unsigned int windowWidth = 800;
// Window current height
unsigned int windowHeight = 600;
// Window title
const char* windowTitle = "Yuliana Fernandez";
// Window pointer
GLFWwindow* window;
// Shader object
Shader* shader, * shaderAllLight;

//Textures
unsigned int textureID;
unsigned int textureID1, specularMap, normalMap, dispMap, blend, depthMap;
unsigned int cubemapTexture;
std::vector<unsigned int> textures;

//skybox
unsigned int planeVAO, planeVBO;

//MVP Matrix
glm::mat4 Model;
glm::mat4 View;
glm::mat4 Proj;

//Camera
camera* Camara = new camera();
float speedMouse = Camara->getSpeedMouse();
float currentTime, deltaTime;
float lastTime = glfwGetTime();


//Models
model* object;
std::vector< model* > modelsObj;

//Framebuffer
//Transformar al espacio de luz
float near_plane, far_plane;
glm::mat4 lightProjection;
glm::mat4 lightView;
glm::mat4 lightSpaceMatrix;
unsigned int depthMapFBO;
const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
unsigned int quadVAO = 0;
unsigned int quadVBO;

/* *
 * Handles the window resize
 * @param{GLFWwindow} window pointer
 * @param{int} new width of the window
 * @param{int} new height of the window
 * */
void resize(GLFWwindow* window, int width, int height)
{
	windowWidth = width;
	windowHeight = height;
	// Sets the OpenGL viewport size and position
	glViewport(0, 0, windowWidth, windowHeight);

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
			}
			else {
				//cameraMode = false;

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
		GLfloat xoffset = ((windowWidth / 2.0) - xpos) * speedMouse * deltaTime;
		GLfloat yoffset = ((windowHeight / 2.0) - ypos) * speedMouse * deltaTime;
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
	
}
/**
 * initialize the user interface
 * @returns{bool} true if everything goes ok
* */
bool initUserInterface()
{
	if (!TwInit(TW_OPENGL_CORE, NULL))
		return false;

	//Interface = new userInterface();
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
	//Blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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

void buildModel(std::string path, std::vector< glm::vec3 > position, std::vector< model* >& arrayObj)
{
	std::vector< glm::vec3 > allVert;
	std::vector< glm::vec3 > Vert;
	std::vector< glm::vec3 > Normal;
	std::vector< glm::vec3 > Tangent;
	std::vector< glm::vec3 > Bitangent;
	std::vector< glm::vec2 > UV;
	//Positions

	model* obj = new model(position[0]);
	obj->loadObj(path, Vert, Normal, UV);
	obj->getTangentBitanget(Vert, UV, Normal, Tangent, Bitangent, allVert);

	//int nMod = 3;
	//Load 3 models
	unsigned int VAOForm[1], VBOForm[NVBOS], numVertex = Vert.size();
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
		std::cout << position[i].x << ' ' << position[i].y << position[i].z << std::endl;
		obj1->setPosition(position[i]);
		arrayObj.push_back(obj1);
	}

	position.clear();
}

void buildPlane()
{
	float vertices[] = {
		// positions         // colors
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top 
	};

	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(planeVAO);

	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

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
	pos.push_back(glm::vec3(0.0f, 5.0f, 0.0f));
	//Carga el mismo modelo en las distintas posiciones que tiene el arreglo
	buildModel(".\\assets\\models\\Crate.obj", pos, modelsObj);
	pos.clear();

	//Load skybox
	//buildSkyBox();
}
/**
 * Loads a texture into the GPU
 * @param{const char} path of the texture file
 * @returns{unsigned int} GPU texture index
 * */
unsigned int loadTexture(const char* path)
{
	unsigned int id;
	// Creates the texture on GPU
	glGenTextures(1, &id);
	// Loads the texture
	int textureWidth, textureHeight, numberOfChannels;
	// Flips the texture when loads it because in opengl the texture coordinates are flipped
	stbi_set_flip_vertically_on_load(true);
	// Loads the texture file data
	unsigned char* data = stbi_load(path, &textureWidth, &textureHeight, &numberOfChannels, 0);
	if (data)
	{
		// Gets the texture channel format
		GLenum format1;
		GLenum format2;
		switch (numberOfChannels)
		{
		case 1:
			format1 = GL_RED;
			format2 = GL_RED;
			break;
		case 3:
			format1 = GL_SRGB;
			format2 = GL_RGB;
			break;
		case 4:
			format1 = GL_SRGB_ALPHA;
			format2 = GL_RGBA;
			break;

		}

		// Binds the texture
		glBindTexture(GL_TEXTURE_2D, id);
		// Creates the texture
		glTexImage2D(GL_TEXTURE_2D, 0, format1, textureWidth, textureHeight, 0, format2, GL_UNSIGNED_BYTE, data);
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
	textureID = loadTexture("assets/textures/wood.png");
	//textureID1 = loadTexture("assets/textures/container2.png");
	//textureID1 = loadTexture("assets/textures/bricks2.jpg");
	normalMap = loadTexture("assets/textures/bricks2_normal.jpg");
	unsigned int normalMap1 = loadTexture("assets/textures/toy_box_normal.png");
	dispMap = loadTexture("assets/textures/bricks2_disp.jpg");
	unsigned int dispMap1 = loadTexture("assets/textures/toy_box_disp.png");
	blend = loadTexture("assets/textures/blending_transparent_window.png");
	specularMap = loadTexture("assets/textures/container2_specular.png");

	textures.push_back(textureID1);
	textures.push_back(textureID1);
	textures.push_back(textureID1);
	textures.push_back(textureID1);
	textures.push_back(textureID);
	textures.push_back(textureID);
	textures.push_back(textureID1);
	textures.push_back(textureID1);
	textures.push_back(textureID1);
	std::vector<unsigned int> normal
	{
		normalMap, normalMap, normalMap, normalMap,
		normalMap1, normalMap1, normalMap, normalMap,
		normalMap
	};
	std::vector<unsigned int> disp
	{
		dispMap, dispMap, dispMap, dispMap,
		dispMap1, dispMap1, dispMap, dispMap,
		dispMap
	};
	for (int i = 0; i < modelsObj.size(); i++)
	{
		modelsObj[i]->texture.diffuse = textures[i];
		modelsObj[i]->texture.normal = normal[i];
		modelsObj[i]->texture.disp = disp[i];
		modelsObj[i]->texture.blend = blend;
	}

	//Load skybox
	std::vector<std::string> faces
	{
		"assets/textures/skybox/right.bmp",
		"assets/textures/skybox/left.bmp",
		"assets/textures/skybox/top.bmp",
		"assets/textures/skybox/bottom.bmp",
		"assets/textures/skybox/front.bmp",
		"assets/textures/skybox/back.bmp"
	};
	cubemapTexture = loadCubemap(faces);
	//textures.push_back(cubemapTexture);

}

void initFramebuffer() {

	//Creamos el objeto de framebuffer
	glGenFramebuffers(1, &depthMapFBO);

	//creamos una textura 2D que usaremos como buffer de profundidad del framebuffer

	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
		SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	//Con la textura de profundidad generada podemos adjuntarla como b�fer de profundidad del framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

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
	shaderAllLight = new Shader("assets/shaders/allLight.vert", "assets/shaders/allLight.frag");

	// Loads all the geometry into the GPU
	buildGeometry();
	buildPlane();

	//Framebuffer
	initFramebuffer();

	// Loads the texture into the GPU
	initTexture();

	//Initializate MVP values
	initMVP();

	
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
void processKeyboardInput(GLFWwindow* window)
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
		delete shaderAllLight;

		shader = new Shader("assets/shaders/basic.vert", "assets/shaders/basic.frag");
		shaderAllLight = new Shader("assets/shaders/allLight.vert", "assets/shaders/allLight.frag");

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

}

/*
* Update MVP
*/
void updateMVP(int i, glm::vec3 pos)
{
	Model = glm::mat4(1.0f);

	Model = glm::translate(glm::mat4(1.0f), pos);

	View = Camara->getView();
	//Proj = glm::perspective(45.0f, (float)windowHeight / (float)windowWidth, 0.1f, 100.0f);
}


void renderPlane(Shader* shaderActual)
{
	shaderActual->use();
	updateMVP(0, glm::vec3(0.0f, 0.0f, -1.0f));
	shaderActual->setMat4("Model", Model);
	shaderActual->setMat4("View", View);
	shaderActual->setMat4("Proj", Proj);

	glBindVertexArray(planeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);

}

void renderObj(Shader* shaderActual, int i, std::vector< model* > arrayObj)
{
	glm::mat4 MVP;
	// Use the shader
	shaderActual->use();

	//Light directional
	shaderActual->setMat4("lightSpaceMatrix", lightSpaceMatrix);

	shaderActual->setVec3("viewPos", Camara->getPosition());


	updateMVP(i, arrayObj[i]->getPosition());
	shaderActual->setMat4("Model", Model);
	shaderActual->setMat4("View", View);
	shaderActual->setMat4("Proj", Proj);

	//Material
	shaderActual->setVec3("ka", arrayObj[i]->getKAmbient());
	shaderActual->setVec3("kd", arrayObj[i]->getKDiffuse());
	shaderActual->setVec3("ks", arrayObj[i]->getKSpecular());
	shaderActual->setFloat("n", arrayObj[i]->getShinniness());
	shaderActual->setFloat("indexMaterial", arrayObj[i]->getindexMaterial());
	shaderActual->setFloat("indexAmbiental", arrayObj[i]->getindexAmbient());
	shaderActual->setFloat("intensityParalax", arrayObj[i]->getintensityParalax());
	shaderActual->setFloat("percentAmbient", arrayObj[i]->getpercentAmbient());
	shaderActual->setFloat("roughness", arrayObj[i]->getRoughness());
	shaderActual->setBool("albedo", arrayObj[i]->getAlbedo());

	//Texture
	/*glActiveTexture(GL_TEXTURE0 + depthMap - 1);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	shaderActual->setInt("shadowMap", depthMap - 1);*/

	/*glActiveTexture(GL_TEXTURE0 + modelsObj[i]->texture.diffuse - 1);
	glBindTexture(GL_TEXTURE_2D, modelsObj[i]->texture.diffuse);
	shaderActual->setInt("text", modelsObj[i]->texture.diffuse - 1);*/

	/*glActiveTexture(GL_TEXTURE0 + modelsObj[i]->texture.specular - 1);
	glBindTexture(GL_TEXTURE_2D, modelsObj[i]->texture.specular);
	shaderActual->setInt("specMap", modelsObj[i]->texture.specular - 1);*/

	/*glActiveTexture(GL_TEXTURE0 + modelsObj[i]->texture.normal - 1);
	glBindTexture(GL_TEXTURE_2D, modelsObj[i]->texture.normal);
	shaderActual->setInt("normalMap", modelsObj[i]->texture.normal - 1);*/

	/*glActiveTexture(GL_TEXTURE0 + modelsObj[i]->texture.disp - 1);
	glBindTexture(GL_TEXTURE_2D, modelsObj[i]->texture.disp);
	shaderActual->setInt("dispMap", modelsObj[i]->texture.disp - 1);*/

	// Binds the vertex array to be drawn
	glBindVertexArray(arrayObj[i]->VAO[0]);

	// Renders the triangle gemotry
	glDrawArrays(GL_TRIANGLES, 0, arrayObj[i]->numVertex);

	glBindVertexArray(0);
}

void updateUserInterface()
{
	//Model
}

void renderModels()
{
}

void renderQuad()
{
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	shader->use();
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}
/**
 * Render Function
 * */
void render()
{
	// Clears the color and depth buffers from the frame buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/** Draws code goes here **/

	//renderLightView();
	//glCullFace(GL_FRONT);
	//renderModels();
	//glCullFace(GL_BACK);
	renderPlane(shader);
	//if (Interface->lightView)
	//{
	//	//pintar quad
	//	shaderquadDepthMap->use();
	//	shaderquadDepthMap->setInt("depthMap", 0);
	//	glActiveTexture(GL_TEXTURE0);
	//	glBindTexture(GL_TEXTURE_2D, depthMap);
		//renderQuad();
	//}
	
	//Update
	updateUserInterface();

	// Swap the buffer
	glfwSwapBuffers(window);
}
/*
* change speed
*/
void updateCameraSpeed()
{
	currentTime = glfwGetTime();
	deltaTime = currentTime - lastTime;
	lastTime = currentTime;

	Camara->setDeltatime(deltaTime);
}


/**
 * App main loop
 * */
void update()
{
	// Loop until something tells the window, that it has to be closed
	while (!glfwWindowShouldClose(window))
	{
		//Update camera speed
		updateCameraSpeed();
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
int main(int argc, char const* argv[])
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
	glDeleteTextures(1, &specularMap);
	glDeleteTextures(1, &dispMap);
	glDeleteTextures(1, &blend);
	glDeleteTextures(1, &cubemapTexture);


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
	delete shaderAllLight;
	//delete shaderDirLight;
	//delete shaderPointLight;
	delete Camara;
	delete object;
	TwTerminate();
	// Stops the glfw program
	glfwTerminate();

	return 0;
}
