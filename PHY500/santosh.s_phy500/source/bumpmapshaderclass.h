////////////////////////////////////////////////////////////////////////////////
// Filename: bumpmapshaderclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _BUMPMAPSHADERCLASS_H_
#define _BUMPMAPSHADERCLASS_H_

/////////////
// GLOBALS //
/////////////
#define NUM_LIGHTS 3

//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx10math.h>
#include <d3dx11async.h>
#include <fstream>
using namespace std;

namespace Framework {

////////////////////////////////////////////////////////////////////////////////
// Class name: BumpMapShaderClass
////////////////////////////////////////////////////////////////////////////////
class BumpMapShaderClass
{
private:
	struct MatrixBufferType
	{
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX projection;
	};

	struct CameraBufferType
	{
		D3DXVECTOR3 cameraPosition;
		float padding;
	};

	struct LightBufferType
	{
		D3DXVECTOR4 diffuseColor[NUM_LIGHTS];
		D3DXVECTOR4 ambientColor;
		float specularPower;
		D3DXVECTOR4 specularColor;
		float padding[3];
	};

	struct LightPositionBufferType
	{
		D3DXVECTOR4 lightPosition[NUM_LIGHTS];
	};

public:
	BumpMapShaderClass();
	BumpMapShaderClass(const BumpMapShaderClass&);
	~BumpMapShaderClass();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, const D3DXMATRIX&, const D3DXMATRIX&, const D3DXMATRIX&, ID3D11ShaderResourceView**, const D3DXVECTOR3&, const std::vector<LightClass>&);

private:
	bool InitializeShader(ID3D11Device*, HWND, char*, char*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, char*);

	bool SetShaderParameters(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView**, const D3DXVECTOR3&, const std::vector<LightClass>&);
	void RenderShader(ID3D11DeviceContext*, int);

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11Buffer* m_cameraBuffer;
	ID3D11SamplerState* m_sampleState;
	ID3D11Buffer* m_lightBuffer;
	ID3D11Buffer* m_lightPositionBuffer;
};
}
#endif