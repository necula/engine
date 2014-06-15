#ifndef _CUBE_TEXTURE_H
#define _CUBE_TEXTURE_H

#include "Texture.h"

class CCubeTexture : public CTexture
{
public:
	CCubeTexture();
	CCubeTexture(char *name, 
				char *up, char *down, 
				char *left, char *right, 
				char *front, char *back);

	GLuint getType();

private:
	void load(char *up, char *down, 
			char *left, char *right, 
			char *front, char *back);

};
#endif