#ifndef OBJECT_H
#define OBJECT_H

#include "Common/Include/esUtil.h"
#include "Common/Vector3D.h"
#include "Resources/Model.h"
#include "Resources/Shader.h"
#include "Resources/Texture.h"
#include "cstdio"
#include "Camera/Camera.h"
#include <vector>

extern CCamera *camera;
extern float totalTime;
using namespace std;

typedef struct
{
	char* uniformName;
	CTexture* texture;
} textureStruct;


class CObject
{
public:
	CObject();
	CObject(char* name);

	virtual void update(ESMatrix &ViewProjectionMatrix, float deltaTime);
	virtual void draw();
	void draw(ESMatrix&);
	
	void translate(float x, float y, float z);
	void rotate(float x, float y, float z);
	void scale(float x, float y, float z);
	
	Vector3D& getPosition();
	Vector3D& getRotation();
	Vector3D& getScale();
	CModel* getModel();
	CShader* getShader();
	vector<textureStruct> getTextures();
	ESMatrix getWVPMatrix();
	ESMatrix getWMatrix();
	ESMatrix getWMatrixInverse();
	char* getName();

	virtual void setShader(CShader *shader);
	void setModel(CModel *model);
	void setWVPMatrix(ESMatrix &wvpMatrix);
	void addTexture(CTexture* texture, char* uniformName);

protected:
	void init();
	void setShaderProgram();

	Vector3D m_position;
	Vector3D m_rotation;
	Vector3D m_scale;
	
	char* m_name;
	CModel *m_model;
	CShader *m_shader;
	vector<textureStruct> m_textures;
	
	GLint m_a_positionLocation, m_a_texcoordLocation, m_a_normalLocation, m_a_tangentLocation, m_a_bitangentLocation;
	GLint m_u_textureLocation, m_u_wvpMatrixLocation, m_u_wMatrixLocation, 
		m_u_wMatrixInverseLocation, m_u_cameraPositionLocation, m_u_timeLocation;
	ESMatrix m_wvpMatrix, m_wMatrix;
	

private:
	// Temp
	


};
#endif