#include "Containers.h"
#include "MessageHandler.h"
#include "WindowsSystem.h"
#include "DebugTools.h"
#include "Core.h"
#include "Physics.h"
#include "Graphics.h"
#include "GameLogic.h"
#include "Sprite.h"
#include "ComponentTypeIds.h"
#include "Composition.h"
//
namespace Wye
{
	MessageHandler* MessageHandler::_instance = 0;
	//
	MessageHandler::MessageHandler()
		:Object(),
		_debug(false),
		_showSettings(false),
		_settings(NULL)
	{
		Composition* comp = LOGIC->createObjectAt("a_14", Vec3(-400, 300, 0));
		_settings = comp->getComponent<Sprite>(CT_Sprite);
		//_settings->setSpriteName("t_35");
		//ToggleFrameByFrameMode fbf;
		//CORE->onMessage(fbf);
	}
	MessageHandler::~MessageHandler()
	{
		//
	}
	void MessageHandler::handleMessage(const Message& msg)
	{
		switch(msg.MessageId)
		{
			// The user has pressed a (letter/number) key, we may respond by creating
			// a specific object based on the key pressed.
			case Msgid::CharacterKey:
				{
					// Cast to the derived message type
					MessageCharacterKey* key = (MessageCharacterKey*)&msg;
					//When different keys are pressed create different objects
					std::string ObjectToCreate;
					
					if(key->character == '1')
						ObjectToCreate = "a_5";		//Ball
					else if(key->character == '2')
						ObjectToCreate = "a_6";		//Square
					//else if(key->character == '3')
						//ObjectToCreate = "Objects\\Bomb.txt";

					if(!ObjectToCreate.empty())
					{
						Vec3 p(LOGIC->WorldMousePosition.x, LOGIC->WorldMousePosition.y, 0.0f);
						LOGIC->createObjectAt(ObjectToCreate.c_str(), p);
					}
					
					//Enable Physics Debugging
					if(key->character == VK_OEM_PERIOD)
					{
						if(isShiftDown())
						{
							PHYSICS->StepModeActive = !PHYSICS->StepModeActive;
						}
						PHYSICS->AdvanceStep = true;
					}

					if(key->character == 'd')
					{
						_debug = !_debug;
						Debug::DebugTools::getInstance()->enable(_debug);
						ToggleDebugDisplay debugMessage(_debug);
						CORE->onMessage(debugMessage);
					}
					if(key->character == 'q')
					{
						PostQuitMessage(0);
					}
					if(key->character == 'a')
					{
						LOGIC->loadLevel(LOGIC->playerData.Level + 1);
					}
					else if(key->character == VK_F1)
					{
						ToggleFrameByFrameMode fbf;
						CORE->onMessage(fbf);
					}
					else if(key->character == VK_F3)
					{
						AdvanceFrame af(1);
						CORE->onMessage(af);
					}
					else if(key->character == VK_ESCAPE)
					{
						_showSettings = !_showSettings;
						_settings->setSpriteName(_showSettings ? "t_22" : "t_35");
					}
					break;
				}
			case Msgid::FileDrop:
				{
					//FileDrop * drop = (FileDrop*)msg;
					//LoadLevelFile(drop->FileName);
					break;
				}
			case Msgid::MouseMove:
				{
					//The mouse has moved, update the mouse's world position
					MouseMove* mouseMove = (MouseMove*)&msg;
					LOGIC->WorldMousePosition = GRAPHICS->screenToWorldSpace(mouseMove->MousePosition);
					break;
				}
			case Msgid::MouseButton:
				{
					MouseButton* mouse = (MouseButton*)&msg;
					//Update the world mouse position
					LOGIC->WorldMousePosition = GRAPHICS->screenToWorldSpace(mouse->MousePosition);
					
					if(mouse->ButtonIsPressed)
					{
						/*
						if( mouse->MouseButtonIndex == MouseButton::LeftMouse )
						{
							//On left click attempt to grad a object at the mouse cursor
							if( GOC * goc = PHYSICS->TestPoint( LOGIC->WorldMousePosition ) )
								GrabbedObjectId = goc->GetId();
						}
						else if( mouse->MouseButtonIndex == MouseButton::RightMouse )
						{
							//On right click destroy the object at the mouse cursor
							GOC * goc = PHYSICS->TestPoint( LOGIC->WorldMousePosition );
							if( goc ) 
								goc->Destroy();
						}
						*/
					}
					else
					{
						//If the mouse has been release let go of the grabbed object
						//GrabbedObjectId = 0;
					}
					break;
				}
		}
	}
}