#ifndef _POST_PROCESS_H
#define _POST_PROCESS_H

#include "Common/Include/esUtil.h"
#include "Resources\ResourceManager.h"

extern CResourceManager *rm;
extern int s_windowWidth, s_windowHeight;

class CPostprocess
{
public:
	CPostprocess();

	void activateFilter(int id);
	void renderToTexture();
	void renderFilter();
	
	int isActive();

private:
	int m_activeFilter;

	GLuint m_colorTextureA, m_colorTextureB, m_depthTexture;
	GLuint m_fbo;
	GLuint m_renderBuffer;

};
#endif