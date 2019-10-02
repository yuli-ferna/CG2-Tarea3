#define _CRT_SECURE_NO_DEPRECATE
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
#include "volume.h"

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
Shader *shader, *shaderCube, *shaderFramebuffer, * shaderRayCasting;

//Textures
unsigned int textureID;
unsigned int depthMap;
//back face culling para el cubo
unsigned int backFaceCubeTexture;
unsigned int depthrenderbuffer;

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

//Volume
const char* path = "assets/models/bonsai_256x256x256_uint8.raw";
//volume* volumeObj = new volume(path);
int XDIM = 256, YDIM = 256, ZDIM = 256;
int size = XDIM * YDIM * ZDIM;
float step = 0.0f;
bool cubeView = true;
//Framebuffer
//Transformar al espacio de luz
float near_plane, far_plane;
glm::mat4 lightProjection;
glm::mat4 lightView;
glm::mat4 lightSpaceMatrix;
unsigned int depthMapFBO;
const unsigned int SHADOW_WIDTH = 800, SHADOW_HEIGHT = 600;
//Framebuffer backface culling
unsigned int backfaceFBO;

unsigned int cubeVAO = 0;
unsigned int cubeVBO;
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
void onKeyPress(GLFWwindow* window, int key, int scancode, int action, int mods) 
{
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
				glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
			}
			else {
				//cameraMode = false;

			}
			Camara->changeCameraMode();
			//cameraMode = !cameraMode;
			break;
		case GLFW_KEY_P:
			cubeView = !cubeView;
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
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
		/*yaw += xoffset;
		pitch += yoffset;
		std::cout << "main:\n" << yaw << ' ' << pitch << std::endl;*/
	}
	else
	{
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	}
	
}

void onCharacter(GLFWwindow* window, unsigned int codepoint) 
{
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
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);

	glEnable(GL_FRAMEBUFFER_SRGB);
	////Blending
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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

bool LoadVolumeFromFile(const char* fileName)
{

	FILE* pFile = fopen(fileName, "rb");
	if (NULL == pFile) {

		return false;
	}
	GLubyte* pVolume = new GLubyte[size];
	fread(pVolume, sizeof(GLubyte), size, pFile);
	fclose(pFile);
	unsigned int t;
	//load data into a 3D texture
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_3D, textureID);

	// set the texture parameters
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexImage3D(GL_TEXTURE_3D, 0, GL_RED, XDIM, YDIM, ZDIM, 0, GL_RED, GL_UNSIGNED_BYTE, pVolume);

	delete[] pVolume;
	return true;
}

/* *
 * Builds all the geometry buffers and
 * loads them up into the GPU
 * (Builds a simple triangle)
 * */
void buildVolume()
{
	LoadVolumeFromFile(path);

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


void initTexture() {

	
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
GLenum DrawBuffers[1] = { GL_COLOR_ATTACHMENT0 };

void initFramebufferBackface()
{

	// The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
	glGenFramebuffers(1, &backfaceFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, backfaceFBO);

	// The texture we're going to render to
	glGenTextures(1, &backFaceCubeTexture);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, backFaceCubeTexture);

	// Give an empty image to OpenGL ( the last "0" means "empty" )
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, windowWidth, windowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);

	// Poor filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// The depth buffer
	glGenRenderbuffers(1, &depthrenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, windowWidth, windowHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);

	
	// Set "backFaceCubeTexture" as our colour attachement #0
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, backFaceCubeTexture, 0);

	//// Depth texture alternative : 
	//glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthTexture, 0);


	// Set the list of draw buffers.
	glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers
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
	shaderCube = new Shader("assets/shaders/cube.vert", "assets/shaders/cube.frag");
	shaderFramebuffer = new Shader("assets/shaders/framebuffer.vert", "assets/shaders/framebuffer.frag");
	shaderRayCasting = new Shader("assets/shaders/rayCasting.vert", "assets/shaders/rayCasting.frag");

	// Loads all the geometry into the GPU
	buildVolume();

	//Framebuffer
	//initFramebuffer();
	initFramebufferBackface();

	/*initCal*/
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
		delete shaderCube;
		delete shaderFramebuffer;
		delete shaderRayCasting;

		shader = new Shader("assets/shaders/basic.vert", "assets/shaders/basic.frag");
		shaderCube = new Shader("assets/shaders/cube.vert", "assets/shaders/cube.frag");
		shaderFramebuffer = new Shader("assets/shaders/framebuffer.vert", "assets/shaders/framebuffer.frag");
		shaderRayCasting = new Shader("assets/shaders/rayCasting.vert", "assets/shaders/rayCasting.frag");

	}

	if (!cubeView)
	{
		//Volume
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && step < 1.0f)
			step += 1.0f / 256;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && step > 0.0f)
			step -= 1.0f / 256;

	}
	else {

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


void renderCube()
{
	if (cubeVAO == 0)
	{
		float cubeVertex[] = {
			-0.5f,-0.5f,-0.5f,
			-0.5f,-0.5f, 0.5f,
			-0.5f, 0.5f, 0.5f,
			 0.5f, 0.5f,-0.5f,
			-0.5f,-0.5f,-0.5f,
			-0.5f, 0.5f,-0.5f,
			 0.5f,-0.5f, 0.5f,
			-0.5f,-0.5f,-0.5f,
			 0.5f,-0.5f,-0.5f,
			 0.5f, 0.5f,-0.5f,
			 0.5f,-0.5f,-0.5f,
			-0.5f,-0.5f,-0.5f,
			-0.5f,-0.5f,-0.5f,
			-0.5f, 0.5f, 0.5f,
			-0.5f, 0.5f,-0.5f,
			 0.5f,-0.5f, 0.5f,
			-0.5f,-0.5f, 0.5f,
			-0.5f,-0.5f,-0.5f,
			-0.5f, 0.5f, 0.5f,
			-0.5f,-0.5f, 0.5f,
			 0.5f,-0.5f, 0.5f,
			 0.5f, 0.5f, 0.5f,
			 0.5f,-0.5f,-0.5f,
			 0.5f, 0.5f,-0.5f,
			 0.5f,-0.5f,-0.5f,
			 0.5f, 0.5f, 0.5f,
			 0.5f,-0.5f, 0.5f,
			 0.5f, 0.5f, 0.5f,
			 0.5f, 0.5f,-0.5f,
			-0.5f, 0.5f,-0.5f,
			 0.5f, 0.5f, 0.5f,
			-0.5f, 0.5f,-0.5f,
			-0.5f, 0.5f, 0.5f,
			 0.5f, 0.5f, 0.5f,
			-0.5f, 0.5f, 0.5f,
			 0.5f,-0.5f, 0.5f
		};

		glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1, &cubeVBO);
		glBindVertexArray(cubeVAO);
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertex), &cubeVertex, GL_STATIC_DRAW);
		// Position
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	}


	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

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

	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void renderFramebuffer() 
{
	glBindFramebuffer(GL_FRAMEBUFFER, backfaceFBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glViewport(0, 0, windowWidth, windowHeight);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);

	shaderCube->use();
	//MVP ???
	updateMVP(0, glm::vec3(0.0f, 0.0f, 0.0f));
	shaderCube->setMat4("Model", Model);
	shaderCube->setMat4("View", View);
	shaderCube->setMat4("Proj", Proj);
	//Texture
	glEnable(GL_TEXTURE_3D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_3D, textureID);

	shaderCube->setFloat("step", step);

	renderCube();
	glCullFace(GL_BACK);

	//Testing backface cube texture

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//shaderFramebuffer->use();
	//glActiveTexture(GL_TEXTURE0);
	//glBindTexture(GL_TEXTURE_2D, backFaceCubeTexture);
	//shaderFramebuffer->setInt("backFaceCubeTexture", 0);
	//renderQuad();
	
	// reset viewport
	/*glViewport(0, 0, windowWidth, windowHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
*/
}

void renderVolume()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, windowWidth, windowHeight);

	shaderRayCasting->use();
	updateMVP(0, glm::vec3(0.0f));
	//MVP matrix
	shaderRayCasting->setMat4("Model", Model);
	shaderRayCasting->setMat4("View", View);
	shaderRayCasting->setMat4("Proj", Proj);
	//Window size
	shaderRayCasting->setVec2("winSize", glm::vec2(windowWidth, windowHeight));
	// Texture 3D
	glEnable(GL_TEXTURE_3D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_3D, textureID);
	shaderRayCasting->setInt("volumeText", 0);
	// Texture 2D
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, backFaceCubeTexture);
	shaderFramebuffer->setInt("backFaceCubeTexture", 1);
	renderCube();
}
/**
 * Render Function
 * */
void render()
{
	// Clears the color and depth buffers from the frame buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/** Draws code goes here **/

	renderFramebuffer();
	glViewport(0, 0, windowWidth, windowHeight*0.8f);

	if (cubeView)
	{
		renderVolume();		
	}
	else 
	{
		shader->use();
		//MVP ???
		updateMVP(0, glm::vec3(0.0f, 0.0f, 0.0f));
		shader->setMat4("Model", Model);
		shader->setMat4("View", View);
		shader->setMat4("Proj", Proj);
		//Texture
		glEnable(GL_TEXTURE_3D);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_3D, textureID);

		shader->setFloat("step", step);

		renderQuad();
	}
	
	//Update
	//updateUserInterface();
	glViewport(0, 0, windowWidth, windowHeight);

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
	glDeleteTextures(1, &depthMap);
	glDeleteTextures(1, &backFaceCubeTexture);
	
	
	glDeleteFramebuffers(1, &depthMapFBO);
	glDeleteFramebuffers(1, &backfaceFBO);
	glDeleteRenderbuffers(1, &depthrenderbuffer);
	glDeleteBuffers(1, &cubeVAO);
	glDeleteBuffers(1, &quadVAO);
	glDeleteBuffers(1, &cubeVBO);
	glDeleteBuffers(1, &quadVBO);

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
	delete shaderCube;
	delete shaderFramebuffer;
	delete shaderRayCasting;
	delete Camara;
	delete object;
	TwTerminate();
	// Stops the glfw program
	glfwTerminate();

	return 0;
}
