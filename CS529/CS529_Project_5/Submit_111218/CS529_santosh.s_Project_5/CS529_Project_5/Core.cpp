///////////////////////////////////////////////////////////////////////////////////////
//
//	Core.cpp
//	Authors: Chris Peters
//	Copyright 2010, Digipen Institute of Technology
//
///////////////////////////////////////////////////////////////////////////////////////
//#pragma message("Compiling: "__FILE__)
//
#include "Core.h"
#include "ComponentTypeIds.h"
#include <vector>
#include "DebugTools.h"
#include "WindowsSystem.h"
//
namespace Wye
{

	//A global pointer to the core
	CoreEngine* CORE;

	CoreEngine::CoreEngine():
	Composition(),
		_lastTime(0),
		_gameActive(true),
		_fps(0),
		_dt(0.0f),
		_frameMode(false),
		_advanceFrame(0),
		_frame(0)
	{
		setParent(NULL);
		setType(CT_Core);
		CORE = this; //Set the global pointer
		//
		_fps = 30;
	}

	CoreEngine::~CoreEngine()
	{
		//Nothing for the destructor to do
	}
	CoreEngine::CoreEngine(const CoreEngine& orig)
	{
		//
	}
	void CoreEngine::initialize()
	{
		std::vector<System*>::iterator it1 = _systems.begin(), it2 = _systems.end();
		while (it1 != it2)
		{
			(*it1) -> initialize();
			++it1;
		}
	}

	void CoreEngine::gameLoop()
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
					sendEnterFrameMsg(cdt);
					//
					std::vector<System*>::iterator it1 = _systems.begin(), it2 = _systems.end();
					while (it1 != it2)
					{
						(*it1) -> update(1.0f/_fps);
						++it1;
					}
					++_frame;
				}
				else
				{
					_winSys->update(1.0f/_fps);
				}
			}
			else
			{
				//std::cout<<"CURRENT FRAME: "<<_frame<<"\n";

				sendEnterFrameMsg(_dt);
				//Get the current time in milliseconds
			
				//Convert it to the time passed since the last frame (in seconds)
				//Update the when the last update started

				//Update every system and tell each one how much
				//time has passed since the last update
				std::vector<System*>::iterator it1 = _systems.begin(), it2 = _systems.end();
				while (it1 != it2)
				{
					(*it1) -> update(_dt);
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
	void CoreEngine::sendEnterFrameMsg(float dt)
	{
		EnterFrameMsg m(_frame, dt);
		std::vector<Component*>::iterator it1 = _components.begin(), it2 = _components.end();
		for(; it1 != it2; ++it1)
		{
			(*it1)->onMessage(m);
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
	void CoreEngine::onMessage(const Message& msg)
	{
		//The message that tells the game to quit
		if(msg.MessageId == Msgid::Quit)
		{
			_gameActive = false;
		}
		if(msg.MessageId == Msgid::FrameByFrame)
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
		if(msg.MessageId == Msgid::AdvanceFrame)
		{
			const AdvanceFrame *m = static_cast<const AdvanceFrame*>(&msg);
			_advanceFrame = m->count;
		}
		//Send the message to every system--each
		//system can figure out whether it cares
		//about a given message or not
		std::vector<System*>::iterator it1 = _systems.begin(), it2 = _systems.end();
		while (it1 != it2)
		{
			(*it1) -> onMessage(msg);
			++it1;
		}
	}
	void CoreEngine::registerForUpdateMsg(Component* comp)
	{
		std::vector<Component*>::iterator it1 = _components.begin(), it2 = _components.end();
		for(; it1 != it2; ++it1)
		{
			if(*it1 == comp)
			{
				break;
			}
		}
		if(it1 == it2)
		{
			_components.push_back(comp);
		}
	}
	void CoreEngine::unregisterForUpdateMsg(Component* comp)
	{
		std::vector<Component*>::iterator it1 = _components.begin(), it2 = _components.end();
		for(; it1 != it2; ++it1)
		{
			if(*it1 == comp)
			{
				_components.erase(it1);
				break;
			}
		}
	}
	void CoreEngine::addSystem(System* system)
	{
		//Add a system to the core to be updated
		//every frame
		system -> setParent(this);
		_systems.push_back(system);
		//
		WindowsSystem* sys;
		if(sys = dynamic_cast<WindowsSystem*>(system))
		{
			_winSys = sys;
		}
	}

	void CoreEngine::destroySystems()
	{
		//Delete all the systems in reverse order
		//that they were added in (to minimize any
		//dependency problems between systems)
		for (unsigned i = 0; i < _systems.size(); ++i)
		{
			delete _systems[_systems.size()-i-1];
		}
	}
	///////////////////////////////////////////
	EnterFrameMsg::EnterFrameMsg(unsigned id, float d)
		:Message(Msgid::EnterFrame),
		Frame(id),
		dt(d)
	{
		//
	}
	EnterFrameMsg::~EnterFrameMsg()
	{
		//
	}
}
