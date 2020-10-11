#include "Precompiled.h"
#include <fstream>
#include "Graphics.h"
#include "Message.h"
#include "Core.h"
#include "d3dclass.h"
#include "ModelClass.h"
#include "FilePath.h"
#include "cameraclass.h"
#include "WindowsSystem.h"
//component registration
#include "Factory.h"
#include "ComponentCreator.h"
#include "ModelComponent.h"
#include "lightshaderclass.h"
#include "lightclass.h"
#include "colorshaderclass.h"
#include "bitmapclass.h"
#include "AnimateShaderClass.h"
#include "FrustumClass.h"
#include "multitextureshaderclass.h"
#include "lightmapshaderclass.h"
#include "alphamapshaderclass.h"
#include "bumpmapshaderclass.h"
#include "specmapshaderclass.h"
#include "rendertextureclass.h"
#include "debugwindowclass.h"
#include "fogshaderclass.h"
#include "clipplaneshaderclass.h"
#include "transparentshaderclass.h"
#include "rendertexturebitmapclass.h"
#include "fadeshaderclass.h"
#include "textclass.h"
#include "LevelEditor.h"
#include "GraphicsAssetsLoader.h"
#include "textureshaderclass.h"
#include "VMath.h"
#include "texturearrayclass.h"
#include "depthshaderclass.h"

namespace Framework
{
Graphics* GRAPHICS = NULL;
//======================================
Graphics::Graphics() : 
m_D3D(NULL), 
m_Camera(NULL),
m_AnimateShader(NULL),
m_LightShader(NULL),
m_Bitmap(NULL), 
m_ColorShader(NULL),
m_Frustum(NULL),
m_MultiTextureShader(NULL),
m_LightMapShader(NULL),
m_AlphaMapShader(NULL),
m_BumpMapShader(NULL),
WireFrame(false),
GraphicsDebugOn(false), 
FrameRateCacAccuCounter(0), 
FrameRateCacAccuTimer(0.0f), 
FrameRate(9999.0f), 
m_SpecMapShader(NULL),
m_RenderTexture(NULL),
m_DebugWindow(NULL),
m_FogShader(NULL),
m_ClipPlaneShader(NULL),
m_Text(NULL),
m_TransparentShader(NULL),
m_FadeShader(NULL),
m_RenderTextureBitmap(NULL),
m_TextureShader(NULL),
m_DepthShader(NULL)
{
	GRAPHICS = this;
}
//======================================
void Graphics::Initialize()
{
	printf("Initializing graphics..\n");
	//@@
	const bool FULL_SCREEN = false;
	const bool VSYNC_ENABLED = true;
	const float SCREEN_DEPTH = 10000.0f;
	const float SCREEN_NEAR = 0.1f;

	bool result;
	// Create the Direct3D object.
	m_D3D = new D3DClass();
	if(!m_D3D){
		return ;
	}
	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screen_width, screen_height, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result){
		MessageBox(hwnd, "Could not initialize Direct3D.", "Error", MB_OK);
		return ;
	}
	// Create the camera object.
	m_Camera = new CameraClass;
	if(!m_Camera){ return; }
	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, 0.0f, -2.0f);
	SetCursorPos( static_cast<int>(screen_width/2.0f), static_cast<int>(screen_height/2.0f) );

	// Create an orthographic projection matrix for 2D rendering.
	D3DXVECTOR3 eye( 0.0f, 0.0f, -10.0f );
	D3DXVECTOR3 at( 0.0f, 0.0f, 0.0f );
	D3DXVECTOR3 up( 0.0f, 1.0f, 0.0f );
	D3DXMATRIX matview;
	D3DXMatrixLookAtLH( &matview, &eye, &at, &up );
	m_Text = new TextClass;
	if(!m_Text){ return; }
	// Initialize the text object.
	result = m_Text->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, screen_width, screen_height, matview);
	if(!result){
		MessageBox(hwnd, "Could not initialize the text object.", "Error", MB_OK);
		return;
	}

	// Create the bitmap object.
	m_Bitmap = new BitmapClass;
	if(!m_Bitmap){ return; }
	// Initialize the bitmap object.
	result = m_Bitmap->Initialize(m_D3D->GetDevice(), screen_width, screen_height, "Assets//textures//crosshair.png", 55, 62.5);
	if(!result){
		MessageBox(hwnd, "Could not initialize the bitmap object.", "Error", MB_OK);
		return;
	}

	//Create the animate shader object
	m_AnimateShader = new AnimateShaderClass;
	if(!m_AnimateShader){ return; }
	// Initialize the animate shader object.
	result = m_AnimateShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result){
		MessageBox(hwnd, "Could not initialize the animate shader object.", "Error", MB_OK);
		return;
	}
	//Create the color shader object
	m_ColorShader = new ColorShaderClass;
	if(!m_ColorShader){ return; }
	// Initialize the animate shader object.
	result = m_ColorShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result){
		MessageBox(hwnd, "Could not initialize the animate shader object.", "Error", MB_OK);
		return;
	}

	// Create the frustum object.
	m_Frustum = new FrustumClass;
	if(!m_Frustum){ return; }

	// Create the multitexture shader object.
	m_MultiTextureShader = new MultiTextureShaderClass;
	if(!m_MultiTextureShader){ return; }
	// Initialize the multitexture shader object.
	result = m_MultiTextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result) {
		MessageBox(hwnd, "Could not initialize the multitexture shader object.", "Error", MB_OK);
		return;
	}

	// Create the multitexture shader object.
	m_TextureShader = new TextureShaderClass;
	if(!m_TextureShader){ return; }
	// Initialize the multitexture shader object.
	result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result) {
		MessageBox(hwnd, "Could not initialize the texture shader object.", "Error", MB_OK);
		return;
	}

	// Create the light map shader object.
	m_LightMapShader = new LightMapShaderClass;
	if(!m_LightMapShader) { return; }
	// Initialize the light map shader object.
	result = m_LightMapShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result){
		MessageBox(hwnd, "Could not initialize the light map shader object.", "Error", MB_OK);
		return;
	}

	// Create the alpha map shader object.
	m_AlphaMapShader = new AlphaMapShaderClass;
	if(!m_AlphaMapShader){return;}
	// Initialize the alpha map shader object.
	result = m_AlphaMapShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result){
		MessageBox(hwnd, "Could not initialize the alpha map shader object.", "Error", MB_OK);
		return;
	}
	// Create the bump map shader object.
	m_BumpMapShader = new BumpMapShaderClass;
	if(!m_BumpMapShader){return;}
	// Initialize the bump map shader object.
	result = m_BumpMapShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result){
		MessageBox(hwnd, "Could not initialize the bump map shader object.", "Error", MB_OK);
		return;
	}

	// Create the render to texture object.
	m_RenderTexture = new RenderTextureClass;
	if(!m_RenderTexture){return;}
	// Initialize the render to texture object.
	result = m_RenderTexture->Initialize(m_D3D->GetDevice(), SHADOWMAP_WIDTH, SHADOWMAP_HEIGHT, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result){return;}

	// Create the debug window object.
	m_DebugWindow = new DebugWindowClass;
	if(!m_DebugWindow){return;}
	// Initialize the debug window object.
	result = m_DebugWindow->Initialize(m_D3D->GetDevice(), screen_width, screen_height, 100, 100);
	if(!result){
		MessageBox(hwnd, "Could not initialize the debug window object.", "Error", MB_OK);
		return;
	}

	// Create the depth shader object.
	m_DepthShader = new DepthShaderClass;
	if(!m_DepthShader)
	{
		return;
	}

	// Initialize the depth shader object.
	result = m_DepthShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, "Could not initialize the depth shader object.", "Error", MB_OK);
		return;
	}

	// Create the fog shader object.
	m_FogShader = new FogShaderClass;
	if(!m_FogShader){return;}
	// Initialize the fog shader object.
	result = m_FogShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result){
		MessageBox(hwnd, "Could not initialize the fog shader object.", "Error", MB_OK);
		return;
	}

	// Create the clip plane shader object.
	m_ClipPlaneShader = new ClipPlaneShaderClass;
	if(!m_ClipPlaneShader){return;}
	// Initialize the clip plane shader object.
	result = m_ClipPlaneShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result){
		MessageBox(hwnd, "Could not initialize the clip plane shader object.", "Error", MB_OK);
		return;
	}

	// Create the transparent shader object.
	m_TransparentShader = new TransparentShaderClass;
	if(!m_TransparentShader)
	{
		return;
	}

	// Initialize the transparent shader object.
	result = m_TransparentShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, "Could not initialize the transparent shader object.", "Error", MB_OK);
		return;
	}

	// Create the bitmap object.
	m_RenderTextureBitmap = new RenderTextureBitmapClass;
	if(!m_RenderTextureBitmap){return;}
	// Initialize the bitmap object.
	result = m_RenderTextureBitmap->Initialize(m_D3D->GetDevice(), screen_width, screen_height, screen_width, screen_height);
	if(!result)
	{
		MessageBox(hwnd, "Could not initialize the bitmap object.", "Error", MB_OK);
		return;
	}

	// Create the fade shader object.
	m_FadeShader = new FadeShaderClass;
	if(!m_FadeShader)
	{
		return;
	}

	// Initialize the fade shader object.
	result = m_FadeShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result)
	{
		MessageBox(hwnd, "Could not initialize the fade shader object.", "Error", MB_OK);
		return;
	}

	// Create the light shader object.
	m_LightShader = new LightShaderClass;
	if(!m_LightShader){return;}
	// Initialize the light shader object.
	result = m_LightShader->Initialize(m_D3D->GetDevice(), hwnd);
	if(!result){
		MessageBox(hwnd,"Could not initialize the light shader object.","Error", MB_OK);
		return;
	}

	// Create the light object.
	LightClass Directionlight;
	// Initialize the light object.
	m_Lights.push_back(Directionlight);
	m_Lights[0].SetAmbientColor(0.4f,0.4f, 0.4f, 1.0f);
	m_Lights[0].SetDiffuseColor(0.0f, 0.0f, 1.0f, 1.0f);
	m_Lights[0].SetDirection(0.0f, -1.0f, 0.0f);
	m_Lights[0].SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Lights[0].SetSpecularPower(50.0f);
	m_Lights[0].SetPosition(0.0f, 1.5f, 0.0f);
	m_Lights[0].SetLookAt(0.0f, 0.0f, 0.0f);
	m_Lights[0].GenerateProjectionMatrix(SCREEN_DEPTH, SCREEN_NEAR);

	m_Lights.push_back(Directionlight);
	m_Lights[1].SetAmbientColor(0.4f,0.4f, 0.4f, 1.0f);
	m_Lights[1].SetDiffuseColor(0.0f, 1.0f, 0.0f, 1.0f);
	m_Lights[1].SetDirection(0.0f, -1.0f, 0.0f);
	m_Lights[1].SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Lights[1].SetSpecularPower(50.0f);
	m_Lights[1].SetPosition(0.0f, 2.0f, 24.5f);
	//m_Lights[1].SetLookAt(0.0f, 0.0f, 0.0f);
	//m_Lights[1].GenerateProjectionMatrix(SCREEN_DEPTH, SCREEN_NEAR);

	m_Lights.push_back(Directionlight);
	m_Lights[2].SetAmbientColor(0.4f,0.4f, 0.4f, 1.0f);
	m_Lights[2].SetDiffuseColor(1.0f, 0.0f, 0.0f, 1.0f);
	m_Lights[2].SetDirection(0.0f, -1.0f, 0.0f);
	m_Lights[2].SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Lights[2].SetSpecularPower(50.0f);
	m_Lights[2].SetPosition(0.0f, -1.25f, 12.0f);
	//m_Lights[2].SetLookAt(0.0f, 0.0f, 0.0f);
	//m_Lights[2].GenerateProjectionMatrix(SCREEN_DEPTH, SCREEN_NEAR);

	m_Lights.push_back(Directionlight);
	m_Lights[3].SetAmbientColor(0.4f,0.4f, 0.4f, 1.0f);
	m_Lights[3].SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Lights[3].SetDirection(0.0f, -1.0f, 0.0f);
	m_Lights[3].SetSpecularColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Lights[3].SetSpecularPower(50.0f);
	m_Lights[3].SetPosition(0.0f, 1.0f, 0.0f);
	//m_Lights[3].SetLookAt(0.0f, 0.0f, 0.0f);
	//m_Lights[3].GenerateProjectionMatrix(SCREEN_DEPTH, SCREEN_NEAR);

	//Set I to Identity Matrix
	D3DXMatrixIdentity(&I);
	// Setup a clipping plane.
	// vec4(xaxis, yaxis, zaxis, height of cull)
	clipPlane = D3DXVECTOR4(0.0f, 1.0f, 0.0f, -20.0f);

	// Set the color of the fog to grey.
	fogColor = 0.5f;
	// Set the start and end of the fog.
	fogStart = 0.0f;
	fogEnd = 10.0f;

	// Set the blending amount to 50%.
	blendAmount = 0.50f;

	// Set the fade in time to 3000 milliseconds.
	m_fadeInTime = 0.1f;

	// Initialize the accumulated time to zero milliseconds.
	m_accumulatedTime = 0;

	// Initialize the fade percentage to zero at first so the scene is black.
	m_fadePercentage = 0;

	// Set the fading in effect to not done.
	m_fadeDone = false;


	GraphicsAssetsLoader loader;
	loader.Initialize();

	vector<ModelInfo*> myvec= loader.GetModelInfoVector();
	vector<ModelInfo*>::iterator iter;
	for (iter = myvec.begin(); iter != myvec.end(); ++iter){
		LoadModelToMap(*iter ); 
	}

	vector<ModelInfo*> myvec2= loader.GetModel2InfoVector();
	vector<ModelInfo*>::iterator iter2;
	for (iter2 = myvec2.begin(); iter2 != myvec2.end(); ++iter2){
		LoadModelToMap2(*iter2); 
	}
	LoadGlobalTexture("Assets/textures/white.png", "", "");

	RegisterComponent(Transform);
	RegisterComponent(ModelComponent);
}

}
