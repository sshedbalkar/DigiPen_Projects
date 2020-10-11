////////////////////////////////////////////////////////////////
//
//	BasicEffect.h
//	Abstraction layer for a shader
//
//	Author: Santosh Shedbalkar
//	Copyright 2011, Digipen Institute of Technology
//
////////////////////////////////////////////////////////////////
#ifndef BASIC_EFFECT_H
#define BASIC_EFFECT_H
//
#include "VMath.h"
//
struct IDirect3DTexture9;
struct ID3DXEffect;
//
namespace Wye
{
	class BasicEffect
	{
#pragma message("Including class: BasicEffect")
	public:
		BasicEffect(const char* shaderId);
		BasicEffect();
		virtual ~BasicEffect();
		//
		void setMatrices(D3DXMATRIXA16 *scaling, D3DXMATRIXA16 *rotation, D3DXMATRIXA16 *translate, D3DXMATRIXA16 *container, D3DXMATRIXA16 *viewProj);
		virtual void draw(IDirect3DDevice9* pDevice, IDirect3DTexture9* texture);
		inline void color(const Vec4& col)
		{
			_color = col;
		}
		inline std::string shader()
		{
			return _name;
		}
		void shader(const char* shaderId);
	protected:
		void calculateWorldViewProj();
	private:
		ID3DXEffect* _shader;
		Vec4 _color;
		std::string _name;
		D3DXMATRIXA16 *_scale;
		D3DXMATRIXA16 *_rotate;
		D3DXMATRIXA16 *_translate;
		D3DXMATRIXA16 *_container;
		D3DXMATRIXA16 *_viewProj;
		D3DXMATRIXA16 _worldViewProj;
	};
}
//
#endif