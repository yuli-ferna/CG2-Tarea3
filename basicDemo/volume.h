#pragma once
#include <glad/glad.h> // Glad has to be include before glfw
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>

class volume
{
public:
	volume(const char* fileName);
	~volume();
	bool LoadVolumeFromFile(const char* fileName);
	int XDIM = 256, YDIM = 256, ZDIM = 256;
	int size;
	unsigned int textureID;
	void render();
private:

};
