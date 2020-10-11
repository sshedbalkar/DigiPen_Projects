////////////////////////////////////////////////////////////////////////////////
// Filename: lightclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _LIGHTCLASS_H_
#define _LIGHTCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3dx10math.h>

namespace Framework {

////////////////////////////////////////////////////////////////////////////////
// Class name: LightClass
////////////////////////////////////////////////////////////////////////////////
class LightClass
{
public:
	LightClass();
	LightClass(const LightClass&);
	~LightClass();

	void SetAmbientColor(float, float, float, float);
	void SetDiffuseColor(float, float, float, float);
	void SetDirection(float, float, float);
	void SetSpecularColor(float, float, float, float);
	void SetSpecularPower(float);

	void SetPosition(float, float, float);
	void SetLookAt(float, float, float);

	void GenerateViewMatrix();
	void GenerateProjectionMatrix(float, float);

	D3DXVECTOR4 GetAmbientColor() const;
	D3DXVECTOR4 GetDiffuseColor() const;
	D3DXVECTOR3 GetDirection() const;
	D3DXVECTOR4 GetSpecularColor() const;
	float GetSpecularPower() const;

	D3DXVECTOR4 GetPosition() const;

	D3DXVECTOR3 GetLookAt() const;
	void GetViewMatrix(D3DXMATRIX&) const;
	void GetProjectionMatrix(D3DXMATRIX&) const;

private:
	D3DXVECTOR4 m_ambientColor;
	D3DXVECTOR4 m_diffuseColor;
	D3DXVECTOR3 m_direction;
	D3DXVECTOR4 m_specularColor;
	float m_specularPower;
	
	D3DXVECTOR4 m_position;
	D3DXVECTOR3 m_lookAt;
	D3DXMATRIX m_viewMatrix;
	D3DXMATRIX m_projectionMatrix;
};
}
#endif