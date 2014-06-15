#include "Model.h"


CModel::CModel()
{
	init();
}

CModel::CModel(char *objectFileLocation)
{
	init();
}

CModel::CModel(char* name, char* objectFileLocation)
{
	init();

	m_name = (char*)malloc(sizeof(name));
	strcpy(m_name, name);
	
	// a little bit of cheating
	if(!strcmp(name, "CUBE"))
	{
		m_normalMapping = 1;
		esLogMessage("[MODEL] normal mapping activated");
	}
	if(strstr(objectFileLocation, ".obj") != NULL)
		load(objectFileLocation);
	else if(strstr(objectFileLocation, ".dat") != NULL)
		loadDAT(objectFileLocation);
}

void CModel::init()
{
	m_numberOfIndices = 0;
	m_numberOfNormals = 0;
	m_numberOfTexcoords = 0;
	m_numberOfVertices = 0;

	m_temp_numberOfIndices = 0;
	m_temp_numberOfNormals = 0;
	m_temp_numberOfTexcoords = 0;
	m_temp_numberOfVertices = 0;

	m_type = -1;
	m_normalMapping = 0;
}


void CModel::load(char *fileLocation)
{
	FILE *f;
	m_type = 0;

	f = fopen(fileLocation, "r");
	if(f == NULL)
	{
		printf("ERROR OBJ FILE");
		exit(1);
	}

	loadFirstPass(f);

	m_temp_position = (GLfloat*)malloc(3 * m_temp_numberOfVertices * sizeof(GLfloat));
	m_temp_posindex = (GLuint*)malloc(3 * m_temp_numberOfIndices * sizeof(GLuint));
	m_temp_texcoords = (GLfloat*)malloc(2 * m_temp_numberOfTexcoords * sizeof(GLfloat));
	m_temp_texindex = (GLuint*)malloc(3 * m_temp_numberOfIndices * sizeof(GLuint));
	m_temp_normals = (GLfloat*)malloc(3 * m_temp_numberOfNormals * sizeof(GLfloat));
	m_temp_normindex = (GLuint*)malloc(3 * m_temp_numberOfIndices * sizeof(GLuint));


	m_numberOfIndices = m_temp_numberOfIndices;

	rewind(f);
	loadSecondPass(f);

	generateVertexArray();

	free(m_temp_position);
	free(m_temp_posindex);
	free(m_temp_texcoords);
	free(m_temp_texindex);
	free(m_temp_normals);
	free(m_temp_normindex);

	
}

void CModel::loadFirstPass(FILE *f)
{
	
	char buffer[256];
	int v, vt, vn;

	while(fscanf(f, "%s", buffer) != EOF)
	{
		switch(buffer[0])
		{
			
			case 'f' : 
			{
				fscanf(f, "%s", buffer);
				// v/vt/vn ONLY
				if(sscanf(buffer, "%d/%d/%d", &v, &vt, &vn) == 3) 
					m_temp_numberOfIndices++;
			}
			break;	

			case 'v' :
			{
				switch(buffer[1])
				{
					case '\0':
						m_temp_numberOfVertices++;
					break;

					case 't':
						m_temp_numberOfTexcoords++;
					break;

					case 'n':
						m_temp_numberOfNormals++;
					break;

					default:
					break;
				}
			}
			break;

			default:
			break;

		}
	}
	
}


void CModel::loadSecondPass(FILE *f)
{
	char buffer[256];
	GLint v, vt, vn;
	GLfloat v1, v2, v3;
	int iV = 0, iVT = 0, iVN = 0, iF = 0, iT = 0, iN = 0;

	while(fscanf(f, "%s", buffer) != EOF)
	{
		switch(buffer[0])
		{
			case 'f' :
			{
				
				fscanf(f, "%s", buffer);
				
				if(sscanf(buffer, "%d/%d/%d", &v, &vt, &vn) == 3) 
				{
					m_temp_posindex[iF++] = v - 1;
					m_temp_texindex[iT++] = vt - 1;
					m_temp_normindex[iN++] = vn - 1;

				
					while(fscanf(f, "%d/%d/%d", &v, &vt, &vn) == 3)
					{
						m_temp_posindex[iF++] = v - 1;
						m_temp_texindex[iT++] = vt - 1;
						m_temp_normindex[iN++] = vn - 1;

					}
				}
			}
			break;	

			case 'v' :
			{
				switch(buffer[1])
				{
					case '\0':
					{
						fscanf(f, "%f", &v1);
						fscanf(f, "%f", &v2);
						fscanf(f, "%f", &v3);
						
						m_temp_position[iV++] = v1;
						m_temp_position[iV++] = v2;
						m_temp_position[iV++] = v3;
					}
					break;

					case 't':
					{
						fscanf(f, "%f", &v1);
						fscanf(f, "%f", &v2);
						
						m_temp_texcoords[iVT++] = v1;
						m_temp_texcoords[iVT++] = v2;
					}
					break;

					case 'n':
						{
							fscanf(f, "%f", &v1);
							fscanf(f, "%f", &v2);
							fscanf(f, "%f", &v3);
							
							m_temp_normals[iVN++] = v1;
							m_temp_normals[iVN++] = v2;
							m_temp_normals[iVN++] = v3;
						}
					break;

					default:
					break;
				}
			}
			break;

			default:
			break;

		}
	}
}

void CModel::generateVertexArray()
{
	int alreadyKnownIndex = 0, crtIndex = 0, crtVertex = -1;
	vertex **vertexArray = new vertex*[3 * m_temp_numberOfIndices];
	

	m_index = (GLuint*)malloc(3 * m_temp_numberOfIndices * sizeof(GLuint));
	

	for(int i = 0; i < 3 * m_temp_numberOfIndices; i++)
	{
		vertex *newVertex = new vertex;

		newVertex->position.x = m_temp_position[m_temp_posindex[i]*3];
		newVertex->position.y = m_temp_position[m_temp_posindex[i]*3 + 1];
		newVertex->position.z = m_temp_position[m_temp_posindex[i]*3 + 2];

		newVertex->texcoord.x = m_temp_texcoords[m_temp_texindex[i]*2];
		newVertex->texcoord.y = m_temp_texcoords[m_temp_texindex[i]*2 + 1];

		newVertex->normal.x = m_temp_normals[m_temp_normindex[i]*3];
		newVertex->normal.y = m_temp_normals[m_temp_normindex[i]*3 + 1];
		newVertex->normal.z = m_temp_normals[m_temp_normindex[i]*3 + 2];

		bool exists = false;

		for(int j = 0; j <= crtVertex; j++)
		{
			if(newVertex->position == vertexArray[j]->position)
			{
				if(newVertex->texcoord == vertexArray[j]->texcoord) 
				{
						if(!m_normalMapping)
							exists = true;
						alreadyKnownIndex = j;
						break;
				}
			}
		}
		
		if(!exists)
		{
			crtVertex++;	
			vertexArray[crtVertex] = newVertex;

			m_index[crtIndex] = crtVertex;
			crtIndex++;
		}
		else
		{
			m_index[crtIndex] = alreadyKnownIndex;
			crtIndex++;
		}

	}
	
	/*
	printf("*** totalIndex=%i currentVertexNumber=%i\n", crtIndex, crtVertex);
	
	FILE *fout;
	
	fout = fopen("vertexcoord_model.txt","w");
	for(int i = 0; i <= crtVertex; i++)
	{
		vertex *curVertex = vertexArray[i];
		fprintf(fout, "%.4f %.4f %.4f  %.4f %.4f\n", curVertex->position.x, curVertex->position.y, curVertex->position.z,
															curVertex->texcoord.x, curVertex->texcoord.y);	
	}
	fclose(fout);

	fout = fopen("index_model.txt","w");
	for(int i = 0; i < crtIndex; i+=3)
	{
		fprintf(fout, "%d %d %d\n", m_index[i], m_index[i+1], m_index[i+2]);
	}
	fclose(fout);
	*/

	m_position = (GLfloat*)malloc(3 * (crtVertex+1) * sizeof(GLfloat));
	m_texcoords = (GLfloat*)malloc(2 * (crtVertex+1) * sizeof(GLfloat));
	m_normals = (GLfloat*)malloc(3 * (crtVertex+1) * sizeof(GLfloat));
	m_tangents = (GLfloat*)malloc(3 * (crtVertex+1) * sizeof(GLfloat));
	m_bitangents = (GLfloat*)malloc(3 * (crtVertex+1) * sizeof(GLfloat));
	

	for(int i = 0; i <= crtVertex; i++)
	{
		vertex *currentVertex = vertexArray[i];

		m_position[i*3]     = currentVertex->position.x;
		m_position[i*3 + 1] = currentVertex->position.y;
		m_position[i*3 + 2] = currentVertex->position.z;

		m_texcoords[i*2] =	currentVertex->texcoord.x;
		m_texcoords[i*2 + 1] = currentVertex->texcoord.y;
		
		m_normals[i*3] = currentVertex->normal.x;
		m_normals[i*3 + 1] = currentVertex->normal.y;
		m_normals[i*3 + 2] = currentVertex->normal.z;
	}
	
	
	Vector3D T, B, N;
	
	for(int i = 0; i < crtIndex; i += 3)
	{
		vertex *v1, *v2, *v3;

		v1 = vertexArray[m_index[i]];
		v2 = vertexArray[m_index[i + 1]];
		v3 = vertexArray[m_index[i + 2]];

		Vector3D v2v1 = v2->position - v1->position;
		Vector3D v3v1 = v3->position - v1->position;

		float c2c1_T = v2->texcoord.x - v1->texcoord.x;
		float c2c1_B = v2->texcoord.y - v1->texcoord.y;

		float c3c1_T = v3->texcoord.x - v1->texcoord.x;
		float c3c1_B = v3->texcoord.y - v1->texcoord.y;

		float fDenominator = c2c1_T * c3c1_B - c3c1_T * c2c1_B;

		if((fDenominator > -0.0001) && (fDenominator < 0.0001))
		{
			T = Vector3D(1.0f, 0.0f, 0.0f);
			B = Vector3D(0.0f, 1.0f, 0.0f);
			N = Vector3D(0.0f, 0.0f, 1.0f);
		}
		else
		{
			float fScale1 = 1.0f / fDenominator;

			 T = Vector3D((c3c1_B * v2v1.x - c2c1_B * v3v1.x) * fScale1,
						 (c3c1_B * v2v1.y - c2c1_B * v3v1.y) * fScale1,
                         (c3c1_B * v2v1.z - c2c1_B * v3v1.z) * fScale1);
 
             B = Vector3D((-c3c1_T * v2v1.x + c2c1_T * v3v1.x) * fScale1,
                         (-c3c1_T * v2v1.y + c2c1_T * v3v1.y) * fScale1,
                         (-c3c1_T * v2v1.z + c2c1_T * v3v1.z) * fScale1);

			 N = T.CrossProduct(B);
		}

		m_tangents[m_index[i] * 3]	   = T.x;
		m_tangents[m_index[i] * 3 + 1] = T.y;
		m_tangents[m_index[i] * 3 + 2] = T.z;

		m_tangents[m_index[i+1] * 3]	 = T.x;
		m_tangents[m_index[i+1] * 3 + 1] = T.y;
		m_tangents[m_index[i+1] * 3 + 2] = T.z;

		m_tangents[m_index[i+2] * 3]	 = T.x;
		m_tangents[m_index[i+2] * 3 + 1] = T.y;
		m_tangents[m_index[i+2] * 3 + 2] = T.z;


		m_bitangents[m_index[i] * 3]		= B.x;
		m_bitangents[m_index[i] * 3 + 1]	= B.y;
		m_bitangents[m_index[i] * 3 + 2]	= B.z;

		m_bitangents[m_index[i+1] * 3]		= B.x;
		m_bitangents[m_index[i+1] * 3 + 1]	= B.y;
		m_bitangents[m_index[i+1] * 3 + 2]	= B.z;

		m_bitangents[m_index[i+2] * 3]		= B.x;
		m_bitangents[m_index[i+2] * 3 + 1]	= B.y;
		m_bitangents[m_index[i+2] * 3 + 2]	= B.z;

	}
	
	
	
	/*
	fout = fopen("position_model.txt","w");
	for(int i = 0; i <= crtVertex; i++)
	{
		
		fprintf(fout, "%.4f %.4f %.4f \n", m_position[i*3], m_position[i*3 + 1], m_position[i*3 + 2]);
	}
	fclose(fout);

	fout = fopen("texcoord_model.txt","w");
	for(int i = 0; i <= crtVertex; i++)
	{
		fprintf(fout, "%.4f %.4f\n", m_texcoords[i*2], m_texcoords[i*2 + 1]);
	}
	fclose(fout);
	*/

	
}

GLfloat* CModel::getPositionPointer()
{
	return m_position;
}

GLfloat* CModel::getTexcoordPointer()
{
	return m_texcoords;
}

GLuint* CModel::getIndicesPointer()
{
	return m_index;
}

int CModel::getNumberOfIndices()
{
	return m_numberOfIndices;
}

GLfloat* CModel::getNormalPointer()
{
	return m_normals;
}

void CModel::setPositionPointer(GLfloat *position)
{
	m_position = position;
}

void CModel::setTexcoordPointer(GLfloat *texcoord)
{
	m_texcoords = texcoord;
}

void CModel::setNormalPointer(GLfloat *normal)
{
	m_normals = normal;
}

void CModel::setIndicesPointer(GLuint *index)
{
	m_index = index;
}

void CModel::setNumberOfIndices(int numberOfIndices)
{
	m_numberOfIndices = numberOfIndices;
}

GLfloat* CModel::getTangentPointer()
{
	return m_tangents;
}

GLfloat* CModel::getBitangentPointer()
{
	return m_bitangents;
}

char* CModel::getName()
{
	return m_name;
}

int CModel::getType()
{
	return m_type;
}

GLfloat* CModel::getVerticesPointer()
{
	return m_vertices;
}

void CModel::loadDAT(char *datFileLocation)
{
	int numverts, numfaces;

	m_type = 1;
	
	FILE *fin = fopen(datFileLocation, "r");
	if(fin == NULL) 
	{
		printf("ERROR: reading rm %s", datFileLocation);
		exit(1);
	}
	
	fscanf(fin, "%i %i\n", &numverts, &numfaces);
	m_vertices = (GLfloat*)malloc(numverts * 8 * sizeof(GLfloat));
	m_index = (GLuint*)malloc(numfaces * 3 * sizeof(GLuint));
	m_numberOfIndices = numfaces;
	m_numberOfVertices = numverts;
	
	for(int i = 0; i < numverts; i++)
	{
		fscanf(fin, "%f %f %f %f %f %f %f %f", &m_vertices[8*i + 0], &m_vertices[8*i + 2], &m_vertices[8*i + 1], // position
												&m_vertices[8*i + 3], &m_vertices[8*i + 4], // texcoords
												&m_vertices[8*i + 5], &m_vertices[8*i + 7], &m_vertices[8*i + 6]); // normal

		m_vertices[8*i + 2] *= -1;
		m_vertices[8*i + 7] *= -1;
	}
	fscanf(fin, "\n");

	for(int i = 0; i < numfaces; i++)
		fscanf(fin, "%i %i %i", &m_index[3*i + 0], &m_index[3*i + 1], &m_index[3*i + 2]);
	
}


