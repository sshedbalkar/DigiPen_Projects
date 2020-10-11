#pragma once
#include "Composition.h"
#include "VMath.h"
#include "lightclass.h"

namespace Framework
{

class Transform;
class ModelClass;
class TextureArrayClass;

class ModelComponent : public GameComponent
{	
public:
	ModelComponent * Next;
	ModelComponent * Prev;
	ModelComponent();
	virtual ~ModelComponent();

	virtual void Serialize( ISerializer& stream );
	virtual void Initialize();
	virtual void Free();
	void Draw(ID3D11DeviceContext* deviceContext, float dt, const D3DXMATRIX&, const D3DXMATRIX&, const D3DXMATRIX&, ID3D11ShaderResourceView** textureArray, const D3DXVECTOR3& cameraPosition, const std::vector<LightClass>&, D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView**);//Draw the sprite to the screen
	void SwapModel();
	void DefaultModel();
	void ChangeTextureDebug( );
	void ChangeTextureBack( );
///private:
	Transform* transform;
	std::string ModelName;
	ModelClass* pModel;
	TextureArrayClass* pTextureArray;
	
	std::string SwapModelName;
	std::string InitialSwapModelName;
	std::string InitialModelName;
	
};

}