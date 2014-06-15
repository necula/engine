#include "CubeTexture.h"


CCubeTexture::CCubeTexture(char *name, char *up, char *down, char *left, char *right, char *front, char *back) : CTexture(name)
{
	load(up, down, left, right, front, back);
}

void CCubeTexture::load(char *up, char *down, char *left, char *right, char *front, char *back)
{
	int width, height, colorDepth;
	char **buffer = (char**)malloc(6 * sizeof(char*));
	GLuint textureId = 0;

	buffer[0] = esLoadTGA(right, &width, &height, &colorDepth);
	buffer[1] = esLoadTGA(left, &width, &height, &colorDepth);
	buffer[2] = esLoadTGA(up, &width, &height, &colorDepth);
	buffer[3] = esLoadTGA(front, &width, &height, &colorDepth);
	buffer[4] = esLoadTGA(back, &width, &height, &colorDepth);
	buffer[5] = esLoadTGA(down, &width, &height, &colorDepth);


	if ( buffer == NULL )
	{
		esLogMessage ( "Error loading (%s) image.\n", "top");
		exit(1);
	}

	glGenTextures ( 1, &textureId );

	glBindTexture ( GL_TEXTURE_CUBE_MAP, textureId );

	if(colorDepth == 24)
	{

		glTexImage2D ( GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, width, height, 0, 
					  GL_RGB, GL_UNSIGNED_BYTE, buffer[1] );

		glTexImage2D ( GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, width, height, 0, 
					  GL_RGB, GL_UNSIGNED_BYTE, buffer[0] );

		glTexImage2D ( GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, width, height, 0, 
					  GL_RGB, GL_UNSIGNED_BYTE, buffer[2] );

		glTexImage2D ( GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, width, height, 0, 
					  GL_RGB, GL_UNSIGNED_BYTE, buffer[5] );

		glTexImage2D ( GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, width, height, 0, 
					  GL_RGB, GL_UNSIGNED_BYTE, buffer[3] );

		glTexImage2D ( GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, width, height, 0, 
					  GL_RGB, GL_UNSIGNED_BYTE, buffer[4] );

	}
	else if(colorDepth == 32)
	{
		glTexImage2D ( GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGBA, width, height, 0, 
					  GL_RGB, GL_UNSIGNED_BYTE, buffer[1] );

		glTexImage2D ( GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGBA, width, height, 0, 
					  GL_RGB, GL_UNSIGNED_BYTE, buffer[0] );

		glTexImage2D ( GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGBA, width, height, 0, 
					  GL_RGB, GL_UNSIGNED_BYTE, buffer[2] );

		glTexImage2D ( GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGBA, width, height, 0, 
					  GL_RGB, GL_UNSIGNED_BYTE, buffer[5] );

		glTexImage2D ( GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGBA, width, height, 0, 
					  GL_RGB, GL_UNSIGNED_BYTE, buffer[3] );

		glTexImage2D ( GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGBA, width, height, 0, 
					  GL_RGB, GL_UNSIGNED_BYTE, buffer[4] );

	}
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	glTexParameteri ( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri ( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri ( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
	glTexParameteri ( GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );


	//free ( buffer );

	m_ID = textureId;
}

GLuint CCubeTexture::getType()
{
	return GL_TEXTURE_CUBE_MAP;
}