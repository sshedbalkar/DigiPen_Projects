#include "Precompiled.h"
#include <fstream>
#include "Graphics.h"
#include "Message.h"
#include "Core.h"
#include "d3dclass.h"
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
//======================================
const float FrameRateCacPeriod=2.0f;
void Graphics::SetSceneNum( int sc )
{
	m_Text->SetScene( sc, m_D3D->GetDeviceContext() );
}
void Graphics::SetIntegrator( const char *ch )
{
	m_Text->SetIntegrator( ch, m_D3D->GetDeviceContext() );
}
void Graphics::Update( float dt )
{	
	//Calculate Frame Rate
	FrameRateCacAccuTimer+=dt;
	++FrameRateCacAccuCounter;
	if(FrameRateCacAccuTimer>=FrameRateCacPeriod){
		FrameRate= FrameRateCacAccuCounter/FrameRateCacAccuTimer;
		FrameRateCacAccuCounter=0;FrameRateCacAccuTimer=0;
		m_Text->SetFps( FrameRate, m_D3D->GetDeviceContext() );
	}

	if(!m_fadeDone){
		// Update the accumulated time with the extra frame time addition.
		m_accumulatedTime += dt;
		// While the time goes on increase the fade in amount by the time that is passing each frame.
		if(m_accumulatedTime < m_fadeInTime){
			// Calculate the percentage that the screen should be faded in based on the accumulated time.
			m_fadePercentage = m_accumulatedTime / m_fadeInTime;
		}else{
			// If the fade in time is complete then turn off the fade effect and render the scene normally.
			m_fadeDone = true;
			// Set the percentage to 100%.
			m_fadePercentage = 1.0f;
		}
	}

	// Generate the view matrix based on the camera's position.
	m_Camera->Render(dt);

	bool result; 

	if(m_fadeDone){
		// If fading in is complete then render the scene normally using the back buffer.
		result = RenderToTexture(dt);
		if(!result){ return;}
		RenderScene(dt);
	}else{
		// If fading in is not complete then render the scene to a texture and fade that texture in.
		result = RenderToTexture(dt);
		if(!result){ return;}
		result = RenderFadingScene(dt);
		if(!result)	{return;}
	}	
}

bool Graphics::RenderToTexture(float dt){
	D3DXMATRIX worldMatrix, lightViewMatrix, lightProjectionMatrix;
	// Set the render target to be the render to texture.
	m_RenderTexture->SetRenderTarget(m_D3D->GetDeviceContext());
	// Clear the render to texture.
	m_RenderTexture->ClearRenderTarget(m_D3D->GetDeviceContext(), 0.0f, 0.0f, 0.0f, 1.0f);

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_D3D->GetWorldMatrix(worldMatrix);
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);
	// Generate the light view matrix based on the light's position.
	m_Lights[0].GenerateViewMatrix();
	m_Lights[0].GetViewMatrix(lightViewMatrix);
	m_Lights[0].GetProjectionMatrix(lightProjectionMatrix);

	ObjectLinkList<ModelComponent>::iterator it = ModelComponentList.begin();
	for(  ; it!=ModelComponentList.end(); ++it)
	{	
		m_D3D->GetWorldMatrix(worldMatrix);

		Mat4 matTrans, matQuadRotation, matScale, meshMatrix, matModelWorldView;
		D3DXMatrixTranslation(&matTrans, it->transform->Position.x, it->transform->Position.y, it->transform->Position.z);
		D3DXMatrixRotationQuaternion(&matQuadRotation, &it->transform->QUATERNION);
		D3DXMatrixScaling(&matScale, it->transform->Scale.x, it->transform->Scale.y, it->transform->Scale.z);

		meshMatrix = matScale * matQuadRotation * matTrans;
		matModelWorldView = meshMatrix * worldMatrix;

		it->pModel->SetAndDraw( m_D3D->GetDeviceContext() );
		m_DepthShader->Render( m_D3D->GetDeviceContext(), it->pModel->GetIndexCount(), matModelWorldView, lightViewMatrix, lightProjectionMatrix);
	}
	// Reset the render target back to the original back buffer and not the render to texture anymore.
	m_D3D->SetBackBufferRenderTarget();

	// Reset the viewport back to the original.
	m_D3D->ResetViewport();

	return true;
}

bool Graphics::RenderFadingScene(float dt)
{
	D3DXMATRIX worldMatrix, viewMatrix, orthoMatrix;
	bool result;
	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);
	// Get the world, view, and ortho matrices from the camera and d3d objects.
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetOrthoMatrix(orthoMatrix);
	// Turn off the Z buffer to begin all 2D rendering.
	m_D3D->TurnZBufferOff();

	// Put the bitmap vertex and index buffers on the graphics pipeline to prepare them for drawing.
	result = m_RenderTextureBitmap->Render(m_D3D->GetDeviceContext(), 0, 0);
	if(!result){return false;}
	// Render the bitmap using the fade shader.
	result = m_FadeShader->Render(m_D3D->GetDeviceContext(), m_RenderTextureBitmap->GetIndexCount(), worldMatrix, I, orthoMatrix, 
				      m_RenderTexture->GetShaderResourceView(), m_fadePercentage);
	if(!result){return false;}
	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_D3D->TurnZBufferOn();
	// Present the rendered scene to the screen.
	m_D3D->EndScene();
	return true;
}

void Graphics::RenderScene(float dt)
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
	D3DXMATRIX lightViewMatrix, lightProjectionMatrix;
	bool result;

	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);
	//m_D3D->BeginScene(fogColor, fogColor, fogColor, 1.0f);

	// Generate the light view matrix based on the light's position.
	//m_Lights[0].GenerateViewMatrix();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	// Get the light's view and projection matrices from the light object.
	m_Lights[0].GetViewMatrix(lightViewMatrix);
	m_Lights[0].GetProjectionMatrix(lightProjectionMatrix);

	//Uncomment lines beginning with FC to enable frustum culling
	// FC Construct the frustum.
	// FC m_Frustum->ConstructFrustum(SCREEN_DEPTH, projectionMatrix, viewMatrix);
	// FC renderCount = 0;

	ObjectLinkList<ModelComponent>::iterator it = ModelComponentList.begin();
	for(  ; it!=ModelComponentList.end(); ++it)
	{
		// FC Set the radius of the sphere to 1.0 since this is already known.
		// FC radius = sqrt(lengthSquared(it->transform->Scale));
		
		// FC renderModel = m_Frustum->CheckSphere(it->transform->Position.x, it->transform->Position.y, it->transform->Position.z, radius);
		
		//FC if(renderModel)	
		//FC {
			
		it->Draw( m_D3D->GetDeviceContext(), dt, worldMatrix, viewMatrix, projectionMatrix, 
		it->pModel->GetTextureArray(), m_Camera->GetPosition(), m_Lights, lightViewMatrix, 
					lightProjectionMatrix, m_RenderTexture->GetShaderResourceView());
			
		//FC 	renderCount++;
		//FC }
	}
	//FC printf("%d", renderCount);
	// Turn off the Z buffer to begin all 2D rendering.
	if (EDITOR){
		EDITOR->Draw();
		lineDrawer.DrawLineFromList(worldMatrix, viewMatrix, projectionMatrix );
	}


	m_D3D->TurnZBufferOff();

	// CROSSHAIR
	// Get the world, view, and ortho matrices from the camera and d3d objects.
	//m_D3D->GetWorldMatrix(worldMatrix);
	//m_D3D->GetOrthoMatrix(orthoMatrix);
	//m_D3D->TurnOnAlphaBlending();
	//result = m_Bitmap->Render(m_D3D->GetDeviceContext(), screen_width/2.0f-30.0f, screen_height/2.0f-30.0f );
	//if(!result){return;}
	//// Render the bitmap with the texture shader.
	//result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_Bitmap->GetIndexCount(), I, I, orthoMatrix, m_Bitmap->GetTextureArray());
	//if(!result){return;}
	//m_D3D->TurnOffAlphaBlending();


	if(GraphicsDebugOn){
		m_D3D->TurnZBufferOn();
		//// Render the entire scene to the texture first.
		result = RenderToTexture(dt);
		if(!result){return;}
		// RENDER TO TEXTURE WINDOW
		// Get the world, view, and ortho matrices from the camera and d3d objects.
		m_D3D->GetWorldMatrix(worldMatrix);
		m_D3D->GetOrthoMatrix(orthoMatrix);	
		// Put the debug window vertex and index buffers on the graphics pipeline to prepare them for drawing.
		result = m_DebugWindow->Render(m_D3D->GetDeviceContext(), 50, 100);
		if(!result){ return; }
		// Render the debug window using the texture shader.
		result = m_TextureShader->Render(m_D3D->GetDeviceContext(), m_DebugWindow->GetIndexCount(), I, I, orthoMatrix, m_RenderTexture->GetShaderResourceView());
		if(!result){return;}
		m_D3D->TurnZBufferOff();

		
		/*
		// TEXT
		// Generate the view matrix based on the Text camera's position.
		m_D3D->GetOrthoMatrix(orthoMatrix);
		// Turn on the alpha blending before rendering the text.
		m_D3D->TurnOnAlphaBlending();
		// Render the text strings.
		result = m_Text->Render(m_D3D->GetDeviceContext(), I, orthoMatrix);
		if(!result){return;}
		// Turn off alpha blending after rendering the text.
		m_D3D->TurnOffAlphaBlending();
		*/
	}
	// TEXT
	// Generate the view matrix based on the Text camera's position.
	m_D3D->GetOrthoMatrix(orthoMatrix);
	// Turn on the alpha blending before rendering the text.
	m_D3D->TurnOnAlphaBlending();
	// Render the text strings.
	result = m_Text->Render(m_D3D->GetDeviceContext(), I, orthoMatrix);
	if(!result){return;}
	// Turn off alpha blending after rendering the text.
	m_D3D->TurnOffAlphaBlending();
	// Turn the Z buffer back on now that all 2D rendering has completed.
	m_D3D->TurnZBufferOn();
	// Present the rendered scene to the screen.
	m_D3D->EndScene();
}
}
