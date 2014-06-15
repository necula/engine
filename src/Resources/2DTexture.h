#ifndef _2D_TEXTURE_H
#define _2D_TEXTURE_H

#include "Texture.h"

class C2DTexture : public CTexture
{
public:
	C2DTexture();
	C2DTexture(char *name, char *textureFileLocation);

	GLuint getType();

private:
	void load(char* textureFileLocation);

};
#endif