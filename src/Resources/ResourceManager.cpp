#include "ResourceManager.h"


CResourceManager::CResourceManager()
{
	init();
}

void CResourceManager::init()
{

}

void CResourceManager::loadResources(char *resourcesFileLocation)
{
	char buf[256], name[64], path[128];
	
	esLogMessage("[RESOURCES] Loading <%s>", resourcesFileLocation);
	
	FILE *fin = fopen(resourcesFileLocation, "r");
	if(fin == NULL) 
	{
		esLogMessage("ERROR: loading rm %s", resourcesFileLocation);
		exit(1);
	}
	

	while(fscanf(fin, "%s", buf) != EOF)
	{
		if(!strcmp(buf, "MODEL"))
		{
			fscanf(fin, "%s\n", name);
			fscanf(fin, "FILE %s\n", path);

			char* _filePath = (char*)malloc(128 * sizeof(char));
			_filePath[0] = '\0';
#ifdef _WIN32
			strcat(_filePath, "..");
#endif

#ifdef OS_ANDROID
			strcat(_filePath, SD_FOLDER);
#endif
			strcat(_filePath, path);

			//LOGI("loading %s", _filePath);

			//esLogMessage("loading model: <%s>", _filePath);

			CModel *newModel = new CModel(name, _filePath);
			models.insert(pair<char*, CModel*>(newModel->getName(), newModel));

			esLogMessage("[RESOURCES] loaded model <%s>", name);
		
		}
		else if(!strcmp(buf, "TEXTURE"))
		{
			char type[5];

			fscanf(fin, "%s", type);
			fscanf(fin, "%s\n", name);
			if(!strcmp(type, "2D"))
			{	
				fscanf(fin, "FILE %s\n", path);

				char* _filePath = (char*)malloc(128 * sizeof(char));
				_filePath[0] = '\0';
#ifdef _WIN32
				strcat(_filePath, "..");
#endif

#ifdef OS_ANDROID
				strcat(_filePath, SD_FOLDER);
#endif
				strcat(_filePath, path);
				

				CTexture *newTexture = new C2DTexture(name, _filePath);

				textures.insert(pair<char*, CTexture*>(newTexture->getName(), newTexture));

				
			}
			else if(!strcmp(type, "CUBE"))
			{
				char up[128], down[128], left[128], right[128], front[128], back[128];

				for(int i = 0; i < 6; i++)
				{
					fscanf(fin, "%s %s\n", buf, path);

					char* _filePath = (char*)malloc(128 * sizeof(char));
					_filePath[0] = '\0';
#ifdef _WIN32
					strcat(_filePath, "..");
#endif

#ifdef OS_ANDROID
					strcat(_filePath, SD_FOLDER);
#endif
					strcat(_filePath, path);
					if(!strcmp(buf, "UP"))
						strcpy(up, _filePath);
					else if(!strcmp(buf, "DOWN"))
						strcpy(down, _filePath);
					else if(!strcmp(buf, "LEFT"))
						strcpy(left, _filePath);
					else if(!strcmp(buf, "RIGHT"))
						strcpy(right, _filePath);
					else if(!strcmp(buf, "FRONT"))
						strcpy(front, _filePath);
					else if(!strcmp(buf, "BACK"))
						strcpy(back, _filePath);
				}
				

				CTexture *newTexture = new CCubeTexture(name, up, down, left, right, front, back);
				textures.insert(pair<char*, CTexture*>(newTexture->getName(), newTexture));
			}
			else
			{
				printf("ERROR Invalid texture type \"%s\"\n", type);
				exit(1);
			}

			esLogMessage("[RESOURCES] loaded texture <%s>", name);
		}
		else if(!strcmp(buf, "SHADER"))
		{
			char vert[128], frag[128], buf2[128];
			int uniformSize = 0; char uniformName[32], temp[32];
		

			fscanf(fin, "%s\n", name);
			fscanf(fin, "V %s\n", vert);
			fscanf(fin, "F %s\n", frag);

			char* _vPath = (char*)malloc(128 * sizeof(char));
			_vPath[0] = '\0';
			char* _fPath = (char*)malloc(128 * sizeof(char));
			_fPath[0] = '\0';

#ifdef _WIN32
				strcat(_vPath, "..");
				strcat(_fPath, "..");
#endif

#ifdef OS_ANDROID
				strcat(_vPath, SD_FOLDER);
				strcat(_fPath, SD_FOLDER);

#endif
				strcat(_vPath, vert);
				strcat(_fPath, frag);
			
			CShader *newShader = new CShader(name, _vPath, _fPath);
			
			// Read uniforms
			while((fscanf(fin, "%s", buf2) != EOF) && (!strcmp(buf2, "UNIFORM")))
			{
				fscanf(fin, "%s %i", uniformName, &uniformSize);
				string uniformValues;
				for(int i = 0; i < uniformSize; i++)
				{
					fscanf(fin, "%s", temp);
					uniformValues.append(temp);
					uniformValues.append(" ");
				}
				
				newShader->addUniform(uniformName, uniformSize, uniformValues);
			}
			// Go back one word
			if(!feof(fin))
			{
				int size = 0;
				while(buf2[size-1] != '\0')
					size++;
				fseek(fin, -size, SEEK_CUR);
			}
			
			// Read states
			while((fscanf(fin, "%s", buf2) != EOF) && (!strcmp(buf2, "STATE")))
			{
				char stateName[32];
				fscanf(fin, "%s\n", stateName);
				
				newShader->addState(stateName);
			}
			// Go back one word
			if(!feof(fin))
			{
				int size = 0;
				while(buf2[size-1] != '\0')
					size++;
				fseek(fin, -size, SEEK_CUR);
			}

			shaders.insert(pair<char*, CShader*>(newShader->getName(), newShader));

			esLogMessage("[RESOURCES] loaded shader <%s>", name);
		}
		else
		{
			esLogMessage("ERROR Invalid resource <%s>", buf);
			exit(1);
		}

	}
	esLogMessage("[RESOURCES] loading done");
}

CModel* CResourceManager::getModel(char* name)
{
	return models[name];
}

CTexture* CResourceManager::getTexture(char* name)
{
	return textures[name];
}

CShader* CResourceManager::getShader(char* name)
{
	return shaders[name];
}