#ifndef TEXTURE_H
#define TEXTURE_H

#include "../Common/Include/esUtil.h"
#include "../Common/Vector3D.h"
#include <stdlib.h>
#include "cstdio"
#include <string.h>

class CTexture
{
public:
	CTexture(char* name);

	GLuint getID();
	char* getName();
	virtual GLuint getType();

private:
	void load(char* textureFileLocation);
	char *m_name;

protected:
	GLuint m_ID;
	
};
#endif