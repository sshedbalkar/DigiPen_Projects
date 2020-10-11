///////////////////////////////////////////////////////////////////////////////
///
///	 \file ModelClass.h 
///  Defines the Virtual Model.
///
///	 Authors: Chia-Wei Wu
///  Copyright (c) 2012, Digipen Institute of Technology
///
///////////////////////////////////////////////////////////////////////////////
#pragma once
#include "texturearrayclass.h"
#include "Animation.h"

namespace Framework {

enum MultiTextureType
{
	SingleTexture = 0,
	TextureBlend,
	LightMap,
	AlphaBlend,
	Fog,
	ClipPlane,
	Transparent, 
	Bump, 
	Spec
};

class ModelInfo;

class ModelClass{
public:
	friend class ModelComponent;
	friend class ModelComponent2;
	ModelClass();
	virtual ~ModelClass();
	virtual void SetAndDraw(ID3D11DeviceContext* deviceContext) = 0;
	virtual unsigned int GetIndexCount() = 0;
	ID3D11ShaderResourceView** GetTextureArray();
	void SwapTexture();

	bool LoadTextures(ID3D11Device*, char*, char*, char*);
	void ReleaseTextures();
public:
	AnimationController * Controller;
	TextureArrayClass* m_TextureArray;
	MultiTextureType mtt;
	ModelInfo * modelInfo1;
};

}
