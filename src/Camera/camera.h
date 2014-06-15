#ifndef CAMERA_H
#define CAMERA_H


#include "../Common/Include/esUtil.h"
#include "../Common/Vector3D.h"
#include "cstdio"

#define PI 3.141592653589793

class CCamera
{
public:
	CCamera();
	
	void	update(float deltaTime);

	ESMatrix*	getViewMatrix(); // returns inverted world matrix

	void	moveForward(float step);
	void	moveBackward(float step);
	void	moveLeft(float step);
	void	moveRight(float step);
	void	reset();

	void	moveAbsoluteUp(float step);
	void	moveAbsoluteDown(float step);

	void	rotateY(float step);
	void	rotateX(float step);

	Vector3D& getPosition();
	float	getYRotation();
private:

	float	m_px, m_py, m_pz; //pos
	float	m_rx, m_ry, m_rz; //rot
	float	m_ax, m_ay, m_az; //rot angle
	Vector3D m_position, m_right, m_forward, m_up;
	ESMatrix m_viewMatrix;
};



#endif