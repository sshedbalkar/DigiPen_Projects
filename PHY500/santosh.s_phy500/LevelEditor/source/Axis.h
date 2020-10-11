#pragma once
#include "Precompiled.h"
#include "Vector3.h"
#include "System.h"
#include "Component.h"

namespace Framework
{

class Axis 
{
	friend class EditorFunctionality;
	friend class LevelEditor;
private:
	enum AxisSelection{x,y,z,none};
	AxisSelection selectedAxis,selectedRotationAxis;
	GOC /**m_selectedGOC,*/*xaxis,*yaxis,*zaxis;
	void CheckRotationaxes();
	bool CheckAxes();
	double magicNumber;
	double magicNumber1;
	double magicNumber2;
public:
	Axis();
	~Axis();
	void Update( float dt );
	void Initialize();
};

}
