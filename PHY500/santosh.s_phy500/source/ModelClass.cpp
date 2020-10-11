///////////////////////////////////////////////////////////////////////////////
///
///	 \file ModelClass.cpp
///  Defines the Virtual Model.
///
///	 Authors: Chia-Wei Wu
///  Copyright (c) 2012, Digipen Institute of Technology
///
///////////////////////////////////////////////////////////////////////////////
#include "Precompiled.h"
#include "ModelClass.h"
#include "ModelInfo.h"
#include "Graphics.h"
#include "d3dclass.h"

namespace Framework {

ModelClass::ModelClass() : 
m_TextureArray(NULL), 
Controller(NULL)
{
	//modelInfo1 = new ModelInfo ;
}
ModelClass::~ModelClass(){
	// Release the texture array object.
	if(m_TextureArray){
		m_TextureArray->Shutdown();
		delete m_TextureArray;
		m_TextureArray = 0;
	}
	SafeDelete( Controller );
	
}

bool ModelClass::LoadTextures(ID3D11Device* device, char* filename1, char* filename2, char* filename3)
{
	bool result;
	// Create the texture array object.
	m_TextureArray = new TextureArrayClass;
	if(!m_TextureArray){return false;}

	// Initialize the texture array object.
	result = m_TextureArray->Initialize(device, filename1, filename2, filename3);
	if(!result){return false;}

	return true;
}


ID3D11ShaderResourceView** ModelClass::GetTextureArray()
{
	return m_TextureArray->GetTextureArray();
}

void ModelClass::SwapTexture()
{
	if (modelInfo1->swaptexture1.length()>0)
	{
		modelInfo1->swaptexture1.swap (modelInfo1->texture1 );
	}
	if (modelInfo1->swaptexture2.length()>0)
	{
		modelInfo1->swaptexture2.swap (modelInfo1->texture2 );
	}
	if (modelInfo1->swaptexture3.length()>0)
	{
		modelInfo1->swaptexture3.swap (modelInfo1->texture3 );
	}
	this->LoadTextures(GRAPHICS->m_D3D->GetDevice(),const_cast<char *>(GetStringBody(modelInfo1->texture1)),  const_cast<char *>(GetStringBody(modelInfo1->texture2)),  const_cast<char *>(GetStringBody(modelInfo1->texture3)));
}

}

