///////////////////////////////////////////////////////////////////////////////////////
//
//	Graphics.cpp
//	
//	Authors: Benjamin Ellinger, Chris Peters
//	Copyright 2010, Digipen Institute of Technology
//
///////////////////////////////////////////////////////////////////////////////////////

#include "Precompiled.h"
#include "Graphics.h"
#include "DebugDraw.h"
#include "VertexTypes.h"
#include "FilePath.h"
#include "Containers.h"
#include "ComponentCreator.h"
#include "Factory.h"
#include "ComponentTypeIds.h"
#include "Utility.h"
#include <iostream>
//
namespace Wye
{
	//Our global pointer to Graphics.
	Graphics* GRAPHICS = NULL;


	//Set everything to default values.
	Graphics::Graphics()
	{
		_pD3D = NULL;
		_pDevice = NULL;
		_pQuadVertexBuffer = NULL;
		//ZeroMemory(_shaders, sizeof(_shaders));
		//
		ErrorIf(GRAPHICS!=NULL,"Graphics already initialized.");
		GRAPHICS = this;
	}

	void Graphics::setWindwProperties(HWND hWnd, int screenWidth, int screenHeight)
	{
		_hWnd = hWnd;
		_screenWidth = screenWidth;
		_screenHeight = screenHeight;
	}

	//Initializes Direct3D
	void Graphics::initialize()
	{
		//Create the D3D object (the parameter is there to make sure the app was built correctly).
		_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

		D3DDISPLAYMODE displayMode;
		_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &displayMode);

		//Set up the structure used to create the D3DDevice.
		ZeroMemory(&_presentParameters, sizeof(_presentParameters));
		//
		D3DPRESENT_PARAMETERS& pp = _presentParameters;
		pp.Windowed = true;						//You can't just set this to FALSE--you'll need to change other stuff as well.
		pp.SwapEffect = D3DSWAPEFFECT_DISCARD;	//Picks the best way to handle back buffers for you, but it means you have draw a full screen every time.
		pp.BackBufferFormat = D3DFMT_UNKNOWN;	//This is for windowed apps, full screen will need to be explicit.
		//pp.BackBufferFormat = _presentParameters.BackBufferFormat;
		//pp.PresentationInterval  = D3DPRESENT_INTERVAL_DEFAULT;
		pp.PresentationInterval  = D3DPRESENT_INTERVAL_IMMEDIATE;
		pp.EnableAutoDepthStencil = TRUE;
		pp.AutoDepthStencilFormat = D3DFMT_D16;
		//
		pp.BackBufferWidth = _screenWidth;
		pp.BackBufferHeight = _screenHeight;

		//Create the D3DDevice
    // IF THIS CALL FAILS:
    //   The most common cause is you are trying to run this on a machine that
    //   does not support hardware vertex processing. At DigiPen, this probably means
    //   your machine does not have a graphics card. Get one, switch machines, or 
    //   alternatively replace the D3DCREATE_HARDWARE_VERTEXPROCESSING constant below 
    //   with D3DCREATE_SOFTWARE_VERTEXPROCESSING.
		DXVerify(_pD3D->CreateDevice(D3DADAPTER_DEFAULT,	//The graphics adapter to be used.
									  D3DDEVTYPE_HAL,		//Type of graphics device: Hardware acceleration or software
									  _hWnd,					//Window Handle for the device.
									  D3DCREATE_HARDWARE_VERTEXPROCESSING,	//Device behavior: vertex processing (software, mixed, hardware), double precision, etc.
									  &pp,				//The presentation parameters created above.
 									  &_pDevice));			//A pointer to the new device.

		//Set our render states (culling, lighting, shading, zbuffers, etc.).
		_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);			//Turn off culling
		_pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);					//Turn off D3D lighting
		_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);			//Turn on alpha blending.
		_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);		//Use the texture's alpha channel.
		_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);	//Use the inverse of the texture's alpha channel.
		_pDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

		//_surfaceSize = Vec2((float)_screenWidth, (float)_screenHeight);
		_surfaceSize.x = (float)_screenWidth;
		_surfaceSize.y = (float)_screenHeight;

		//Create an orthogonal left-handed projection matrix.
		//This will transform everything to the view port with no perspective.
		//The near and far clipping plains are still needed, but not as important.
		Mat4 matProj;
		D3DXMatrixOrthoLH(&matProj, _surfaceSize.x, _surfaceSize.y, 1.0f, 500.0f);
		//std::cout<<"Proj Mat11: " <<matProj<<"\n";
		//D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(20.0f), 1.25f, 1.0f, 100.0f);
		//D3DXMatrixPerspectiveLH( &matProj, 800, 600, 1.0f, 100.0f );
		//Store the projection matrix;
		_projMatrix = matProj;

		_pDevice->SetTransform(D3DTS_PROJECTION, &_projMatrix);

		D3DXMatrixIdentity(&_worldTransform);
		//D3DXMatrixTranslation(&_worldTransform, 0.0f, -50.0f, 0.0f);
		//RegisterComponent(Sprite);
		//RegisterComponent(CT_Camera, Camera);
		FACTORY->addComponentCreator(CT_Sprite, new ComponentCreatorType<Sprite>(CT_Sprite));
		FACTORY->addComponentCreator(CT_Camera, new ComponentCreatorType<Camera>(CT_Camera));

		//Load all of our assets (textures and shaders)
		loadAssets();
		createFont();
		//
		std::cout<<"Graphics inited\n";
	}

	//Release Direct3D and do any needed cleanup.
	Graphics::~Graphics()
	{

		//Release all the shaders.
		for(ShaderMap::iterator it1 = _shaders.begin(), it2 = _shaders.end(); it1 != it2; ++it1)
		{
			it1->second->Release();
		}
		/*
		for (int i = 0; i < NumberOfShaders; i++)
		{
			if (_shaders[i] != NULL)
			{
				_shaders[i]->Release();
			}
			_shaders[i] = NULL;
		}
		*/
		//
		TextureMap::iterator it1 = _textures.begin(), it2 = _textures.end();
		//Release all the texture in the texture
		for(; it1 != it2; ++it1)
		{
			it1->second->Release();
		}
		
		//Release the vertex buffer.
		safeRelease(_pQuadVertexBuffer);
		//Release the device.
		safeRelease(_pDevice);
		//Release the Direct3D object.
		safeRelease(_pD3D);
		safeRelease(_pFont);
	}

	//Render the array of sprites.
	void Graphics::update(float dt)
	{
		//Clear the backbuffer and fill it with the background color.
		//The first parameter is the number of rectangles you are going to clear--0 means clear the whole thing.
		//The second parameter is the array of rectangle to clear--just set it to NULL.
		//The third parameter says what to clear (in this case, the render target, but could be a zbuffer or stencil buffer).
		//The fourth parameter is the color to fill the target with.
		//The fifth and last parameters are the values to fill the zbuffer and stencil buffer with (if necessary).
		_pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);

		//Begin the scene
		if(SUCCEEDED(_pDevice->BeginScene()))
		{
			drawWorld();
			renderAllTexts();
			drawDebugInfo();
			//
			//End the scene
			_pDevice->EndScene();
		}
		//
		//Present the backbuffer contents to the display.
		//The parameters are: source rect, dest rect, dest window (NULL meaning the default window), and dirty region.
		_pDevice->Present(NULL, NULL, NULL, NULL);
	}

	void Graphics::drawWorld()
	{
		//Setup the world, view, and projection matrices
		setupMatrices();

		//TODO: Draw background

		//Iterate through the link list of sprite and draw them all
		//TODO: Need Visibility to cull off screen sprites
		ObjectLinkList<Sprite>::iterator it1 = _spriteList.begin(), it2 = _spriteList.end();
		for(; it1!=it2; ++it1)
		{
			it1->draw(_pDevice);
		}
	}

	void Graphics::drawDebugInfo()
	{
		Drawer::Instance.flush();

		std::vector<Drawer::LineSegment>& Lines = Drawer::Instance._lineSegments;
		std::vector<Drawer::LineSet>& Sets = Drawer::Instance._sets;
		_pDevice->SetFVF(LINE_FVF);

		//Each set is collection of lines that are in the same style (color)
		//so they can be draw together
		unsigned index = 0;
		//std::cout<<"Debug sets: "<<Sets.size()<<"\n";
		for(unsigned set=0; set<Sets.size(); ++set)
		{
			//Draw all the lines in the set
			unsigned numberOfSegments = Sets[set].Segments;
			Vec4 lineColor = Sets[set].Color;
			
			//Mat4 worldViewProj = _viewMatrix * _projMatrix;

			_debugShader->SetMatrix("WorldViewProj", &_viewProjMatrix);
			_debugShader->SetVector("color", &lineColor);
			
			UINT numberOfPasses = 0;
			_debugShader->Begin(&numberOfPasses, 0);
			for(UINT pass=0; pass<numberOfPasses; ++pass)
			{
				_debugShader->BeginPass(pass);
				_pDevice->DrawPrimitiveUP(D3DPT_LINELIST, numberOfSegments, &Lines[index] , sizeof(Drawer::LineSegment) / 2);
				index += numberOfSegments;
				_debugShader->EndPass();
			}
			_debugShader->End();
		}

		//Clear all the lines they must be submitted again each frame
		Drawer::Instance.clear();
	}

	//Set up the world, view, and projection transform matrices.
	void Graphics::setupMatrices()
	{
		//Set the position of the camera.
		//Vec3& cpos = *_mainCamera->getPosition();
		//float cameraX = cpos.x;
		//float cameraY = cpos.y;
		//float cameraH = cpos.z;

		//The eye point is the location of the viewer (center of the screen, 10 units away).
		//Vec3 eyePoint( cameraX, cameraY, cameraH );
		//The look-at point is where the viewer is looking (center of the screen).
		//Vec3 lookAtPoint( cameraX, cameraY, 0.0f );
		//The up vector defines which way is up (the y-direction).
		//Vec3 upVector( 0.0f, 1.0f, 0.0f );
		//Create a left-handed view matrix.
		Mat4 matView;
		//D3DXMatrixLookAtLH(&matView, &eyePoint, &lookAtPoint, &upVector);
		Vec3 cpos(*_mainCamera->getPosition()), cLook(cpos.x, cpos.y, 0.0f);
		//D3DXMatrixLookAtLH(&matView, _mainCamera->getPosition(), _mainCamera->getLookAt(), _mainCamera->getUpVec());
		D3DXMatrixLookAtLH(&matView, _mainCamera->getPosition(), &cLook, _mainCamera->getUpVec());
		//std::cout<<" Camera Pos: "<<*_mainCamera->getPosition()<<", Look at: " <<*_mainCamera->getLookAt()<<", View: " <<matView<<"\n";
		//Store the view matrix
		_viewMatrix = matView;

		//Store the view projection matrix
		_viewProjMatrix = _worldTransform * _viewMatrix * _projMatrix;
		
		/*
		float cameraX = _mainCamera->getTransform()->position.x;
		float cameraY = _mainCamera->getTransform()->position.y;
		float cameraH = -10.0f;

		//The eye point is the location of the viewer (center of the screen, 10 units away).
		Vec3 eyePoint(cameraX, cameraY, cameraH);
		//The look-at point is where the viewer is looking (center of the screen).
		Vec3 lookAtPoint(cameraX, cameraY, 0.0f);
		//The up vector defines which way is up (the y-direction).
		Vec3 upVector(0.0f, 1.0f, 0.0f);
		//Create a left-handed view matrix.
		Mat4 matView;
		D3DXMatrixLookAtLH(&matView, &eyePoint, &lookAtPoint, &upVector);
		std::cout<<" Camera Pos: "<<eyePoint<<", Look at: " <<lookAtPoint<<", View: " <<matView<<"\n";
		//Store the view matrix
		_viewMatrix = matView;
		//Create an orthogonal left-handed projection matrix.
		//This will transform everything to the view port with no perspective.
		//The near and far clipping plains are still needed, but not as important.
		//Mat4 matProj;
		//D3DXMatrixOrthoLH(&matProj, _surfaceSize.x, _surfaceSize.y, 1.0f, 100.0f);
		//Store the projection matrix;
		//_projMatrix = matProj;
		//std::cout<<"Proj Mat22: " <<matProj<<"\n";
		//Store the view projection matrix
		_viewProjMatrix = _viewMatrix * _projMatrix;
		*/
		
	}

	Vec3 Graphics::screenToWorldSpace(Vec2 screenPosition)
	{
		//Convert to normalized screen space 0 to 1
		screenPosition.x /= _surfaceSize.x;
		screenPosition.y /= _surfaceSize.y;

		//Convert to projected space -1 to 1
		screenPosition *= 2.0f;
		screenPosition -= Vec2(1.0f, 1.0f);
		screenPosition.y *= -1.0f;
		Vec3 sPos(screenPosition.x, screenPosition.y, 0.0f);
		//Unproject the point by applying the inverse
		//of the ViewProjection matrix
		Mat4 inverseViewProjection;
		float det;
		D3DXMatrixInverse(&inverseViewProjection, &det, &_viewProjMatrix);
		Vec4 worldSpacePosition;
		//D3DXVec2Transform(&worldSpacePosition, &screenPosition, &inverseViewProjection);
		D3DXVec3Transform(&worldSpacePosition, &sPos, &inverseViewProjection);
		return Vec3(worldSpacePosition.x, worldSpacePosition.y, worldSpacePosition.z);
	}

	//Load all the textures we need.
	void Graphics::loadAssets()
	{
		//Create the vertex buffer used for sprites
		initGeometry();

		//Load the textures
		//loadTexture("../Assets/Circle.png");
		//loadTexture("../Assets/Square.png");
		//loadTexture("../Assets/CircleOutline.png");
		//loadTexture("../Assets/SquareOutline.png");

		//Load the shaders
		//loadEffect(SID_Basic, "../Shaders/Basic.fx");
		//loadEffect(SID_DebugShader, "../Shaders/Debug.fx");
		
	}

	//Load a specific texture file and add it to the asset texture map
	void Graphics::loadTexture(const char* id, const std::string& filename)
	{
		IDirect3DTexture9* newTexture = NULL;
		FilePath texturefile(filename);

		//Use D3DX to load the texture
		if (SUCCEEDED(D3DXCreateTextureFromFile(_pDevice, texturefile.FullPath.c_str() , &newTexture)))
		{
			std::cout << "Loaded Texture: " << texturefile.FullPath.c_str() << "\n";
			//_textures[texturefile.FileName.c_str()] = newTexture;
			_textures[id] = newTexture;
		}
		else
		{
			std::cout << "Failed to load Texture: " << texturefile.FullPath.c_str() << "\n";
			ErrorIf(true, "Failed to load texture %s in %s", texturefile.FileName.c_str(), texturefile.FullDirectory.c_str());
		}
	}

	IDirect3DTexture9* Graphics::getTexture(const std::string& texture)
	{
		TextureMap::iterator it = _textures.find(texture);
		if(it!= _textures.end())
		{
			return it->second;
		}
		else
		{
			return NULL;
		}
	}

	bool Graphics::loadEffect(const char* id, const std::string& filename)
	{
		//char str[1024];
		//sprintf_s(str, "file name: %s", filename.c_str());
		//MessageBox(0, str, 0, 0);
		std::cout << "Loading Shader: " << filename << "\n";
		FilePath shaderfile(filename);
		//
		HRESULT hr;
		LPD3DXBUFFER pBufferErrors = NULL;

		//Set up the shader flags
		#ifdef _DEBUG
			DWORD shaderFlags  = D3DXSHADER_DEBUG;
		#else
			DWORD shaderFlags  = D3DXSHADER_OPTIMIZATION_LEVEL3;//Max speed shaders
		#endif

		ID3DXEffect* newShader= NULL;

		//Compile the shaders at load time. 
		hr = D3DXCreateEffectFromFile(_pDevice, 
			filename.c_str(),//The shader file to to load
			NULL, //Optional NULL-terminated array of preprocessor macro definitions
			NULL, //Not using include interface
			shaderFlags, //Shader Flags
			NULL, //No shader pool
			&newShader, 
			&pBufferErrors );
		_shaders[id] = newShader;
		if(FAILED(hr))
		{
			//If the shader failed to load signal an error
			if(pBufferErrors)
			{
				LPVOID pCompilErrors = pBufferErrors->GetBufferPointer();
				std::cout<<"FX COMPILE ERROR: "<<std::string((const char*)pCompilErrors)<<"\n";
				ErrorIf(FAILED(hr), "Fx Compile Error: %s", (const char*)pCompilErrors);
			}
			else
			{
				//ErrorIf(FAILED(hr), "Failed to load shader: %s", filename.c_str());
				ErrorIf(FAILED(hr), "Failed to load shader: %s", shaderfile.FullPath.c_str());
			}
			return false;
		}
		return true;
	}

	//Create the vertex buffer that is used for our sprites.
	bool Graphics::initGeometry()
	{
		//Create the vertex buffer.
		if (FAILED(_pDevice->CreateVertexBuffer(4*sizeof(Vertex2D),	//Size of the vertex buffer in bytes.
												 D3DUSAGE_WRITEONLY,					//Gives usage hints to Direct3D.
												 VERTEX2D_FVF,		//The flexible vertex format for this buffer.
												 D3DPOOL_MANAGED,	//Where the buffer will be placed in memory (in this case, let Direct3D decide).
												 &_pQuadVertexBuffer,	//The new vertex buffer.
												 NULL)))			//An unused, reserved parameter.
			return false;

		Vertex2D* pVertices;
		//Lock the vertex buffer so that we can set the vertices.
		//The first parameter is the offset in to the buffer to lock.
		//The second parameter is the amount to lock (0 means lock the whole buffer).
		//The third parameter is a pointer to the actual vertex data.
		//The last parameter is the type of lock (discard all data, read only, etc.).
		if (FAILED(_pQuadVertexBuffer->Lock(0, 0, (void**)&pVertices, 0)))
		{
			return false;
		}

		//Set the four vertices, each of which is the corner of a square.
		//The square is 1 unit long on each side, and centered on 0,0.
		//The order of the vertices is important.
		//The tu and tv values are the x and y coordinates to map the texture to.
		//They range from 0.0 to 1.0, which 1.0 being the height or width of the texture.
		pVertices[0].Position = Vec3(-0.5f, -0.5f, 0.0f);
		pVertices[0].tu       = 0.0f;
		pVertices[0].tv       = 1.0f;
		pVertices[1].Position = Vec3(-0.5f, 0.5f, 0.0f);
		pVertices[1].tu       = 0.0f;
		pVertices[1].tv       = 0.0f;
		pVertices[2].Position = Vec3( 0.5f, -0.5f, 0.0f);
		pVertices[2].tu       = 1.0f;
		pVertices[2].tv       = 1.0f;
		pVertices[3].Position = Vec3( 0.5f, 0.5f, 0.0f);
		pVertices[3].tu       = 1.0f;
		pVertices[3].tv       = 0.0f;

		//Unlock the vertex buffer so we can actually use it.
		_pQuadVertexBuffer->Unlock();

		return true;
	}
	//
	Graphics::SpriteListType* Graphics::getSpriteList()
	{
		return &_spriteList;
	}
	ID3DXEffect* Graphics::getShader(const std::string id)
	{
		//return _shaders[id];
		ShaderMap::iterator it = _shaders.find(id);
		if(it!= _shaders.end())
		{
			return it->second;
		}
		else
		{
			return NULL;
		}
	}
	void Graphics::setMainCamera(Camera* camera)
	{
		_mainCamera = camera;
	}
	Camera* Graphics::getMainCamera()
	{
		return _mainCamera;
	}
	IDirect3DDevice9* Graphics::getD3DDevice()
	{
		return _pDevice;
	}
	IDirect3DVertexBuffer9* Graphics::getQVF()
	{
		return _pQuadVertexBuffer;
	}
	Mat4* Graphics::getViewMatrix()
	{
		return &_viewMatrix;
	}
	void Graphics::setViewMatrix(Mat4& view)
	{
		_viewMatrix = view;
	}
	Mat4* Graphics::getProjectionMatrix()
	{
		return &_projMatrix;
	}
	void Graphics::setProjectionMatrix(Mat4& proj)
	{
		_projMatrix = proj;
	}
	Mat4* Graphics::getViewProjectionMatrix()
	{
		return &_viewProjMatrix;
	}
	void Graphics::setViewProjectionMatrix(Mat4& proj)
	{
		_viewProjMatrix = proj;
	}
	void Graphics::debugShader(const std::string id)
	{
		_debugShader = getShader(id);
	}
	void Graphics::createFont()
	{
		HRESULT hr;
		hr = D3DXCreateFont(_pDevice, 20, 0, FW_NORMAL, 0, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, TEXT("Calibri"), &_pFont);
		ErrorIf(FAILED(hr), "Failed to load font");
	}
	void Graphics::drawText(const char* text, int top, int left, Vec4& color)
	{
		_textBuffer.push_back(new TextData(text, top, left, color));
	}
	void Graphics::renderText(const char* text, int top, int left, Vec4& color)
	{
		D3DCOLOR fontColor = D3DCOLOR_COLORVALUE(color.x, color.y, color.z, color.w);

		// Create a rectangle to indicate where on the screen it should be drawn
		RECT rct;
		rct.top=top;
		rct.left=left;
		rct.right=_screenWidth;
		rct.bottom=_screenHeight;
 
		// Draw some text
		if(!(_pFont->DrawText(NULL, text, -1, &rct, 0, fontColor)))
		{
			std::cout<<"Error drawing text\n";
		}
	}
	void Graphics::renderAllTexts()
	{
		for(std::vector<TextData*>::iterator it1 = _textBuffer.begin(), it2 = _textBuffer.end(); it1 != it2; ++it1)
		{
			renderText((*it1)->text.c_str(), (*it1)->top, (*it1)->left, (*it1)->color);
			delete *it1;
		}
		_textBuffer.clear();
	}
}
