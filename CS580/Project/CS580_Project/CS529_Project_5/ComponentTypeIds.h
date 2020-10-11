////////////////////////////////////////////////////////////////
//
//	ComponentTypeIds.h
//	Enumerates all the available component types
//
//	Author: Santosh Shedbalkar
//	Copyright 2011, Digipen Institute of Technology
//
////////////////////////////////////////////////////////////////
#ifndef COMPONENT_IDS_H
#define COMPONENT_IDS_H
//#pragma message("Compiling: "__FILE__)
//
namespace Wye
{
	enum ComponentTypeId
	{
//#pragma message("Including Class: ComponentTypeId")
		//Invalid component id
		CT_None = 0,
		//Basic Components
		CT_Transform,
		//Graphics
		CT_Camera,
		CT_Sprite,
		//Physics
		CT_Body,
		//Game Logic
		CT_Controller,
		// Composition
		CT_Component,
		CT_Composition,
		CT_System,
		CT_EventDispatcher,
		CT_Core,
		CT_MovieClip,
		CT_HUD,
		CT_Boid,
		//Max
		CT_MaxComponents
	};
}
#endif