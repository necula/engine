#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include "Model.h"
#include "Shader.h"
#include "Texture.h"
#include "2DTexture.h"
#include "CubeTexture.h"
#include "..\Object.h"
#include <map>
#include <iostream>
#include <string>

using namespace std;

class CResourceManager
{
public:
	CResourceManager();
	
	void loadResources(char *resourcesFileLocation);

	CModel* getModel(char* name);
	CTexture* getTexture(char* name);
	CShader* getShader(char* name);

private:
	void init();

	struct my_cmp
	{
		bool operator()(const char *a, const  char *b) const
		{
			 return strcmp(a, b) < 0;
		}
	};

	map<char*, CTexture*, my_cmp> textures;
	map<char*, CShader*, my_cmp> shaders;
	map<char*, CModel*, my_cmp> models;
	
};

#endif