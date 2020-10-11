///////////////////////////////////////////////////////////////////////////////
///
///	 \file Sprite.h 
///  Defines the Sprite Component.
///
///	 Authors: Chris Peters
///  Copyright 2010, Digipen Institute of Technology
///
///////////////////////////////////////////////////////////////////////////////
#ifndef SPRITE_H
#define SPRITE_H
//#pragma message("Compiling: "__FILE__)
//
#include "Transform.h"
#include "DirectXIncludes.h"
#include "VMath.h"

namespace Wye
{
	class Component;
	class BasicEffect;
	//
	/// A two-dimensional hardware accelerated sprite class using textures.
	/// Has color, size, and a sprite texture name.
	/// Depends on Transform.
	class Sprite: public Component
	{
#pragma message("Including Class: Sprite")
	public:
		//Linked list Nodes
		Sprite* Next;
		Sprite* Prev;
		Sprite();
		virtual ~Sprite();
		void serialize(const ISerializer& stream);
		void initialize();
		//Draw the sprite to the screen
		virtual void draw(IDirect3DDevice9* pDevice);
		void setShader(const char* shaderId);
		void setTexture(IDirect3DTexture9* texture);
		IDirect3DTexture9* getTexture();
		void setTransform(Transform* transform);
		Transform* getTransform();
		void setSpriteName(const std::string& name);
		const std::string& getSpriteName();
		void setColor(Vec4& color);
		void size(const Vec2& sz);
		void effect(BasicEffect* effekt);
		inline BasicEffect* effect()
		{
			return _effect;
		}
		//
	protected:
		bool _sizeSet;
		Vec2 _size;
		//The height and width of the sprite, in pixels, 
		//which usually matches the texture size.
		Vec2 _texDim;
		//What texture to use for this sprite
		IDirect3DTexture9* _pTexture;
		//What Transform to use for this sprite
		Transform* _transform;
		Transform* _parentTransform;
		//Name of the sprite asset texture
		std::string _spriteName;
		//Blend color of this sprite
		Vec4 _color;
		std::string _shaderId;
		//ID3DXEffect* _shader;
		BasicEffect* _effect;
	};
}
//
#endif