#ifndef MODEL_H
#define MODEL_H

#include "../Common/Include/esUtil.h"
#include "../Common/Vector3D.h"
#include "cstdio"
#include <stdlib.h>
#include <string.h>


typedef struct
{
	Vector3D position;
	Vector3D texcoord;
	Vector3D normal;
	int index;

} vertex;

class CModel
{

public:
	CModel();
	CModel(char* objectFileLocation);
	CModel(char* name, char* objectFileLocation);

	void load(char* objectFileLocation);

	GLfloat* getPositionPointer();
	GLfloat* getTexcoordPointer();
	GLuint* getIndicesPointer();
	GLfloat* getNormalPointer();
	GLfloat* getTangentPointer();
	GLfloat* getBitangentPointer();
	GLfloat* getVerticesPointer();
	int getNumberOfIndices();
	char* getName();
	int getType();

	void setPositionPointer(GLfloat *position);
	void setTexcoordPointer(GLfloat *texcoords);
	void setNormalPointer(GLfloat *normal);
	void setIndicesPointer(GLuint *index);
	void setNumberOfIndices(int numberOfIndices);

private:
	void init();
	void loadFirstPass(FILE*); // Get number of verts/texcoords/etc.
	void loadSecondPass(FILE*); // Load vert/texcoords/etc. values
	void generateVertexArray();
	void loadDAT(char* datFileLocation);

	char* m_fileLocation;
	int m_numberOfVertices, m_numberOfTexcoords, m_numberOfNormals;
	int m_numberOfIndices;
	GLfloat *m_position, *m_texcoords, *m_normals, *m_tangents, *m_bitangents; // Final arrays
	GLfloat *m_vertices;
	GLuint *m_index;
	char* m_name;
	int m_type; // 0 - .OBJ; 1 - .DAT
	int m_normalMapping;
	
	// Will be deleted after object load
	int m_temp_numberOfVertices, m_temp_numberOfTexcoords, m_temp_numberOfNormals, m_temp_numberOfIndices;
	GLfloat *m_temp_position, *m_temp_texcoords, *m_temp_normals; // Values loaded from .obj
	GLuint	*m_temp_posindex, *m_temp_texindex, *m_temp_normindex; // will be discarded after loading

};
#endif