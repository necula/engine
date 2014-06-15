#ifndef SHADER_H
#define SHADER_H

#include "../Common/Include/esUtil.h"
#include <stdlib.h>
#include <cstdio>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

typedef struct
{
	char* uniformName;
	int length;
	float* values;
} uniform;

class CShader
{
public:
	CShader(char *vertexShaderLocation, char *fragmentShaderLocation);
	CShader(char *name, char *vertexShaderLocation, char *fragmentShaderLocation);

	void addUniform(char* uniformName, int length, string values);
	void addState(char* state);

	GLint getProgram();
	uniform getUniform(int i);
	char* getName();
	int getNumberOfUniforms();
	int getNumberOfStates();
	int getState(int i);

private:
	GLint load(char*, char*);

	GLint m_program;
	char *m_name;
	vector<uniform> m_uniforms;
	vector<int> m_states;

};



#endif