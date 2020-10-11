#include "BasicEffect.h"
#include "DebugTools.h"
#include "Graphics.h"
#include "VertexTypes.h"
#include <string>
//
namespace Wye
{
	BasicEffect::BasicEffect(const char* shaderId)
		:_name(shaderId),
		_color(0, 0, 0, 0),
		_shader(GRAPHICS->getShader(shaderId))
	{
		//std::cout<<"Shader id: "<<shaderId<<", shader: " <<_shader<<"\n";
	}
	BasicEffect::BasicEffect()
		:_name(""),
		_color(0, 0, 0, 0),
		_shader(NULL)
	{
		//std::cout<<"Shader id: "<<shaderId<<", shader: " <<_shader<<"\n";
	}
	BasicEffect::~BasicEffect()
	{
		//
	}
	void BasicEffect::shader(const char* shaderId)
	{
		if(_name.empty() || !strcmp(shaderId, _name.c_str()))
		{
			_name = shaderId;
			_shader = GRAPHICS->getShader(shaderId);
		}
	}
	void BasicEffect::setMatrices(D3DXMATRIXA16 *scaling, D3DXMATRIXA16 *rotation, D3DXMATRIXA16 *translate, D3DXMATRIXA16 *container, D3DXMATRIXA16 *viewProj)
	{
		_scale = scaling;
		_rotate = rotation;
		_translate = translate;
		_container = container;
		_viewProj = viewProj;
	}
	void BasicEffect::calculateWorldViewProj()
	{
		D3DXMatrixMultiply(&_worldViewProj, _scale, _rotate);
		D3DXMatrixMultiply(&_worldViewProj, &_worldViewProj, _translate);
		if(_container)
		{
			D3DXMatrixMultiply(&_worldViewProj, &_worldViewProj, _container);
		}
		D3DXMatrixMultiply(&_worldViewProj, &_worldViewProj, _viewProj);
	}
	void BasicEffect::draw(IDirect3DDevice9* pDevice, IDirect3DTexture9* texture)
	{
		//return;
		calculateWorldViewProj();
		//
		_shader->SetTechnique("Technique0");
		pDevice->SetStreamSource(0, GRAPHICS->getQVF(), 0, sizeof(Vertex2D));
		pDevice->SetFVF(VERTEX2D_FVF);
		//
		UINT numberOfPasses = 0;
		_shader->SetMatrix("WorldViewProj", &_worldViewProj);
		_shader->Begin(&numberOfPasses, 0);
		_shader->SetTexture("texture0", texture);
		_shader->SetVector("color", &_color);
		//
		for(UINT pass=0; pass<numberOfPasses; ++pass)
		{
			_shader->BeginPass(pass);
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);
			_shader->EndPass();
		}
		_shader->End();
	}
}