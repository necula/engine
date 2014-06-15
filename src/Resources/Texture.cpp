#include "Texture.h"

CTexture::CTexture(char *name)
{
	m_name = (char*)malloc(sizeof(name));
	strcpy(m_name, name);

	//load(name);
}

void CTexture::load(char *textureFileLocation)
{

}

GLuint CTexture::getID()
{
	return m_ID;
}

GLuint CTexture::getType()
{
	return -1;
}

char* CTexture::getName()
{
	return m_name;
}