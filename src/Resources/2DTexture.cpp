#include "2DTexture.h"

C2DTexture::C2DTexture(char *name, char *textureFileLocation) : CTexture(name)
{
	load(textureFileLocation);
}

void C2DTexture::load(char *textureFileLocation)
{
	int width, height, colorDepth;
	char *buffer = esLoadTGA ( textureFileLocation, &width, &height, &colorDepth );

	GLuint texId;

	if ( buffer == NULL )
	{
		//esLogMessage ( "Error loading (%s) image.\n", textureFileLocation );
		exit(1);
	}

	glGenTextures ( 1, &texId );
	glBindTexture ( GL_TEXTURE_2D, texId );
	
	if(colorDepth == 24)
		glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, buffer );
	else if(colorDepth == 32)
		glTexImage2D ( GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, buffer );
	
	glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );

	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri ( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );


	free ( buffer );

	m_ID = texId;
}

GLuint C2DTexture::getType()
{
	return GL_TEXTURE_2D;
}