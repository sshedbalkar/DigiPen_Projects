///////////////////////////////////////////////////////////////////////////////
///
///	 \file ModelClassBinary.h 
///  Defines the Sprite Component.
///
///	 Authors: Chia-Wei Wu
///  Copyright (c) 2012, Digipen Institute of Technology
///
///////////////////////////////////////////////////////////////////////////////
#pragma once
#include "VertexTypes.h"
#include "ModelClass.h"

#include "Asset.hpp"
namespace Framework
{
class ChunkReader;

enum ModelType
{
	Static,
	Skinned
};

class ModelClassBinary : public ModelClass{
public:
	ModelClassBinary();
	~ModelClassBinary();
	ModelType modelType;
	void* vertexBufferData;
	unsigned int * indexBufferData;
	unsigned int VertexTypeIndex;
	unsigned int VertexSize;
	unsigned int NumVertices;
	unsigned int NumIndices;
	ID3D11Buffer * d3dVertexBuffer;
	ID3D11Buffer * d3dIndexBuffer;
	
	void Initialize(ID3D11Device * device);
	void ReadFrom( ChunkReader& reader );
	void SetAndDraw(ID3D11DeviceContext* deviceContext);
	unsigned int GetIndexCount();
};

ModelClassBinary* LoadModel(std::string filename, ID3D11Device * device, char* textureFilename1, char* textureFilename2, char* textureFilename3);

}
