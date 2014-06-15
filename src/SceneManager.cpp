#include "SceneManager.h"


CSceneManager::CSceneManager() : m_numberOfObjects(0)
{

}

CSceneManager::~CSceneManager()
{
	/*
	for(int i = 0; i < m_numberOfObjects; i++)
	{
		free(m_objects[i]);
	}

	free(m_objects);
	*/

}

void CSceneManager::loadScene(char *sceneFileLocation)
{
	char buf[256], name[64], uniformName[64];
	float x, y, z;
	CObject *newObject = NULL;

	FILE *fin = fopen(sceneFileLocation, "r");
	if(fin == NULL) 
	{
		esLogMessage("ERROR: reading sm %s", sceneFileLocation);
		exit(1);
	}

	while(fscanf(fin, "%s", buf) != EOF)
	{
		if(!strcmp(buf, "OBJECT"))
		{
			if(newObject != NULL)
				m_objects.push_back(newObject);

			fscanf(fin, "%s\n", name);
			newObject = new CObject(name);
		}
		else if(!strcmp(buf, "MODEL"))
		{
			fscanf(fin, "%s\n", name);
			newObject->setModel(m_resourceManager->getModel(name));	
		}
		else if(!strcmp(buf, "TEXTURE"))
		{
			fscanf(fin, "%s %s\n", name, uniformName);
			newObject->addTexture(m_resourceManager->getTexture(name), uniformName);
		}
		else if(!strcmp(buf, "SHADER"))
		{
			fscanf(fin, "%s\n", name);
			newObject->setShader(m_resourceManager->getShader(name));
		}
		else if(!strcmp(buf, "POS"))
		{
			fscanf(fin, "%f %f %f\n", &x, &y, &z);
			newObject->translate(x, y, z);
		}
		else if(!strcmp(buf, "ROT"))
		{
			fscanf(fin, "%f %f %f\n", &x, &y, &z);
			newObject->rotate(x, y, z);
		}
		else if(!strcmp(buf, "SCA"))
		{
			fscanf(fin, "%f %f %f\n", &x, &y, &z);
			newObject->scale(x, y, z);
		}
	}
	m_objects.push_back(newObject); // add last object

	//esLogMessage("[SCENE] loaded <%s>", newObject->getName());
}

vector<CObject*> CSceneManager::getObjects()
{
	return m_objects;
}

void CSceneManager::setResourceManager(CResourceManager *rm)
{
	m_resourceManager = rm;
}

int CSceneManager::getNumberOfObjects()
{
	return m_objects.size();
}
