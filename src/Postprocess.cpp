#include "Postprocess.h"

CPostprocess::CPostprocess()
{
	m_activeFilter = 0;

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &m_colorTextureA);
	glBindTexture(GL_TEXTURE_2D, m_colorTextureA);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, s_windowWidth, s_windowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &m_colorTextureB);
	glBindTexture(GL_TEXTURE_2D, m_colorTextureB);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, s_windowWidth, s_windowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &m_depthTexture);
	glBindTexture(GL_TEXTURE_2D, m_depthTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, s_windowWidth, s_windowHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, NULL);

	glGenFramebuffers(1, &m_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorTextureA, 0);

	glGenRenderbuffers(1, &m_renderBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, m_renderBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, s_windowWidth, s_windowHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_renderBuffer);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void CPostprocess::activateFilter(int id)
{	
	m_activeFilter = (m_activeFilter == id) ? (0) : (id);
}

int CPostprocess::isActive()
{
	return (m_activeFilter > 0);
}

void CPostprocess::renderToTexture()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorTextureA, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture, 0);

}

void CPostprocess::renderFilter()
{
	GLfloat quad_verts[] = { -1.0f,  1.0f, 0.0f,	// Position 0
								0.0f,  1.0f,        // TexCoord 0 0 0
							   -1.0f, -1.0f, 0.0f,  // Position 1 
								0.0f,  0.0f,        // TexCoord 1 0 1
								1.0f, -1.0f, 0.0f,  // Position 2
								1.0f,  0.0f,        // TexCoord 2 1 1
								1.0f,  1.0f, 0.0f,  // Position 3
								1.0f,  1.0f         // TexCoord 3 1 0
							 };
	GLushort quad_indices[] = { 0, 1, 2, 0, 2, 3 };
	
	if(m_activeFilter == 1)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		int shaderProg = rm->getShader("PPSIMPLE")->getProgram();
		glUseProgram(shaderProg);
		
		int posLoc = glGetAttribLocation(shaderProg, "a_position");
		int texcoordLoc = glGetAttribLocation(shaderProg, "a_texture");
		int texLoc = glGetUniformLocation(shaderProg, "u_texIMG");

		glVertexAttribPointer( posLoc, 3, GL_FLOAT, 
								GL_FALSE, 5 * sizeof(GLfloat), quad_verts );
		glVertexAttribPointer( texcoordLoc, 2, GL_FLOAT,
							   GL_FALSE, 5 * sizeof(GLfloat), &quad_verts[3]);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_colorTextureA);
		glUniform1i(texLoc, 0);

		glDrawElements (GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, quad_indices);
		
	}
	else if(m_activeFilter == 2)
	{
		
		// get highpass into texture
		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);		
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorTextureB, 0);

		int shaderProg = rm->getShader("PPHIGHPASS")->getProgram();
		glUseProgram(shaderProg);
		
		int posLoc = glGetAttribLocation(shaderProg, "a_position");
		int texcoordLoc = glGetAttribLocation(shaderProg, "a_texture");
		int texLoc = glGetUniformLocation(shaderProg, "u_texIMG");

		glVertexAttribPointer( posLoc, 3, GL_FLOAT, 
								GL_FALSE, 5 * sizeof(GLfloat), quad_verts );
		glVertexAttribPointer( texcoordLoc, 2, GL_FLOAT,
							   GL_FALSE, 5 * sizeof(GLfloat), &quad_verts[3]);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_colorTextureA);
		glUniform1i(texLoc, 0);

		glDrawElements (GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, quad_indices);
		
		
		// blur & bloom
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
		shaderProg = rm->getShader("PPBLOOM")->getProgram();
		glUseProgram(shaderProg);
		
		posLoc = glGetAttribLocation(shaderProg, "a_position");
		int highpassLoc = glGetUniformLocation(shaderProg, "u_highpass");
		texcoordLoc = glGetAttribLocation(shaderProg, "a_texture");

		glVertexAttribPointer( posLoc, 3, GL_FLOAT, 
								GL_FALSE, 5 * sizeof(GLfloat), quad_verts );
		glVertexAttribPointer( texcoordLoc, 2, GL_FLOAT,
							   GL_FALSE, 5 * sizeof(GLfloat), &quad_verts[3]);

		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D,m_colorTextureA);
		glUniform1i(texLoc, 0);
		
					
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_colorTextureB);
		glUniform1i(highpassLoc, 1);

		glDrawElements (GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, quad_indices);
	}
	else if(m_activeFilter == 3)
	{
		int shaderProg, posLoc, texcoordLoc, texLoc, depthBufferLoc, blurredIMG;

		glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorTextureB, 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, 0, 0);
		
		shaderProg = rm->getShader("PPBLUR")->getProgram();
		glUseProgram(shaderProg);
		
		posLoc = glGetAttribLocation(shaderProg, "a_position");
		texcoordLoc = glGetAttribLocation(shaderProg, "a_texture");
		texLoc = glGetUniformLocation(shaderProg, "u_texIMG");

		glVertexAttribPointer( posLoc, 3, GL_FLOAT, 
								GL_FALSE, 5 * sizeof(GLfloat), quad_verts );
		glVertexAttribPointer( texcoordLoc, 2, GL_FLOAT,
							   GL_FALSE, 5 * sizeof(GLfloat), &quad_verts[3]);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_colorTextureA);
		glUniform1i(texLoc, 0);

		glDrawElements (GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, quad_indices);

		
		
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		shaderProg = rm->getShader("PPDOF")->getProgram();
		glUseProgram(shaderProg);
		
		posLoc = glGetAttribLocation(shaderProg, "a_position");
		texcoordLoc = glGetAttribLocation(shaderProg, "a_texture");
		texLoc = glGetUniformLocation(shaderProg, "u_texIMG");
		depthBufferLoc = glGetUniformLocation(shaderProg, "u_depthBuffer");
		blurredIMG = glGetUniformLocation(shaderProg, "u_blurredIMG");

		glVertexAttribPointer( posLoc, 3, GL_FLOAT, 
								GL_FALSE, 5 * sizeof(GLfloat), quad_verts );
		glVertexAttribPointer( texcoordLoc, 2, GL_FLOAT,
							   GL_FALSE, 5 * sizeof(GLfloat), &quad_verts[3]);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_colorTextureA);
		glUniform1i(texLoc, 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_depthTexture);
		glUniform1i(depthBufferLoc, 1);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, m_colorTextureB);
		glUniform1i(blurredIMG, 2);

		glDrawElements (GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, quad_indices);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
		
	
}