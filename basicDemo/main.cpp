#define N_SPOTLIGHTS 1
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
#include "main.h"

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
Shader *shader, *shaderLightDir, *shaderLightPoint;

unsigned int textureID;

//MVP Matrix
glm::mat4 Model;
glm::mat4 View;
glm::mat4 Proj;

//Camera
camera Camara;
float speedMouse = Camara.getSpeedMouse();

//Lights
enum modeLight {dir, point};
modeLight mode = dir;
light directionalLight(glm::vec3(0.0f, 5.0f, 5.0f));
pointLight PointLight(glm::vec3(0.0f, 5.0f, 5.0f));


//tweakBar
userInterface *Interface;

//Models
model object;
std::vector< model > modelsObj, lightsObj;

/**
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
	if (action == GLFW_PRESS) TwKeyPressed(key, TW_KMOD_NONE);

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
	if (Camara.getCameraMode()) {

		glfwSetCursorPos(window, windowWidth / 2.0, windowHeight / 2.0);
		GLfloat xoffset = ((windowWidth / 2.0) - xpos) * speedMouse;
		GLfloat yoffset = ((windowHeight / 2.0) - ypos) * speedMouse;
		Camara.updateInputMouse(xoffset, yoffset);

		/*yaw += xoffset;
		pitch += yoffset;
		std::cout << "main:\n" << yaw << ' ' << pitch << std::endl;*/
	}
}
void onCharacter(GLFWwindow* window, unsigned int codepoint) {
	TwKeyPressed(codepoint, TW_KMOD_NONE);
}
/**
 * initialize the user interface
 * @returns{bool} true if everything goes ok
* */
bool initUserInterface()
{
	if (!TwInit(TW_OPENGL_CORE, NULL))
		return false;

	Interface = userInterface::Instance();
	TwWindowSize(windowHeight, windowHeight);
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
	std::cout << "dsfsdfds: " << PointLight.getAmbientColor().x << std::endl;
	Model = glm::mat4(1.0f);
	View = Camara.getView();
	Proj = glm::perspective(45.0f, 800.0f / 600.0f, 0.1f, 190.0f);

}
/**
 * Builds all the geometry buffers and
 * loads them up into the GPU
 * (Builds a simple triangle)
 * */
void buildGeometry()
{
	std::vector< std::string > paths;
	//Paths
	paths.push_back(".\\assets\\models\\cubeS.obj");
	paths.push_back(".\\assets\\models\\pokeballS.obj");
	paths.push_back(".\\assets\\models\\catS.obj");
	//paths.push_back(".\\assets\\models\\light1S.obj");
	//Load models
	for (size_t i = 0; i < paths.size(); i++)
	{
		object = object.loadObj(paths[i]);

		// Creates on GPU the vertex array
		glGenVertexArrays(1, &object.VAO[0]);
		// Creates on GPU the vertex buffer object
		glGenBuffers(3, object.VBO);
		// Binds the vertex array to set all the its properties
		glBindVertexArray(object.VAO[0]);

		//vexter position object.VBO
		// Sets the buffer geometry data
		glBindBuffer(GL_ARRAY_BUFFER, object.VBO[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * object.vertex.size(), &object.vertex[0], GL_STATIC_DRAW);
		//vertex position position object.VAO
		// Sets the vertex attributes
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	
		//uv object.VBO
		// Sets the buffer geometry data
		glBindBuffer(GL_ARRAY_BUFFER, object.VBO[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * object.uv.size(), &object.uv[0], GL_STATIC_DRAW);

		//uv object.VAO
		// Sets the vertex attributes
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
		//glBindVertexArray(0);
		//color object.VBO
		// Sets the buffer geometry data
		glBindBuffer(GL_ARRAY_BUFFER, object.VBO[2]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * object.normal.size(), &object.normal[0], GL_STATIC_DRAW);

		//color object.VAO
		// Sets the vertex attributes
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glBindVertexArray(0);
		modelsObj.push_back(object);
	}

	//Load Lights
	for (size_t i = 0; i < N_SPOTLIGHTS; i++)
	{
		object = object.loadObj(".\\assets\\models\\light1S.obj");

		// Creates on GPU the vertex array
		glGenVertexArrays(1, &object.VAO[0]);
		// Creates on GPU the vertex buffer object
		glGenBuffers(3, object.VBO);
		// Binds the vertex array to set all the its properties
		glBindVertexArray(object.VAO[0]);

		//vexter position object.VBO
		// Sets the buffer geometry data
		glBindBuffer(GL_ARRAY_BUFFER, object.VBO[0]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * object.vertex.size(), &object.vertex[0], GL_STATIC_DRAW);
		//vertex position position object.VAO
		// Sets the vertex attributes
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		//uv object.VBO
		// Sets the buffer geometry data
		glBindBuffer(GL_ARRAY_BUFFER, object.VBO[1]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * object.uv.size(), &object.uv[0], GL_STATIC_DRAW);

		//uv object.VAO
		// Sets the vertex attributes
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
		//glBindVertexArray(0);
		//color object.VBO
		// Sets the buffer geometry data
		glBindBuffer(GL_ARRAY_BUFFER, object.VBO[2]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * object.normal.size(), &object.normal[0], GL_STATIC_DRAW);

		//color object.VAO
		// Sets the vertex attributes
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glBindVertexArray(0);
		lightsObj.push_back(object);
	}
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
	shaderLightDir = new Shader("assets/shaders/lightDirectional.vert", "assets/shaders/lightDirectional.frag");
	shaderLightPoint = new Shader("assets/shaders/lightPoint.vert", "assets/shaders/lightPoint.frag");
	
    // Loads all the geometry into the GPU
    buildGeometry();
    
	// Loads the texture into the GPU
    textureID = loadTexture("assets/textures/Neji.jpg");

	//Initializate MVP values
	initMVP();

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
		delete shaderLightDir;
		delete shaderLightPoint;
		shader = new Shader("assets/shaders/basic.vert", "assets/shaders/basic.frag");
		shaderLightDir = new Shader("assets/shaders/lightDirectional.vert", "assets/shaders/lightDirectional.frag");
		shaderLightPoint = new Shader("assets/shaders/lightPoint.vert", "assets/shaders/lightPoint.frag");

    }
	// Checks if the c key is pressed
	if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
	{
		//Changes camera mode
		if (!Camara.getCameraMode())
		{
			//cameraMode = true;
			Interface->hide();
		}
		else {
			//cameraMode = false;
			Interface->show();

		}
		Camara.changeCameraMode();
		//cameraMode = !cameraMode;
	}

	//Move camera
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		Camara.updateInputKeyboard('w');
		
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		Camara.updateInputKeyboard('s');
		
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		Camara.updateInputKeyboard('a');
		
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		Camara.updateInputKeyboard('d');
	
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		directionalLight.leftLightDir();
	
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		directionalLight.rightLightDir();

	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
		directionalLight.changeONOFF();


}

/*
* Update MVP
*/
void updateMVP()
{
	Model = glm::mat4(1.0f);
	View = Camara.getView();
	//Proj = glm::perspective(45.0f, (float)windowHeight / (float)windowWidth, 0.1f, 100.0f);
}
void renderPointLight() 
{
	// Use the shader
	shaderLightPoint->use();

	//MVP trnasformations
	updateMVP();
	glm::mat4 MVP = (Proj * View * Model);
	shaderLightPoint->setMat4("MVP", MVP);
	shaderLightPoint->setMat4("Model", Model);
	shaderLightPoint->setMat4("View", View);
	shaderLightPoint->setMat4("Proj", Proj);

	//Lights
	shaderLightPoint->setVec3("lightDir", directionalLight.getLightDir());
	shaderLightPoint->setVec3("ambientColor", directionalLight.getAmbientColor());
	shaderLightPoint->setVec3("diffuseColor", directionalLight.getDiffuseColor());
	shaderLightPoint->setVec3("specularColor", directionalLight.getSpecularColor());
	shaderLightPoint->setBool("on", directionalLight.getONOFF());

	//Draw models of the scene
	for (size_t i = 0; i < modelsObj.size(); i++)
	{
		//Material
		shaderLightPoint->setVec3("ka", modelsObj[i].getKAmbient());
		shaderLightPoint->setVec3("kd", modelsObj[i].getKDiffuse());
		shaderLightPoint->setVec3("ks", modelsObj[i].getKSpecular());
		shaderLightPoint->setFloat("n", modelsObj[i].getShinniness());

		//Texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);
		shaderLightPoint->setInt("text", 0);

		// Binds the vertex array to be drawn
		glBindVertexArray(modelsObj[i].VAO[0]);

		// Renders the triangle gemotry
		glDrawArrays(GL_TRIANGLES, 0, modelsObj[i].vertex.size());

	}
	glBindVertexArray(0);

}
void renderDirLight() 
{
	// Use the shader
	shaderLightDir->use();

	//MVP trnasformations
	updateMVP();
	glm::mat4 MVP = (Proj * View * Model);
	shaderLightDir->setMat4("MVP", MVP);
	shaderLightDir->setMat4("Model", Model);
	shaderLightDir->setMat4("View", View);
	shaderLightDir->setMat4("Proj", Proj);

	//Lights
	shaderLightDir->setVec3("lightDir", directionalLight.getLightDir());
	shaderLightDir->setVec3("ambientColor", directionalLight.getAmbientColor());
	shaderLightDir->setVec3("diffuseColor", directionalLight.getDiffuseColor());
	shaderLightDir->setVec3("specularColor", directionalLight.getSpecularColor());
	shaderLightDir->setBool("on", directionalLight.getONOFF());

	//Draw models of the scene
	for (size_t i = 0; i < modelsObj.size(); i++)
	{
		//Material
		shaderLightDir->setVec3("ka", modelsObj[i].getKAmbient());
		shaderLightDir->setVec3("kd", modelsObj[i].getKDiffuse());
		shaderLightDir->setVec3("ks", modelsObj[i].getKSpecular());
		shaderLightDir->setFloat("n", modelsObj[i].getShinniness());

		//Texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);
		shaderLightDir->setInt("text", 0);

		// Binds the vertex array to be drawn
		glBindVertexArray(modelsObj[i].VAO[0]);
    
		// Renders the triangle gemotry
		glDrawArrays(GL_TRIANGLES, 0, modelsObj[i].vertex.size());
		
	}

	glBindVertexArray(0);
	//Light models
	// Use the shader
	shader->use();

	//MVP trnasformations
	updateMVP();
	MVP = (Proj * View * Model);
	shader->setMat4("MVP", MVP);

	//Draw models of the scene
	for (size_t i = 0; i < lightsObj.size(); i++)
	{
		//Material
		shader->setVec3("ka", lightsObj[i].getKAmbient());
		shader->setVec3("kd", lightsObj[i].getKDiffuse());
		shader->setVec3("ks", lightsObj[i].getKSpecular());
		shader->setFloat("n", lightsObj[i].getShinniness());

		//Texture
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textureID);
		shader->setInt("text", 0);

		// Binds the vertex array to be drawn
		glBindVertexArray(lightsObj[i].VAO[0]);

		// Renders the triangle gemotry
		glDrawArrays(GL_TRIANGLES, 0, lightsObj[i].vertex.size());

	}
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
    // Use the shader
	//shader->use();
	if (mode == dir)
	{
		renderDirLight();		
	}
	else if (mode == point)
	{
		renderPointLight();
	}


	//tweakbar
	TwDraw();

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

	for (size_t i = 0; i < modelsObj.size(); i++)
	{
		// Deletes the vertex array from the GPU
		glDeleteVertexArrays(1, &modelsObj[i].VAO[0]);
		// Deletes the vertex object from the GPU
		glDeleteBuffers(1, &modelsObj[i].VBO[0]);
		glDeleteBuffers(1, &modelsObj[i].VBO[1]);
		glDeleteBuffers(1, &modelsObj[i].VBO[2]);

	}

	// Destroy the shader
	delete shader;
	delete shaderLightDir;
	delete Interface;

	TwTerminate();
    // Stops the glfw program
    glfwTerminate();

    return 0;
}
