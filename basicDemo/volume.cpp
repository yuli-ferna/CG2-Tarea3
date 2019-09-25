#define _CRT_SECURE_NO_DEPRECATE
#include "volume.h"

volume::volume(const char* fileName)
{
	XDIM = 256, YDIM = 256, ZDIM = 256;
	size = XDIM * YDIM * ZDIM;
	LoadVolumeFromFile(fileName);
}

volume::~volume()
{
}

bool volume::LoadVolumeFromFile(const char* fileName) 
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
	glGenTextures(1, &t);
	glBindTexture(GL_TEXTURE_3D, t);

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

void volume::render()
{

}
