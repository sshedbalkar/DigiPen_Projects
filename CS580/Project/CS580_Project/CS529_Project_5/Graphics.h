///////////////////////////////////////////////////////////////////////////////
///
///	\file Graphics.h
/// Defines grahics system.
///	
///	Authors: Benjamin Ellinger, Chris Peters
///	Copyright 2010, Digipen Institute of Technology
///
///////////////////////////////////////////////////////////////////////////////
#pragma once //Makes sure this header is only included once

#include "Sprite.h"
#include "System.h"
#include "Containers.h"
#include "Camera.h"
#include "VMath.h"
//
namespace Wye
{	
	//Forward Declaration of Graphics Objects
	class Camera;

	/*
	enum PixelShaders
	{
		Basic = 0,
		DebugShader,
		NumberOfShaders
	};
	*/
	///A two-dimensional hardware accelerated non fixed function 
	///sprite based graphics system.
	///Provides Sprite and Camera GameComponents.
	class Graphics: public System
	{
//#pragma message("Including Class: Graphics")
	public:
		typedef ObjectLinkList<Sprite> SpriteListType;
		///Update by rendering the scene
		void update(float dt);
		virtual std::string getName(){return "Graphics";}
		//Initialize the Direct3D system.
		Graphics();
		~Graphics();
		//Get a texture asset. Will return null if texture is not loaded
		IDirect3DTexture9* getTexture(const std::string& fileName);
		Vec3 screenToWorldSpace(Vec2);
		void setWindwProperties(HWND hWnd, int screenWidth, int screenHeight);
		//
		void setMainCamera(Camera* camera);
		Camera* getMainCamera();
		IDirect3DDevice9* getD3DDevice();
		IDirect3DVertexBuffer9* getQVF();
		SpriteListType* getSpriteList();
		ID3DXEffect* getShader(const std::string id);
		Mat4* getViewMatrix();
		void setViewMatrix(Mat4& view);
		Mat4* getProjectionMatrix();
		void setProjectionMatrix(Mat4& proj);
		Mat4* getViewProjectionMatrix();
		void setViewProjectionMatrix(Mat4& proj);
		void debugShader(const std::string id);
		//
	public:
		void initialize();
		//Create a vertex buffer for our sprites.
		bool initGeometry();
		//Load all the textures for our game.
		void loadAssets();
		//Load an individual texture.
		void loadTexture(const char* id, const std::string& filename);
		//Set up the default world, view, and projection matrices
		void setupMatrices();
		//Load a effect file
		bool loadEffect(const char* id, const std::string& filename);
		//Draw Debug Data
		void drawDebugInfo();
		//Draw the world
		void drawWorld();
		//TODO: Need to handle device lost / device reset
		void deviceLost();
		void deviceReset();
		void drawText(const char* text, int top, int left, Vec4 &color = Vec4(1.0f, 1.0f, 1.0f, 1.0f));
		inline void worldTransform(Mat4& world)
		{
			_worldTransform = world;
		}
		//
	private:
		//The active camera
		Camera* _mainCamera;
		//
		HWND _hWnd;
		int _screenWidth;
		int _screenHeight;
		//
		//Direct3D
		IDirect3D9* _pD3D;
		//Direct3D Device
		IDirect3DDevice9* _pDevice;
		//
		//The quad vertex buffer
		IDirect3DVertexBuffer9* _pQuadVertexBuffer;
		//
		//Stored texture assets
		typedef std::map<std::string, IDirect3DTexture9*> TextureMap;
		TextureMap _textures;
		//
		//An array for our pixel shaders.
		typedef std::map<std::string, ID3DXEffect*> ShaderMap;
		//typedef std::map<std::string, LPD3DXEFFECT*> ShaderMap;
		ShaderMap _shaders;
		//ID3DXEffect* _shaders[NumberOfShaders];
		//
		//World Projection and view matrices
		Mat4 _projMatrix;
		Mat4 _viewMatrix;
		Mat4 _viewProjMatrix;
		Mat4 _worldTransform;
		//
		//Stored copy of the DirectX Presentation Parameters
		D3DPRESENT_PARAMETERS _presentParameters;
		Vec2 _surfaceSize;
		//
		SpriteListType _spriteList;
		//
		ID3DXEffect* _debugShader;
		ID3DXFont *_pFont;
		//
		struct TextData
		{
			TextData(const char* str, int& t, int& l, Vec4& c)
				:text(str),
				top(t),
				left(l),
				color(c)
			{
				//
			}
			std::string text;
			int top;
			int left;
			Vec4 color;
		};
		std::vector<TextData*> _textBuffer;
		void createFont();
		void renderAllTexts();
		void renderText(const char* text, int top, int left, Vec4& color);
	};
	//
	//A global pointer to the Graphics system, used to access it anywhere.
	extern Graphics* GRAPHICS;
}
