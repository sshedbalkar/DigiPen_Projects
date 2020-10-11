////////////////////////////////////////////////////////////////////////////////
// Filename: cameraclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "Precompiled.h"
#include "cameraclass.h"
#include "VMath.h"
#include "WindowsSystem.h"

namespace Framework {

CameraClass * CAMERA=NULL;

const float fowardSpeedC = 40.0f;
const float backwardSpeedC = 40.0f;
const float SideMoveSpeedC = 40.0f;
const float ElevateSpeedC = 7.0f;

void CameraClass::resetCameraSpeed(){
	fowardSpeed = fowardSpeedC;
	backwardSpeed = backwardSpeedC;
	SideMoveSpeed = SideMoveSpeedC;
	ElevateSpeed = ElevateSpeedC;
}

CameraClass::CameraClass() : 
position( D3DXVECTOR3(0.0f, 0.0f, -1.0f) ), 
m_lookAt( D3DXVECTOR3(0.0f, 0.0f, 1.0f) ), 
movingDirection( D3DXVECTOR3(0.0f, 0.0f, 1.0f) ), 
sideDirection( D3DXVECTOR3(0.0f, 0.0f, -1.0f) ), 
bMousing(false), 
DebugCameraMode(false), 
fowardSpeed(fowardSpeedC), 
backwardSpeed(backwardSpeedC), 
SideMoveSpeed(SideMoveSpeedC), 
ElevateSpeed(ElevateSpeedC) 
{
	CAMERA=this;
}

CameraClass::CameraClass(const CameraClass& other)
{
}

CameraClass::~CameraClass()
{
}

void CameraClass::SetPosition(float x, float y, float z)
{
	position.x = x;
	position.y = y;
	position.z = z;
	return;
}

//void CameraClass::SetRotation(float x, float y, float z)
//{
//	m_rotationX = x;
//	m_rotationY = y;
//	m_rotationZ = z;
//	return;
//}

D3DXVECTOR3 CameraClass::GetPosition()
{
	return position;
}

//D3DXVECTOR3 CameraClass::GetRotation()
//{
//	return D3DXVECTOR3(m_rotationX, m_rotationY, m_rotationZ);
//}
//
//const float fowardSpeed = 10.0f;
//const float backwardSpeed = 10.0f;
//const float SideMoveSpeed = 10.0f;
//const float ElevateSpeed = 7.0f;

void CameraClass::Render(float dt)
{
	D3DXVECTOR3 up;
	D3DXVECTOR3 lookAt;
	//float yaw, pitch, roll;
	//D3DXMATRIX rotationMatrix;

	// Setup the position of the camera in the world.
	/*position.x = m_positionX;
	position.y = m_positionY;
	position.z = m_positionZ;*/
	if(DebugCameraMode){
		if(IsWHeld() ) position += fowardSpeed * dt * movingDirection;
		if(IsSHeld() ) position -= backwardSpeed * dt * movingDirection;
		if(IsAHeld() ) position -= SideMoveSpeed * dt * sideDirection;
		if(IsDHeld() ) position += SideMoveSpeed * dt * sideDirection;
		if(IsSpaceHeld() ) position.y += dt * ElevateSpeed;
		if(IsShiftHeld() ) position.y -= dt * ElevateSpeed;
	}
	//else position.y = 0;

	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// Setup where the camera is looking by default.
	/*lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;*/
	lookAt = m_lookAt;

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	//pitch = m_rotationX * 0.0174532925f;
	//yaw   = m_rotationY * 0.0174532925f;
	//roll  = m_rotationZ * 0.0174532925f;

	//// Create the rotation matrix from the yaw, pitch, and roll values.
	//D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);

	//// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	//D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
	//D3DXVec3TransformCoord(&up, &up, &rotationMatrix);

	// Translate the rotated camera position to the location of the viewer.
	lookAt = position + lookAt;

	// Finally create the view matrix from the three updated vectors.
	D3DXMatrixLookAtLH(&m_viewMatrix, &position, &lookAt, &up);

	return;
}

void CameraClass::GetViewMatrix(D3DXMATRIX& viewMatrix)
{
	viewMatrix = m_viewMatrix;
	return;
}
}