#ifndef SCENE_H
#define SCENE_H

#include "Resources\ResourceManager.h"
#include "SceneManager.h"

extern int width, height;
extern int s_windowWidth, s_windowHeight;

class CScene
{
public:
	CScene(char *sceneFileLocation, CResourceManager *resourceManager);
	CScene(CSceneManager *sceneManager, CResourceManager *resourceManager);
	~CScene();

	void update(float deltaTime);
	void draw();

	CObject* getObjects();
	CTexture* getTextures();
	CShader* getShaders();

private:
	void init();


	CResourceManager *m_resourceManager;
	CSceneManager *m_sceneManager;

	float m_totalTime;
	GLuint m_skyboxTextureId;
	ESMatrix m_projectionMatrix, m_viewProjectionMatrix;


};



#endif 