///////////////////////////////////////////////////////////////////////////////////////
//
//	Core.cpp
//	Authors: Chris Peters
//	Copyright 2010, Digipen Institute of Technology
//
///////////////////////////////////////////////////////////////////////////////////////

#include "Precompiled.h"
#include "Core.h"
#include "DebugTools.h"
#include "WindowsSystem.h"

namespace Framework
{

	//A global pointer to the core
	CoreEngine* CORE;

	CoreEngine::CoreEngine():
		_lastTime(0),
		_gameActive(true),
		_fps(3000),
		_dt(0.0f),
		_frameMode(false),
		_advanceFrame(0),
		_frame(0)
	{
		CORE = this; //Set the global pointer
	}

	CoreEngine::~CoreEngine()
	{
		//Nothing for the destructor to do
	}

	void CoreEngine::Initialize()
	{
		std::vector<ISystem*>::iterator it1 = Systems.begin(), it2 = Systems.end();
		while (it1 != it2)
		{
			(*it1) -> Initialize();
			++it1;
		}
	}

	void CoreEngine::GameLoop()
	{
		//Initialize the last time variable so our first frame
		//is "zero" seconds (and not some huge unknown number)
		_lastTime = timeGetTime();

		while (_gameActive)
		{
			if(_frameMode)
			{
				_lastTime =  timeGetTime();
				if(_advanceFrame > 0)
				{
					std::cout<<"Advancing to next frame\n";
					--_advanceFrame;
					float cdt = 1.0f/_fps;
					//
					std::vector<ISystem*>::iterator it1 = Systems.begin(), it2 = Systems.end();
					while (it1 != it2)
					{
						(*it1) -> Update(1.0f/_fps);
						++it1;
					}
					++_frame;
				}
				else
				{
					_winSys->Update(1.0f/_fps);
				}
			}
			else
			{
				//std::cout<<"CURRENT FRAME: "<<_frame<<"\n";

				//sendEnterFrameMsg(_dt);
				//Get the current time in milliseconds
			
				//Convert it to the time passed since the last frame (in seconds)
				//Update the when the last update started

				//Update every system and tell each one how much
				//time has passed since the last update
				std::vector<ISystem*>::iterator it1 = Systems.begin(), it2 = Systems.end();
				while (it1 != it2)
				{
					(*it1) -> Update( _dt > 0.031f? 0.031f : _dt );
					++it1;
				}
				//
				unsigned currenttime;
				float dt;
				do
				{
					currenttime = timeGetTime();
					dt = (currenttime - _lastTime) / 1000.0f;
				} while(dt < (1.0f/_fps));
				_dt = dt;
				_lastTime = currenttime;
				++_frame;
			}
		}
	}
	float CoreEngine::fps()
	{
		return 1.0f/_dt;
	}
	void CoreEngine::fps(int f)
	{
		_fps = f;
	}
	void CoreEngine::BroadcastMessage(Message* message)
	{
		//The message that tells the game to quit
		if (message->MessageId == Mid::Quit)
		{
			_gameActive = false;
		}
		if(message->MessageId == Mid::FrameByFrame)
		{
			_frameMode = !_frameMode;
			if(_frameMode)
			{
				std::cout<<"Entering Frame by Frame mode\n";
			}
			else
			{
				std::cout<<"Exiting Frame by Frame mode\n";
			}
		}
		if(message->MessageId == Mid::AdvanceFrame)
		{
			const AdvanceFrame *m = static_cast<const AdvanceFrame*>(message);
			_advanceFrame = m->count;
		}
		//Send the message to every system--each
		//system can figure out whether it cares
		//about a given message or not
		std::vector<ISystem*>::iterator it1 = Systems.begin(), it2 = Systems.end();
		while (it1 != it2)
		{
			(*it1) -> SendMessage(message);
			++it1;
		}
	}

	void CoreEngine::AddSystem(ISystem* system)
	{
		//Add a system to the core to be updated
		//every frame
		Systems.push_back(system);
		//
		WindowsSystem* sys;
		if(sys = dynamic_cast<WindowsSystem*>(system))
		{
			_winSys = sys;
		}
	}

	void CoreEngine::DestroySystems()
	{
		//Delete all the systems in reverse order
		//that they were added in (to minimize any
		//dependency problems between systems)
		for (unsigned i = 0; i < Systems.size(); ++i)
		{
			delete Systems[Systems.size()-i-1];
		}
	}

}
