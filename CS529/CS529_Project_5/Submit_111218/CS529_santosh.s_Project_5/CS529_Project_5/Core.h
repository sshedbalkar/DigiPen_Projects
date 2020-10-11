///////////////////////////////////////////////////////////////////////////////////////
///
///	\file Core.h
///	Defines the CoreEngine.
///	Authors:  Chris Peters
///	Copyright 2010, Digipen Institute of Technology
///
///////////////////////////////////////////////////////////////////////////////////////
#ifndef CORE_H
#define CORE_H
//#pragma message("Compiling: "__FILE__)
//
#include "Composition.h"
#include "System.h"
//
namespace Wye
{
	class WindowsSystem;
	class Component;
	///The core manages all the systems in the game. Updating them, routing messages, and
	///destroying them when the game ends.
	class CoreEngine: public Composition
	{
#pragma message("Including Class: CoreEngine")
	public:
		CoreEngine();
		~CoreEngine();
		///Update all the systems until the game is no longer active.
		void gameLoop();
		///Destroy all systems in reverse order that they were added.
		void destroySystems();
		///Broadcasts a message to all systems.
		void onMessage(const Message& msg);
		///Adds a new system to the game.
		void addSystem(System* system);
		///Initializes all systems in the game.
		void initialize();
		float fps();
		void fps(int f);
		void registerForUpdateMsg(Component* comp);
		void unregisterForUpdateMsg(Component* comp);
	private:
		CoreEngine(const CoreEngine& orig);
		void sendEnterFrameMsg(float dt);
		//Tracks all the systems the game uses
		std::vector<System*> _systems;
		std::vector<Component*> _components;
		//The last time the game was updated
		unsigned _lastTime;
		//Is the game running (true) or being shut down (false)?
		bool _gameActive;
		//
		int _fps;
		float _dt;
		bool _frameMode;
		unsigned _advanceFrame;
		WindowsSystem* _winSys;
		unsigned _frame;
	};

	///Message to tell the game to quit
	class MessageQuit: public Message
	{
	public:
		MessageQuit(): Message(Msgid::Quit) {};
	};
	//
	class ToggleFrameByFrameMode: public Message
	{
	public:
		ToggleFrameByFrameMode(): Message(Msgid::FrameByFrame){};
	};
	//
	class AdvanceFrame: public Message
	{
	public:
		unsigned count;
		AdvanceFrame(unsigned frames): Message(Msgid::AdvanceFrame), count(frames){};
	};
	///Signals all systems to activate or deactivate the display of debug data.
	class ToggleDebugDisplay: public Message
	{
	public:
		bool debugActive;
		ToggleDebugDisplay(bool dbugActive)
			: Message(Msgid::ToggleDebugInfo), debugActive(dbugActive) {};
	};
	//
	class EnterFrameMsg: public Message
	{
	public:
		friend class CoreEngine;
		unsigned Frame;
		float dt;
	private:
		EnterFrameMsg(unsigned id, float d);
		~EnterFrameMsg();
	};

	//A global pointer to the instance of the core
	extern CoreEngine* CORE;
}
#endif