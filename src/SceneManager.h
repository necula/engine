#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include "Resources\ResourceManager.h"
#include <vector>

using namespace std;


class CSceneManager
{
public:
	CSceneManager();
	~CSceneManager();

	void loadScene(char *sceneFileLocation);
	
	vector<CObject*> getObjects();
	int getNumberOfObjects();

	void setResourceManager(CResourceManager *resourceManager);

private:
	
	int m_numberOfObjects;
	//CObject **m_objects;
	CResourceManager *m_resourceManager;

	vector<CObject*> m_objects;
};



#endif