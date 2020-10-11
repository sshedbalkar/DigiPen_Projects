#include <iostream>
#include <algorithm>
#include "Precompiled.h"
#include "Camera.h"
#include "GameLogic.h"
#include "ComponentCreator.h"
#include "GameData.h"
#include "LevelLoader.h"
#include "Factory.h"
#include "ComponentTypeIds.h"
#include "MovieClip.h"
#include "DebugDraw.h"
#include "Utility.h"
#include "MessageHandler.h"
#include "Core.h"
#include "Block.h"
#include "AI_1.h"
#include "VMath.h"
#include <time.h>
#include "Bullet.h"
#include "BombEffect.h"
#include "Bomb.h"
#include "Gold.h"
#include "Key.h"
//
namespace Wye
{
	Vec4 getColor(int value, int maxValue)
	{
		float f = (float)value/(float)maxValue;
		if(f > 0.66f)
		{
			return Vec4(0, 1, 0, 1);
		}
		if(f > 0.33f)
		{
			return Vec4(1, 1, 0, 1);
		}
		else
		{
			return Vec4(1, 0, 0, 1);
		}
	}
	//Our global pointer to the game.
	GameLogic* LOGIC = NULL;

	void GameLogic::initialize()
	{
		srand((unsigned)time(NULL));
		//
		Mat4 mat;
		D3DXMatrixTranslation(&mat, 0.0f, -25.0f, 0.0f);
		GRAPHICS->worldTransform(mat);
		//
		//Components are explicitly registered in initialize
		//RegisterComponent(Transform);
		FACTORY->addComponentCreator(CT_Transform, new ComponentCreatorType<Transform>(CT_Transform));
		//This macro expands into FACTORY->AddComponentCreator( "Transform" , new ComponentCreatorType<Transform>()  );

		//RegisterComponent(Controller);
		FACTORY->addComponentCreator(CT_Controller, new ComponentCreatorType<Controller>(CT_Controller));
		FACTORY->addComponentCreator(CT_MovieClip, new ComponentCreatorType<MovieClip>(CT_MovieClip));
		FACTORY->addComponentCreator(CT_Block, new ComponentCreatorType<Block>(CT_MovieClip));
		FACTORY->addComponentCreator(CT_Hero, new ComponentCreatorType<Hero>(CT_Hero));
		FACTORY->addComponentCreator(CT_AI_1, new ComponentCreatorType<AI_1>(CT_AI_1));
		FACTORY->addComponentCreator(CT_Bullet, new ComponentCreatorType<Bullet>(CT_Bullet));
		FACTORY->addComponentCreator(CT_BombEffect, new ComponentCreatorType<BombEffect>(CT_BombEffect));
		FACTORY->addComponentCreator(CT_Bomb, new ComponentCreatorType<Bomb>(CT_Bomb));
		FACTORY->addComponentCreator(CT_Gold, new ComponentCreatorType<Gold>(CT_Gold));
		FACTORY->addComponentCreator(CT_Key, new ComponentCreatorType<Key>(CT_Key));
		//
		_nextLoadLevel = -1;
		playerData.Level = 1;
		playerData.Coins = 0;
		_ended = false;
		_paused = true;
		//
		createDummyCamera();
		//
		bool useLevelFile = true;
		if(useLevelFile)
		{
			loadSettingsFile("../Objects/gameSettings.xml");
			loadLevelI(playerData.Level - 1, true);
		}
		else
		{
			Composition* camera = FACTORY->createEmptyComposition();
			Transform* tr1 = new Transform();
			camera->addComponent(tr1);
			//Controller* ct1 = new Controller();
			//ct1->setSpeed(20.0f);
			//camera->addComponent(ct1);
			Camera* cm = new Camera();
			//GRAPHICS->setMainCamera(cm);
			camera->addComponent(cm);
			tr1->position.z = 10;
			camera->initialize();
			//
			std::cout<<"-----------------\n";
			//
			Composition* gameObject = FACTORY->createEmptyComposition();
			Transform* transform = new Transform();
			transform->position = Vec3(0, 0, 0);
			gameObject->addComponent(transform);
			Sprite* sprite = new Sprite();
			sprite->setSpriteName("square");
			//sprite->Size = Vec2(600,20);
			sprite->setColor(Vec4(1,0,0,1));
			gameObject->addComponent(sprite);
			Controller* ct2 = new Controller();
			ct2->speed(20.0f);
			gameObject->addComponent(ct2);
			gameObject->initialize();
			//
			std::cout<<"-----------------\n";
		}
	}

	GameLogic::GameLogic()
	{	
		//Safe Id reference of the object the user has grabbed
		//GrabbedObjectId = 0;

		//Set up the global pointer
		ErrorIf(LOGIC!=NULL,"Logic already initialized\n");
		LOGIC = this;
	}

	GameLogic::~GameLogic()
	{
		//
	}
	void GameLogic::createDummyCamera()
	{
		Composition* obj = FACTORY->createEmptyComposition();
		Transform* tr = static_cast<Transform*>(FACTORY->createComponent(CT_Transform));
		obj->addComponent(tr);
		tr->position = Vec3(0, 0, -10);
		Component* cam = FACTORY->createComponent(CT_Camera);
		obj->addComponent(cam);
		obj->initialize();
	}
	void GameLogic::drawInfo()
	{
		char str[10];
		int max;
		//
		GRAPHICS->drawText("ESC=Help", 13, 30, Vec4(1, 1, 1, 0.5));
		//
		sprintf_s(str, "LEVEL %d", playerData.Level+1);
		GRAPHICS->drawText(str, 13, 130, Vec4(1, 1, 1, 1));
		//
		sprintf_s(str, "%d", playerData.Coins);
		GRAPHICS->drawText(str, 13, 275, Vec4(1, 1, 0, 1));
		//
		GAMEDATA->readSetting("maxLives", max);
		sprintf_s(str, "%d", playerData.Lives);
		GRAPHICS->drawText(str, 13, 425, getColor(playerData.Lives, max));
		//
		GAMEDATA->readSetting("maxBullets", max);
		sprintf_s(str, "%d", playerData.Bullets);
		GRAPHICS->drawText(str, 13, 575, getColor(playerData.Bullets, max));
		//
		GAMEDATA->readSetting("maxBombs", max);
		sprintf_s(str, "%d", playerData.Bombs);
		GRAPHICS->drawText(str, 13, 725, getColor(playerData.Bombs, max));
	}
	void GameLogic::loadLevel(int level)
	{
		_nextLoadLevel = level;
	}
	void GameLogic::loadLevelI(int level, bool first)
	{
		unloadLevel();
		playerData.Level = level;
		LevelLoader l;
		if(first)
		{
			l.loadAssets();
		}
		l.loadLevel(level);
	}
	void GameLogic::unloadLevel()
	{
		FACTORY->destroyAllObjects();
	}
	void GameLogic::onMessage(const Message& msg)
	{
		//if(_paused) return;
		System::onMessage(msg);
		//
		ControllerListType::iterator it1 = _controllers.begin(), it2 = _controllers.end();
		for(; it1!=it2; ++it1)
		{
			it1->onMessage(msg);
		}
		//
		MessageHandler::getInstance()->handleMessage(msg);
	}
	void GameLogic::update(float dt)
	{
		if(_ended) return;
		currentTime = timeGetTime();
		ControllerListType::iterator it1 = _controllers.begin(), it2 = _controllers.end();
		for(; it1!=it2; ++it1)
		{
			it1->update(dt);
		}
		////////////////////////////////////
		/*
		Vec3 ctr(0.0f, 0.0f, 0.0f);
		Vec3 s1(80.0f, 60.0f, 60.0f);
		float siz = 20.0f;
		Vec4 color(0.3f, 0.8f, 0.2f, 0.9f);
		Drawer::Instance.setColor(color);
		//Drawer::Instance.drawCircle2D(ctr, siz, 20);
		//Drawer::Instance.drawSquare2D(ctr, siz);
		//Drawer::Instance.drawRect3D(ctr, s1);
		Drawer::Instance.drawRect2D(Vec3(0.0f, 0.0f, 0.0f), Vec2 (800.0f, 600.0f));
		*/
		char str[100];
		sprintf_s(str, "FPS: %0.3f", CORE->fps());
		//std::cout<<"FPS: " <<str<<"\n";
		GRAPHICS->drawText(str, -2, 1, Vec4(1,1,1,1));
		//
		drawInfo();
		//
		if(_nextLoadLevel > -1)
		{
			int i;
			GAMEDATA->readSetting("maxLevels", i);
			if(_nextLoadLevel < i)
			{
				loadLevelI(_nextLoadLevel, false);
				_nextLoadLevel = -1;
			}
			else
			{
				unloadLevel();
				createDummyCamera();
				createObjectAt("a_17", Vec3(0, 0, 0));
				_nextLoadLevel = -1;
				_ended = true;
			}
		}
	}

	void GameLogic::loadSettingsFile(const char* filename)
	{
		GAMEDATA->loadLevelData(filename);
		int fps=0;
		GAMEDATA->readSetting("fps", fps);
		GAMEDATA->readSetting("maxLives", playerData.Lives);
		GAMEDATA->readSetting("maxBullets", playerData.Bullets);
		GAMEDATA->readSetting("maxBombs", playerData.Bombs);
		CORE->fps(fps);
	}

	void GameLogic::addComponent(Controller* controller)
	{
		_controllers.push_back(controller);
		//System::addComponent(controller);
	}
	void GameLogic::removeComponent(Controller* controller)
	{
		_controllers.erase(controller);
		//System::removeComponent(controller);
	}
	Composition* GameLogic::createObjectAt(const char* archId, Vec3& position, Vec3& rotation, Vec3& scale)
	{
		//This is an example of using some data out of data file using
		//serialization and then override fields before the object
		//is initialized.

		//Build and serialize the object with data from the file
		Composition* newObject = FACTORY->buildFromArchetype(archId);
		//Get the transform and adjust the position
		Transform* transform = newObject->getComponent<Transform>(CT_Transform);
		transform->position = position;
		transform->rotation = rotation;
		transform->scale = scale;
		std::cout<<"New Obj Created: Arch: "<<archId<<", Pos: "<<position<<", Rot: " <<rotation<<", Scale: " << scale<<"\n";
		//Initialize the composition
		newObject->initialize();

		return newObject;
	}
	Composition* GameLogic::createObjectAt(const char* archId, Vec3& position, Vec3& rotation)
	{
		return createObjectAt(archId, position, rotation, Vec3(1.0f, 1.0f, 1.0f));
	}
	Composition* GameLogic::createObjectAt(const char* archId, Vec3& position)
	{
		return createObjectAt(archId, position, Vec3(0.0f, 0.0f, 0.0f), Vec3(1.0f, 1.0f, 1.0f));
	}
	void GameLogic::hero(Hero* hr)
	{
		_hero = hr;
	}
	Hero* GameLogic::hero()
	{
		return _hero;
	}
}