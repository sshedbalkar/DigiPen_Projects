///////////////////////////////////////////////////////////////////////////////////////
//
//	Sprite.cpp
//	
//	Authors: Benjamin Ellinger, Chris Peters
//	Copyright 2010, Digipen Institute of Technology
//
///////////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "BasicEffect.h"
#include "Sprite.h"
#include "Transform.h"
#include "VertexTypes.h"
#include "ComponentTypeIds.h"
#include "Graphics.h"
#include "Factory.h"
#include "Utility.h"
//
namespace Wye
{

	Sprite::Sprite()
		:Component(CT_Sprite),
		Next(NULL),
		Prev(NULL),
		_texDim(1.0f, 1.0f),
		_pTexture(NULL),
		_transform(static_cast<Transform*>(FACTORY->createComponent(CT_Transform))),
		_parentTransform(NULL),
		_spriteName(""),
		_color(0.0f, 0.0f, 0.0f, 1.0f),
		//_shader(NULL),
		_sizeSet(false),
		_size(1.0f, 1.0f),
		_effect(NULL),
		_shaderId("")
	{
		//
	}

	Sprite::~Sprite()
	{
		GRAPHICS->getSpriteList()->erase(this);
		delete _effect;
	}

	void Sprite::initialize()
	{ 
		//transform = (Transform*)Base->GetComponent( "Transform" ); 
		//transform = getOwner()->has( Transform );
		_transform->initialize();
		_parentTransform = getParent()->getComponent<Transform>(CT_Transform);
		//_transform->copyFromParent();
		GRAPHICS->getSpriteList()->push_back(this);
		if(!_spriteName.empty())
		{
			setTexture(GRAPHICS->getTexture(_spriteName));
		}
		setShader(_shaderId.c_str());
		//_texDim *= 100;
		//
		//std::cout<<"Sprite: Tex: "<<_texDim<<", name: " <<_spriteName<<", color: " <<_color<<", Shader: "<<_shaderId<<", Parent Trans: " << _parentTransform<<"\n";
	}
	void Sprite::serialize(const ISerializer& stream)
	{
		//std::cout<<"Sprite serializing\n";
		ISerializer* str1 = stream.clone();
		ISerializer* str = stream.clone();
		str->readProperty("Data", *str);
		if(str->isGood())
		{
			//str->readProperty("size", _texDim);
			str->readProperty("color", _color);
			str->readProperty("sprite", _spriteName);
			str->readProperty("shader", _shaderId);
			Vec2 sz;
			if(str->readProperty("size", sz))
			{
				size(sz);
			}
			_transform->serialize(*str1);
		}
		//std::cout<<"Sprite serialized: Texture: " <<_spriteName<<", Color: " <<_color<<", Shader: "<<_shaderId<<", Size: "<<_texDim<<"\n";
		delete str;
		delete str1;
	}

	void Sprite::draw(IDirect3DDevice9* pDevice)
	{
		if(_pTexture)
		{
			Mat4 matSprite, matScaling, matTranslate, matRotate, matContainer, *pCont = NULL;
			if(_sizeSet)
			{
				D3DXMatrixScaling(&matScaling, _transform->scale.x * _size.x, _transform->scale.y * _size.y, _transform->scale.z);
			}
			else
			{
				D3DXMatrixScaling(&matScaling, _transform->scale.x * _texDim.x, _transform->scale.y * _texDim.y, _transform->scale.z);
			}
			D3DXMatrixRotationYawPitchRoll(&matRotate, _transform->rotation.y, _transform->rotation.x, _transform->rotation.z);
			D3DXMatrixTranslation(&matTranslate, floor(_transform->position.x), floor(_transform->position.y), floor(_transform->position.z));
			//
			if(_parentTransform != NULL)
			{
				Mat4 rot2, trans2;
				D3DXMatrixScaling(&matContainer, _parentTransform->scale.x, _parentTransform->scale.y, _parentTransform->scale.z);
				D3DXMatrixRotationYawPitchRoll(&rot2, _parentTransform->rotation.y, _parentTransform->rotation.x, _parentTransform->rotation.z);
				D3DXMatrixMultiply(&matContainer, &matContainer, &rot2);
				D3DXMatrixTranslation(&trans2, _parentTransform->position.x, _parentTransform->position.y, _parentTransform->position.z);
				D3DXMatrixMultiply(&matContainer, &matContainer, &trans2);
				pCont = &matContainer;
			}
			_effect->setMatrices(&matScaling, &matRotate, &matTranslate, pCont, GRAPHICS->getViewProjectionMatrix());
			//
			_effect->draw(pDevice, _pTexture);
		}
	}
	//
	void Sprite::setShader(const char* shaderId)
	{
		_shaderId.assign(shaderId);
		//_shader = GRAPHICS->getShader(_shaderId);
		effect(new BasicEffect(shaderId));
	}
	void Sprite::effect(BasicEffect* effekt)
	{
		delete _effect;
		_effect = effekt;
	}
	void Sprite::setTexture(IDirect3DTexture9* texture)
	{
		_pTexture = texture;
		if(_pTexture)
		{
			D3DSURFACE_DESC d;
			_pTexture->GetLevelDesc(0, &d);
			_texDim.x = (FLOAT)d.Width;
			_texDim.y = (FLOAT)d.Height;
			//_texDim.x = 10;
			//_texDim.y = 10;
			//std::cout<<"Size: X: "<<d.Width<<", Y: "<<d.Height<<"\n";
		}
	}
	IDirect3DTexture9* Sprite::getTexture()
	{
		return _pTexture;
	}
	void Sprite::setTransform(Transform* transform)
	{
		_transform = transform;
	}
	Transform* Sprite::getTransform()
	{
		return _transform;
	}
	void Sprite::setSpriteName(const std::string& name)
	{
		_spriteName = name;
		if(!name.empty())
		{
			setTexture(GRAPHICS->getTexture(_spriteName));
		}
		else
		{
			_pTexture = NULL;
		}
	}
	const std::string& Sprite::getSpriteName()
	{
		return _spriteName;
	}
	void Sprite::setColor(Vec4& color)
	{
		_color = color;
	}
	void Sprite::size(const Vec2& sz)
	{
		_size = sz;
		_sizeSet = (!sz.x || !sz.y) ? false : true;
	}
}