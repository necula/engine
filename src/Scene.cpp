#include "Scene.h"

CScene::CScene(char *sceneFileLocation, CResourceManager *resourceManager)
{
	m_resourceManager = resourceManager;
	
	m_sceneManager = new CSceneManager();
	m_sceneManager->setResourceManager(m_resourceManager);
	m_sceneManager->loadScene(sceneFileLocation);

	init();
}

CScene::CScene(CSceneManager *sceneManager, CResourceManager *resourceManager)
{
	m_resourceManager = resourceManager;
	m_sceneManager = sceneManager;

	init();
}

CScene::~CScene()
{
	delete m_sceneManager;
}

void CScene::init()
{
	m_totalTime = 0;

	//float aspect = 1600.0f / 900;
	esLogMessage("[CScene->init()] %i %i", s_windowWidth, s_windowHeight);
	float aspect = ((float)s_windowWidth) / s_windowHeight;
	esMatrixLoadIdentity(&m_projectionMatrix);
	esPerspective(&m_projectionMatrix, 50.0f, aspect, 0.2f, 2500.0f);
}


void CScene::update(float deltaTime)
{
	// cameraView * projection
	esMatrixLoadIdentity(&m_viewProjectionMatrix);
	esMatrixMultiply(&m_viewProjectionMatrix, camera->getViewMatrix(), &m_projectionMatrix);
	
	for(int i = 0; i < m_sceneManager->getNumberOfObjects(); i++)
	{
	   
		CObject *crtObject = m_sceneManager->getObjects()[i];
		char *shaderName = crtObject->getShader()->getName();
	
		if(!strncmp(shaderName, "PHONG", 5) || !strcmp(shaderName, "NORMALMAPPING") || !strcmp(shaderName, "OBJECT"))
			crtObject->rotate(0.0, crtObject->getRotation().y + deltaTime * 40.0f, 0.0);

		else if(!strcmp(crtObject->getName(), "UVDISP"))
		{
			float camRot = camera->getYRotation();

			crtObject->rotate(0.0, camRot, 0.0);
		}
		
		else if(!strcmp(crtObject->getName(), "SKYBOX"))
			crtObject->translate(camera->getPosition().x, camera->getPosition().y, camera->getPosition().z); 

		crtObject->update(m_viewProjectionMatrix, deltaTime);
	}
}

void CScene::draw()
{	
	glViewport(0, 0, s_windowWidth, s_windowHeight);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	
	for(int i = 0; i < m_sceneManager->getNumberOfObjects(); i++)
	{
		CObject *crtObject = m_sceneManager->getObjects()[i];
		crtObject->draw();
	}
}