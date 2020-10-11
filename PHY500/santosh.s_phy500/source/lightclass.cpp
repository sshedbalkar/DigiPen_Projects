////////////////////////////////////////////////////////////////////////////////
// Filename: lightclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "Precompiled.h"
#include "lightclass.h"

namespace Framework {

LightClass::LightClass()
{
}


LightClass::LightClass(const LightClass& other)
{
	m_ambientColor = other.GetAmbientColor();
	m_diffuseColor = other.GetDiffuseColor();
	m_direction = other.GetDirection();
	m_specularColor = other.GetSpecularColor();
	m_specularPower = other.GetSpecularPower();

	m_position = other.GetPosition();
	m_lookAt = other.GetLookAt();

	other.GetViewMatrix(m_viewMatrix);
	other.GetProjectionMatrix(m_projectionMatrix);
}


LightClass::~LightClass()
{
}

void LightClass::SetAmbientColor(float red, float green, float blue, float alpha)
{
	m_ambientColor = D3DXVECTOR4(red, green, blue, alpha);
	return;
}


void LightClass::SetDiffuseColor(float red, float green, float blue, float alpha)
{
	m_diffuseColor = D3DXVECTOR4(red, green, blue, alpha);
	return;
}

void LightClass::SetPosition(float x, float y, float z)
{
	m_position = D3DXVECTOR4(x, y, z, 1.0f);
	return;
}

void LightClass::SetLookAt(float x, float y, float z)
{
	m_lookAt.x = x;
	m_lookAt.y = y;
	m_lookAt.z = z;
	return;
}

void LightClass::SetDirection(float x, float y, float z)
{
	m_direction = D3DXVECTOR3(x, y, z);
	return;
}

void LightClass::SetSpecularColor(float red, float green, float blue, float alpha)
{
	m_specularColor = D3DXVECTOR4(red, green, blue, alpha);
	return;
}

void LightClass::SetSpecularPower(float power)
{
	m_specularPower = power;
	return;
}

D3DXVECTOR4 LightClass::GetAmbientColor() const
{
	return m_ambientColor;
}

D3DXVECTOR4 LightClass::GetDiffuseColor() const
{
	return m_diffuseColor;
}

D3DXVECTOR4 LightClass::GetPosition() const
{
	return m_position;
}

D3DXVECTOR3 LightClass::GetDirection() const
{
	return m_direction;
}

D3DXVECTOR4 LightClass::GetSpecularColor() const
{
	return m_specularColor;
}

float LightClass::GetSpecularPower() const
{
	return m_specularPower;
}

void LightClass::GenerateViewMatrix()
{
	D3DXVECTOR3 up, m_pos3;


	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 0.0f;
	up.z = 1.0f;

	//m_position

	m_pos3.x = m_position.x;
	m_pos3.y = m_position.y;
	m_pos3.z = m_position.z;

	//m_lookAt = m_pos3 + m_lookAt;

	// Create the view matrix from the three vectors.
	D3DXMatrixLookAtLH(&m_viewMatrix, &m_pos3, &m_lookAt, &up);
	
	return;
}

void LightClass::GenerateProjectionMatrix(float screenDepth, float screenNear)
{
	float fieldOfView, screenAspect;


	// Setup field of view and screen aspect for a square light source.
	fieldOfView = (float)D3DX_PI / 2.0f;
	screenAspect = 1.0f;

	// Create the projection matrix for the light.
	D3DXMatrixPerspectiveFovLH(&m_projectionMatrix, fieldOfView, screenAspect, screenNear, screenDepth);

	return;
}

void LightClass::GetViewMatrix(D3DXMATRIX& viewMatrix) const
{
	viewMatrix = m_viewMatrix;
	return;
}


void LightClass::GetProjectionMatrix(D3DXMATRIX& projectionMatrix) const
{
	projectionMatrix = m_projectionMatrix;
	return;
}

D3DXVECTOR3 LightClass::GetLookAt() const
{
	return m_lookAt;
}
}