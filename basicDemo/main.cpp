#include <glad/glad.h> // Glad has to be include before glfw
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>

#include <stb_image.h>
#include "userInterface.h"
#include "model.h"

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
Shader *shader;
// Index (GPU) of the geometry buffer
unsigned int VBO[3];
// Index (GPU) vertex array object
unsigned int VAO[1];
// Index (GPU) of the texture
unsigned int textureID;

//tweakBar
userInterface *Interface;

//Models
std::vector< model > modelsObj;
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

/**
 * Builds all the geometry buffers and
 * loads them up into the GPU
 * (Builds a simple triangle)
 * */
void buildGeometry()
{
	float triangleVertices[] =
	{
		-0.5f, -0.5f, 0.0f,
		 0.0f, 0.0f, 0.5f,  // Color
		-0.5f,  0.5f, 0.0f,
		 0.0f, 0.0f, 0.5f,  // Color
		 0.5f,  0.5f, 0.0f,
		 0.0f, 0.0f, 0.5f,  // Color
		 0.5f, -0.5f, 0.0f,
		 0.0f, 0.0f, 0.5f,  // Color
		-0.5f, -0.5f, 0.0f,
		 0.0f, 0.0f, 0.5f   // Color
	};
	static const float vertex[] =
	{
		 1.0f, -1.0f, -1.0f, 1.0f,
		 1.0f, -1.0f,  1.0f, 1.0f,
		-1.0f, -1.0f,  1.0f, 1.0f,
		-1.0f, -1.0f, -1.0f, 1.0f,
		 1.0f,  1.0f, -1.0f, 1.0f,
		 1.0f,  1.0f,  1.0f, 1.0f,
		-1.0f,  1.0f,  1.0f, 1.0f,
		-1.0f,  1.0f, -1.0f, 1.0f
	};
	model object;
	
	object = object.loadObj(".\\assets\\models\\cube2.obj");
/*
	object.vertex.push_back(glm::vec3(0.5f, -0.5f, 0.0f));
	object.vertex.push_back(glm::vec3(-0.5f, -0.5f, 0.0f));
	object.vertex.push_back(glm::vec3(0.5f, 0.5f, 0.0f));
	*/
	object.color.push_back(glm::vec3(1.0f, 0.0f, 0.0f));
	object.color.push_back(glm::vec3(0.0f, 1.0f, 0.0f));
	object.color.push_back(glm::vec3(0.0f, 0.0f, 1.0f));

	/*object.uv.push_back(glm::vec2(0.0f, 0.0f));
	object.uv.push_back(glm::vec2(0.5f, 1.0f));
	object.uv.push_back(glm::vec2(1.0f, 0.0f));*/

	// Creates on GPU the vertex array
    glGenVertexArrays(1, &VAO[0]);
    // Creates on GPU the vertex buffer object
    glGenBuffers(3,VBO);
    // Binds the vertex array to set all the its properties
    glBindVertexArray(VAO[0]);

	//vexter position VBO
    // Sets the buffer geometry data
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * object.vertex.size(), &object.vertex[0], GL_STATIC_DRAW);
	//vertex position position VAO
    // Sets the vertex attributes
    glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	
	//uv VBO
	// Sets the buffer geometry data
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * object.uv.size(), &object.uv[0], GL_STATIC_DRAW);

	//uv VAO
	// Sets the vertex attributes
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glBindVertexArray(1);
	//color VBO
	// Sets the buffer geometry data
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * object.color.size(), &object.color[0], GL_STATIC_DRAW);

	//color VAO
	// Sets the vertex attributes
	glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glBindVertexArray(2);


	modelsObj.push_back(object);
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
	
    // Loads all the geometry into the GPU
    buildGeometry();
    // Loads the texture into the GPU
    textureID = loadTexture("assets/textures/bricks2.jpg");

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
        shader = new Shader("assets/shaders/basic.vert", "assets/shaders/basic.frag");
    }
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
    shader->use();

	//MVP trnasformations
	glm::mat4 model = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));
	model = glm::scale(model, glm::vec3(0.5, 0.5, 0.5));
	model = glm::mat4(1.0f);
	glm::mat4 view = glm::lookAt(glm::vec3(4, 3, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	glm::mat4 proj = glm::perspective(45.0f, 800.0f / 600.0f, 0.1f, 100.0f);

	glm::mat4 MVP = (proj * view * model);
	shader->setMat4("MVP", MVP);

	//Texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	shader->setInt("text", 0);
    
	// Binds the vertex array to be drawn
    glBindVertexArray(VAO[0]);
    
	// Renders the triangle gemotry
    glDrawArrays(GL_TRIANGLES, 0, modelsObj[0].vertex.size());
    glBindVertexArray(0);


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
    // Deletes the vertex array from the GPU
    glDeleteVertexArrays(1, &VAO[0]);
    // Deletes the vertex object from the GPU
	glDeleteBuffers(1, &VBO[0]);
	glDeleteBuffers(1, &VBO[1]);
	glDeleteBuffers(1, &VBO[2]);
    // Destroy the shader
    delete shader;
	delete Interface;

	TwTerminate();
    // Stops the glfw program
    glfwTerminate();

    return 0;
}
