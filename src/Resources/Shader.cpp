#include "Shader.h"

CShader::CShader(char *vertexShaderLocation, char *fragmentShaderLocation)
{
	m_program = load(vertexShaderLocation, fragmentShaderLocation);
}

CShader::CShader(char *name, char *vertexShaderLocation, char *fragmentShaderLocation)
{
	m_name = (char*)malloc(sizeof(name));
	strcpy(m_name, name);

	m_program = load(vertexShaderLocation, fragmentShaderLocation);
}

GLint CShader::load(char *vertFileLoc, char *fragFileLoc)
{
	std::string myVert, myFrag;
	
	char buf[256];
	FILE *fin = fopen(vertFileLoc, "r");
	if(fin == NULL) 
	{
		printf("ERROR: reading shader %s", vertFileLoc);
		exit(1);
	}
	while(fgets(buf, 256, fin) != NULL)
		myVert.append(buf);
	fclose(fin);
	

	fin = fopen(fragFileLoc, "r");
	if(fin == NULL) 
	{
		printf("ERROR: reading shader %s", fragFileLoc);
		exit(1);
	}
	while(fgets(buf, 256, fin) != NULL)
		myFrag.append(buf);
	fclose(fin);

	GLint shaderProgram = esLoadProgram(myVert.c_str(), myFrag.c_str());

	return shaderProgram;
}

GLint CShader::getProgram()
{
	return m_program;
}

char* CShader::getName()
{
	return m_name;
}

void CShader::addUniform(char* uniformName, int length, string values)
{
	//printf("%s %i %s\n", uniformName, length, values.c_str());

	uniform newUniform;
	newUniform.length = length;
	newUniform.values = (float*)malloc(length * sizeof(float));
	newUniform.uniformName = (char*)malloc(sizeof(uniformName) * sizeof(char));
	strcpy(newUniform.uniformName, uniformName);
	
	// kinda messy, but it does the job
	switch(length)
	{
		case 1:
		{
			sscanf(values.c_str(),"%f", &newUniform.values[0]);
		}
		break;

		case 2:
		{
			sscanf(values.c_str(),"%f %f", &newUniform.values[0], &newUniform.values[1]);
		}
		break;

		case 3:
		{
			sscanf(values.c_str(),"%f %f %f", &newUniform.values[0], &newUniform.values[1], 
												&newUniform.values[2]);
		}
		break;

		case 4:
		{
			sscanf(values.c_str(),"%f %f %f %f", &newUniform.values[0], &newUniform.values[1], 
												&newUniform.values[2], &newUniform.values[3]);
		}
		break;
	}

	m_uniforms.push_back(newUniform);
}

uniform CShader::getUniform(int i)
{
	return m_uniforms[i];
}

int CShader::getNumberOfUniforms()
{
	return m_uniforms.size();
}

void CShader::addState(char *state)
{
	if(!strcmp(state, "GL_CULL_FACE"))
	{
		m_states.push_back(GL_CULL_FACE);
	}
	else if(!strcmp(state, "GL_DEPTH_TEST"))
	{
		m_states.push_back(GL_DEPTH_TEST);
	}
	else if(!strcmp(state, "GL_BLEND"))
	{
		m_states.push_back(GL_BLEND);
	}
}

int CShader::getNumberOfStates()
{
	return m_states.size();
}

int CShader::getState(int i)
{
	return m_states[i];
}