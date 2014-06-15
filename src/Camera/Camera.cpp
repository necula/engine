#include "Camera.h"


CCamera::CCamera()
{
	reset();
}

void CCamera::reset()
{
	m_px = 0; m_py = 0; m_pz = 0;
	m_rx = 0; m_ry = 0; m_rz = 0;
	m_ax = 0; m_ay = 0; m_az = 0;

	m_position = Vector3D(0.0f, 0.0f, 0.0f);

	m_up = Vector3D(0.0f, 1.0f, 0.0f);
	m_right = Vector3D(1.0f, 0.0f, 0.0f);
	m_forward = Vector3D(0.0f, 0.0f, -1.0f);
}

void CCamera::update(float deltaTime)
{
	
}

void CCamera::moveBackward(float step)
{
	//m_position.z += step;
	m_position = m_position + (m_forward * -step);
}

void CCamera::moveForward(float step)
{
	//m_position.z -= step;
	m_position = m_position + (m_forward * step);
}

void CCamera::moveLeft(float step)
{
	//m_position.x -= step;
	m_position = m_position + (m_right * -step);
}

void CCamera::moveRight(float step)
{
	//m_position.x += step;
	m_position = m_position + (m_right * step);
}

void CCamera::moveAbsoluteUp(float step)
{
	m_position.y += step;
}

void CCamera::moveAbsoluteDown(float step)
{
	m_position.y -= step;
}

ESMatrix* CCamera::getViewMatrix()
{

	esMatrixLoadIdentity(&m_viewMatrix);
	esRotate(&m_viewMatrix, -m_ax, 1.0f, 0.0f, 0.0f);
	esRotate(&m_viewMatrix, -m_ay, 0.0f, 1.0f, 0.0f);
	esRotate(&m_viewMatrix, -m_az, 0.0f, 0.0f, 1.0f);
	
	esTranslate(&m_viewMatrix, -m_position.x, -m_position.y, -m_position.z);
	
	return &m_viewMatrix;
}

void CCamera::rotateY(float step)
{
	Vector3D fwdNew, rightNew;
	float angle = (float)(step * (PI / 180));

	float temp = m_ax;
	rotateX(-temp);

	fwdNew = m_forward * cos(angle) + m_right * sin(angle);
	rightNew = m_right * cos(angle) - m_forward * sin(angle);

	m_right = rightNew;
	m_forward = fwdNew;	
	
	rotateX(temp);

	m_ay += step;
}

void CCamera::rotateX(float step)
{
	Vector3D fwdNew, upNew;
	float angle = (float)(-step * (PI / 180));

	fwdNew = m_forward * cos(angle) + m_up * sin(angle);
	upNew = m_up * cos(angle) - m_forward * sin(angle);

	m_up = upNew;
	m_forward = fwdNew;
	

	m_ax += step;
}

Vector3D& CCamera::getPosition()
{
	return m_position;
}

float CCamera::getYRotation()
{
	return m_ay;
}