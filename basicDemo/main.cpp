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
unsigned int VBO[2];
// Index (GPU) vertex array object
unsigned int VAO[2];
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

void loadObj(std::string path)
{
	std::ifstream file = std::ifstream(path);
    if (!file.is_open()) {
		std::cout << "No se ecuentra: " << path << std::endl;
    }

	std::vector< glm::vec3 > allVert;
	std::vector< glm::vec3 > allNormal;
	std::vector< glm::vec2 > allUV;
	std::vector< unsigned int > vertInd, normInd, uvInd;
	//int count = 0;
    while (file) {
        std::string token, first, trash;
		float vx, vy, vz;
        getline(file, token);
		std::istringstream str(token);
		str >> first;
        std::cout << " " << first << std::endl;
		//Vertex position
		if (first == "v")
		{
			str >> vx >> vy >> vz;
			allVert.push_back(glm::vec3(vx, vy, vz));

		}
		//Coordenadas uv
		else if (first == "vt")
		{
			str >> vx >> vy;
			allUV.push_back(glm::vec2(vx, vy));

		}
		//Normal
		else if (first == "vn")
		{
			str >> vx >> vy >> vz;
			allNormal.push_back(glm::vec3(vx, vy, vz));

		}
		//faces
		else if (first == "f")
		{
			unsigned int vertIndex[3], uvIndex[3], normalIndex[3];
			std::replace_if(std::begin(token), std::end(token), [](const char& ch) { return ch == '/'; }, ' ');

			std::istringstream face_str(token);
			face_str.ignore(token.length(), ' ');

			face_str >> vertIndex[0] >> uvIndex[0] >> normalIndex[0] 
				>> vertIndex[1] >> uvIndex[1] >> normalIndex[1] 
				>> vertIndex[2] >> uvIndex[2] >> normalIndex[2];

			//Se le resta 1 porque el index de los vertices en el obj empieza en 1
			vertInd.push_back(vertIndex[0] - 1);
			vertInd.push_back(vertIndex[1] - 1);
			vertInd.push_back(vertIndex[2] - 1);

			uvInd.push_back(uvIndex[0] - 1);
			uvInd.push_back(uvIndex[1] - 1);
			uvInd.push_back(uvIndex[2] - 1);

			normInd.push_back(normalIndex[0] - 1);
			normInd.push_back(normalIndex[1] - 1);
			normInd.push_back(normalIndex[2] - 1);
		}
		else if (first == "usmtl")
		{
			//img
		}
		else if (first == "mtllib")
		{
			//mtl
		}
		else if (first == "s")
		{

		}

    }
	
	
	//Creando el arreglo final
	std::vector< glm::vec3 > Vert;
	std::vector< glm::vec3 > Normal;
	std::vector< glm::vec2 > UV;
	for (int i = 0; i < vertInd.size(); i++)
	{
		Vert.push_back(allVert[vertInd[i]]);
		UV.push_back(allUV[uvInd[i]]);
		Normal.push_back(allNormal[normInd[i]]);
	}

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
	
	loadObj(".\\assets\\models\\cube2.obj");
	glm::mat4 model1 = glm::rotate(model1, 45.0f, glm::vec3(1.0f, 0.0f, 0.0f));

	glm::mat4 view = glm::lookAt(glm::vec3(4, 3, -3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	glm::mat4 proj = glm::perspective(45.0f, 800.0f / 600.0f, 0.1f, 100.0f);

	glm::mat4 MVP = proj * view * model1;
	object.vertex.push_back(glm::vec4(0.5f, -0.5f, 0.0f, 1.0f));
	object.vertex.push_back(glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f));
	object.vertex.push_back(glm::vec4(0.5f, 0.5f, 0.0f, 1.0f));

	object.color.push_back(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	object.color.push_back(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
	object.color.push_back(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));

    // Creates on GPU the vertex array
    glGenVertexArrays(1, &VAO[0]);
    // Creates on GPU the vertex buffer object
    glGenBuffers(2, VBO);
    // Binds the vertex array to set all the its properties
    glBindVertexArray(VAO[0]);
    // Sets the buffer geometry data
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	//vexter position VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * object.vertex.size(), &object.vertex[0], GL_STATIC_DRAW);
    // Sets the vertex attributes
    glEnableVertexAttribArray(0);
	//vertex position position VAO
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	//color position VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec4) * object.color.size(), &object.color[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	//color position VAO
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, NULL);
    glBindVertexArray(0);

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
/*
	glm::mat4 model = glm::rotate(model, 45.0f, glm::vec3(1.0f, 0.0f, 0.0f));

	glm::mat4 view = glm::lookAt(glm::vec3(4, 3, -3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	glm::mat4 proj = glm::perspective(45.0f, 800.0f / 600.0f, 0.1f, 100.0f);

	glm::mat4 MVP = model;*/
    /** Draws code goes here **/
    // Use the shader
    shader->use();/*
	shader->setMat4("MVP", MVP);*/
    // Binds the vertex array to be drawn
    glBindVertexArray(VAO[0]);
    // Renders the triangle gemotry
    glDrawArrays(GL_TRIANGLES, 0, 3);
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
	glDeleteBuffers(1, VBO);
    // Destroy the shader
    delete shader;
	delete Interface;

	TwTerminate();
    // Stops the glfw program
    glfwTerminate();

    return 0;
}
