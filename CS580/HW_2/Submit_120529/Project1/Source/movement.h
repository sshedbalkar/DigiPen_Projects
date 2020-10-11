/* Copyright Steve Rabin, 2012. 
 * All rights reserved worldwide.
 *
 * This software is provided "as is" without express or implied
 * warranties. You may freely copy and compile this source into
 * applications you distribute provided that the copyright text
 * below is included in the resulting source code, for example:
 * "Portions Copyright Steve Rabin, 2012"
 */

#pragma once

class GameObject;


class Movement
{
public:
	Movement( GameObject& owner );
	~Movement( void );

	inline void SetTarget( D3DXVECTOR3& target )			{ m_target = target; }
	inline D3DXVECTOR3& GetTarget( void )					{ return( m_target ); }

	void Animate( double dTimeDelta );

	void SetIdleSpeed( void );
	void SetWalkSpeed( float scale = 1.0f );
	void SetJogSpeed( float scale = 1.0f );

protected:

	GameObject* m_owner;

	D3DXVECTOR3 m_target;
	float m_speedWalk;
	float m_speedJog;

};