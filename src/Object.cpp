#include "Object.h"
//#include "main.h"

CObject::CObject()
{
	init();
}

CObject::CObject(char *name)
{
	m_name = (char*)malloc(sizeof(name));
	strcpy(m_name, name);
	
	esLogMessage("## CObject <%s> init", name);
	init();
}

void CObject::init()
{
	m_position = Vector3D(0, 0, 0);
	m_rotation = Vector3D(0, 0, 0);
	m_scale = Vector3D(1, 1, 1);
}

CModel* CObject::getModel()
{
	return m_model;
}

void CObject::translate(float x, float y, float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
}

void CObject::rotate(float x, float y, float z)
{
	if(x >= 360.0f)
		x -= 360.0f;
	if(y >= 360.0f)
		y -= 360.0f;
	if(z >= 360.0f)
		z -= 360.0f;
	
	m_rotation.x = x;
	m_rotation.y = y;
	m_rotation.z = z;
}

void CObject::scale(float x, float y, float z)
{
	m_scale.x = x;
	m_scale.y = y;
	m_scale.z = z;
}

Vector3D& CObject::getPosition()
{
	return m_position;
}

Vector3D& CObject::getRotation()
{
	return m_rotation;
}

Vector3D& CObject::getScale()
{
	return m_scale;
}

void CObject::setModel(CModel *model)
{
	m_model = model;
}

void CObject::setShader(CShader *shader)
{
	m_shader = shader;
	setShaderProgram();
}

void CObject::setShaderProgram()
{
	int shaderProgram = getShader()->getProgram();

	m_a_positionLocation = glGetAttribLocation(shaderProgram, "a_position");
	m_a_texcoordLocation = glGetAttribLocation(shaderProgram, "a_texture");
	m_a_normalLocation = glGetAttribLocation(shaderProgram, "a_normal");
	m_a_tangentLocation = glGetAttribLocation(shaderProgram, "a_tangent");
	m_a_bitangentLocation = glGetAttribLocation(shaderProgram, "a_bitangent");

	m_u_textureLocation = glGetUniformLocation(shaderProgram, "u_texIMG");
	m_u_wvpMatrixLocation = glGetUniformLocation(shaderProgram, "u_mvpMatrix");
	m_u_wMatrixLocation = glGetUniformLocation(shaderProgram, "u_wMatrix");
	m_u_wMatrixInverseLocation = glGetUniformLocation(shaderProgram, "u_wMatrixInverse");
	m_u_cameraPositionLocation = glGetUniformLocation(shaderProgram, "u_cameraPosition");
	m_u_timeLocation = glGetUniformLocation(shaderProgram, "u_time");
}

CShader* CObject::getShader()
{
	return m_shader;
}

vector<textureStruct> CObject::getTextures()
{
	return m_textures;
}

ESMatrix CObject::getWVPMatrix()
{
	return m_wvpMatrix;
}

ESMatrix CObject::getWMatrix()
{
	return m_wMatrix;
}

void CObject::update(ESMatrix &viewProjectionMatrix, float deltaTime)
{
	ESMatrix viewProjection;

	esMatrixLoadIdentity(&m_wMatrix);
	esTranslate(&m_wMatrix, getPosition().x, getPosition().y, getPosition().z);
	esRotate(&m_wMatrix, getRotation().x, 1.0, 0.0, 0.0);
	esRotate(&m_wMatrix, getRotation().y, 0.0, 1.0, 0.0);
	esRotate(&m_wMatrix, getRotation().z, 0.0, 0.0, 1.0);
	esScale(&m_wMatrix, getScale().x, getScale().y, getScale().z);

	viewProjection = viewProjectionMatrix;

	esMatrixLoadIdentity(&m_wvpMatrix);
	esMatrixMultiply(&m_wvpMatrix, &m_wMatrix, &viewProjection);
	
}

void checkGlError(const char* op) {
    for (GLint error = glGetError(); error; error
            = glGetError()) {
		esLogMessage("after %s() glError (0x%x)\n", op, error);
    }
}

void CObject::draw()
{
	//esLogMessage("[CObject->draw()] drawing %s", getName());
	
	glEnable(GL_DEPTH_TEST);

	for(int i = 0; i < getShader()->getNumberOfStates(); i++)
		glEnable(getShader()->getState(i));
	//esLogMessage("[CObject->draw()] getState()");
	
	glUseProgram(getShader()->getProgram());
	checkGlError("glUseProgram");
	//esLogMessage("[CObject->draw()] getProgram()");
	
	if(getModel()->getType() == 0)
	{
		glVertexAttribPointer(m_a_positionLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), getModel()->getPositionPointer());
		checkGlError("glVertexAttribPointer 1");
		glVertexAttribPointer(m_a_texcoordLocation, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), getModel()->getTexcoordPointer());
		//checkGlError("glVertexAttribPointer 2");
		glGetError(); //ignore error

		glVertexAttribPointer(m_a_normalLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), getModel()->getNormalPointer());
		//checkGlError("glVertexAttribPointer 3");
		glGetError(); //ignore error

		glVertexAttribPointer(m_a_tangentLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), getModel()->getTangentPointer());
		//checkGlError("glVertexAttribPointer 4");
		glGetError(); //ignore error

		glVertexAttribPointer(m_a_bitangentLocation, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), getModel()->getBitangentPointer());
		//checkGlError("glVertexAttribPointer 5");
		glGetError(); //ignore error

		glEnableVertexAttribArray(m_a_positionLocation);
		checkGlError("glVertexAttribArray 1");
		glEnableVertexAttribArray(m_a_texcoordLocation);
		//checkGlError("glVertexAttribArray 2");
		glGetError(); //ignore error

		glEnableVertexAttribArray(m_a_normalLocation);
		//checkGlError("glVertexAttribArray 3");
		glGetError(); //ignore error

		glEnableVertexAttribArray(m_a_tangentLocation);
		//checkGlError("glVertexAttribArray 4");
		glGetError(); //ignore error

		glEnableVertexAttribArray(m_a_bitangentLocation);
		//checkGlError("glVertexAttribArray 5");
		glGetError(); //ignore error

	}
	else if(getModel()->getType() == 1)
	{
		glVertexAttribPointer(m_a_positionLocation, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), getModel()->getVerticesPointer());
		checkGlError("glVertexAttribPointer 1");
		glVertexAttribPointer(m_a_texcoordLocation, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), getModel()->getVerticesPointer() + 3);
		checkGlError("glVertexAttribPointer 2");
		glVertexAttribPointer(m_a_normalLocation, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), getModel()->getVerticesPointer() + 5);
		checkGlError("glVertexAttribPointer 3");
		
		glEnableVertexAttribArray(m_a_positionLocation);
		checkGlError("glVertexAttribArray 1");
		glEnableVertexAttribArray(m_a_texcoordLocation);
		checkGlError("glVertexAttribArray 2");
		glEnableVertexAttribArray(m_a_normalLocation);
		checkGlError("glVertexAttribArray 3");
	}
	//esLogMessage("[CObject->draw()] set pointers");

	for(unsigned int i = 0; i < m_textures.size(); i++)
	{
		CTexture* crtTexture = m_textures[i].texture;
		char* crtUniformName = m_textures[i].uniformName;
		int u_textureLocation = glGetUniformLocation(getShader()->getProgram(), crtUniformName);
		checkGlError("glGetUniformLocation texture");

		glActiveTexture(GL_TEXTURE0 + i);
		checkGlError("glActiveTexture");
		glBindTexture(crtTexture->getType(), crtTexture->getID());
		checkGlError("glBindTexture");
		glUniform1i(u_textureLocation, i);
		checkGlError("glUniform1i texture");
	}
	//esLogMessage("[CObject->draw()] bindTextures");

	glUniformMatrix4fv(m_u_wvpMatrixLocation, 1, GL_FALSE, (GLfloat*) &m_wvpMatrix.m[0][0]);
	checkGlError("glUniform wvp");
	//esLogMessage("[CObject->draw()] set wvpMatrix");

	glUniformMatrix4fv(m_u_wMatrixLocation, 1, GL_FALSE, (GLfloat*) &m_wMatrix.m[0][0]);
	checkGlError("glUniform w");
	//esLogMessage("[CObject->draw()] set wMatrix");

	glUniformMatrix4fv(m_u_wMatrixInverseLocation, 1, GL_FALSE, (GLfloat*) &getWMatrixInverse().m[0][0]);
	checkGlError("glUniform w inverse");
	//esLogMessage("[CObject->draw()] set wMatrixInverse");

	glUniform4f(m_u_cameraPositionLocation, camera->getPosition().x, camera->getPosition().y, camera->getPosition().z, 1.0);
	checkGlError("glUniform cameraPosition");
	//esLogMessage("[CObject->draw()] set camera");

	glUniform1f(m_u_timeLocation, totalTime);
	//esLogMessage("[CObject->draw()] set totalTime");

	//esLogMessage("[CObject->draw()] set uniforms");
	
	for(int i = 0; i < getShader()->getNumberOfUniforms(); i++)
	{
		uniform crtUniform = getShader()->getUniform(i);
		int crtUniformLocation = glGetUniformLocation(getShader()->getProgram(), crtUniform.uniformName);
		checkGlError("glUniformLocation shader");
		
		switch(crtUniform.length)
		{
			case 1:
			{
				glUniform1fv(crtUniformLocation, 1, crtUniform.values);
			}
			break;

			case 2:
			{
				glUniform2fv(crtUniformLocation, 1, crtUniform.values);
			}
			break;

			case 3:
			{
				glUniform3fv(crtUniformLocation, 1, crtUniform.values);
			}
			break;

			case 4:
			{
				glUniform4fv(crtUniformLocation, 1, crtUniform.values);
			}
			break;
		}

		checkGlError("glUniform shader");
	}
	//esLogMessage("[CObject->draw()] set custom uniforms");

	glDrawElements ( GL_TRIANGLES, 3 * getModel()->getNumberOfIndices(), GL_UNSIGNED_INT, getModel()->getIndicesPointer());
	checkGlError("glDrawElements");
	//esLogMessage("[CObject->draw()] drawElements()");

	for(int i = 0; i < getShader()->getNumberOfStates(); i++)
		glDisable(getShader()->getState(i));
	//esLogMessage("[CObject->draw()] disable getState()");
}

void CObject::setWVPMatrix(ESMatrix &wvpMatrix)
{
	m_wvpMatrix = wvpMatrix;
}

ESMatrix CObject::getWMatrixInverse()
{
	ESMatrix w_inv;

	esMatrixLoadIdentity(&w_inv);
	
	esRotate(&w_inv, -getRotation().x, 1.0, 0.0, 0.0);
	esRotate(&w_inv, -getRotation().y, 0.0, 1.0, 0.0);
	esRotate(&w_inv, -getRotation().z, 0.0, 0.0, 1.0);
	
	Vector3D pos = getPosition();

	esTranslate(&w_inv, -pos.x, -pos.y, -pos.z);


	return w_inv;
}

char* CObject::getName()
{
	return m_name;
}

void CObject::addTexture(CTexture *texture, char *uniformName)
{
	textureStruct newTextureStruct;
	newTextureStruct.texture = texture;
	newTextureStruct.uniformName = (char*)malloc(sizeof(uniformName) * sizeof(char));
	strcpy(newTextureStruct.uniformName, uniformName);

	m_textures.push_back(newTextureStruct);
}