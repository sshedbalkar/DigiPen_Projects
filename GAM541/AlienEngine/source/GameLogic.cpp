///////////////////////////////////////////////////////////////////////////////////////
//
//	Game Logic
//	Simple game logic for demo. A good way to approach understand the game sample
//	is to walk through all the code in game logic and follow the execution through
//	the rest of the engine.
//	
//	Authors: Chris Peters, Benjamin Ellinger
//	Copyright 2010, Digipen Institute of Technology
//
///////////////////////////////////////////////////////////////////////////////////////

#include "Precompiled.h"
#include "GameLogic.h"
#include "WindowsSystem.h"
#include "Core.h"
#include "ComponentCreator.h"
#include "Camera.h"
#include "DebugTools.h"
#include "XMLSerialization.h"
#include "Utility.h"
#include <ctime>

namespace Framework
{
	//Our global pointer to the game.
	GameLogic* LOGIC = NULL;

	void GameLogic::Initialize()
	{
		//Components are explicitly registered in initialize
		RegisterComponent(Transform);
		//This macro expands into FACTORY->AddComponentCreator( "Transform" , new ComponentCreatorType<Transform>()  );

		loadGameSettings("Objects\\gameSettings.xml");
		//loadGameSettings("Objects\\gameSettings.xml");
		const bool UseLevelFile = true;

		if( UseLevelFile )
		{
			std::cout<<"reading level file\n";
			LoadLevelFile("Objects\\Level_0.xml");

			CAMERA=FACTORY->CreateEmptyComposition();
			Transform * cameratransform = new Transform();
			cameratransform->Position= Vec3( 0.0f,0.0f,-30.0f );
			CAMERA->AddComponent( CT_Transform, cameratransform );
			CAMERA->AddComponent( CT_Camera , new Camera() );
			CAMERA->Initialize();

			{
				GOC * gameObject = FACTORY->CreateEmptyComposition();
				Transform * transform = new Transform();
				transform->Position = Vec3(2,2,0);
				GRAPHICS->RegisterDebug(transform,DEBUG_SHAPE_PRISM, Vec3(1,1,3), Vec4(0,1,0,1), "platform");
				gameObject->AddComponent(CT_Transform , transform );
				Body * body = new Body();
				body->Mass = 0.0f;
				body->Restitution = 0.00001f;
				body->Friction = 0.3f;
				ShapeAAB * box = new ShapeAAB();
				box->Extents = Vec2(0.5,0.5);
				body->Position = Vec2(2,2);
				body->BodyShape = box;		
				gameObject->AddComponent(CT_Body, body );
				gameObject->Initialize();
			}

		}
		else
		{
			
		}
		ray= new Rays();
	}

	GameLogic::GameLogic()
	{
		//Safe Id reference of the object the user has grabbed
		GrabbedObjectId = 0;

		//Set up the global pointer
		ErrorIf(LOGIC!=NULL,"Logic already initialized");
		HERO = 0;
		LOGIC = this;

	}

	GameLogic::~GameLogic()
	{
		delete _gameData;
	}


	static bool cameraview=false;//WRONG CODE, FIX LATER
	static bool allowJump=false;//WRONG CODE, FIX LATER


	void GameLogic::SendMessage(Message * m )
	{
		
		switch( m->MessageId )
		{

			case Mid::Collide:
				{
					MessageCollide * key = (MessageCollide*)m;

					if (key->CollideObject->CompositionTypeId ==GOC_Type_Hero &&  key->ContactNormal.y==1){
						allowJump=true;
					}
					if (key->CollideObject->CompositionTypeId ==GOC_Type_ButtonPlatform  &&  key->ContactNormal.y==-1){
					}
				break;
				}
			// The user has pressed a (letter/number) key, we may respond by creating
			// a specific object based on the key pressed.
			case Mid::CharacterKey:
				{
					// Cast to the derived message type
					MessageCharacterKey * key = (MessageCharacterKey*)m;
					//std::cout<<"Got char key: "<<key->character<<"\n";
					//When different keys are pressed create different objects
					if(key->character=='c')
					{
						cameraview=(!cameraview);
					}
					else if(key->character == VK_F1)	// Tobggle Frame pause or enter frame by frame mode
					{
						ToggleFrameByFrameMode fbf;
						CORE->BroadcastMessage(&fbf);
					}
					else if(key->character == VK_F3)	// Advance the frame in the frame by frame mode
					{
						AdvanceFrame af(1);
						CORE->BroadcastMessage(&af);
					}
					else if( key->character == VK_ESCAPE )// quit the game
					{
						MessageQuit *mc= new MessageQuit;
						CORE->BroadcastMessage( mc );
					}
					else if( key->character == VK_SPACE )// jump
					{
						if (allowJump )
						{
							allowJump=false;
							//std::cout<<"DO NOT ALLOW JUMP"<< std::endl;
							HERO->has(Body)->SetVelocity(Vec2(0,7.5));
						}
					}
					break;
				}
			case Mid::MouseMove:
				{
					//The mouse has moved, update the mouse's world position
					MouseMove * mouseMove = (MouseMove*)m;
					GRAPHICS->ScreenToWorldSpace(&WorldMousePosition,&mouseMove->MousePosition);
					break;
				}

		}

	};
	
	void GameLogic::Update(float dt)
	{
		
		if(!cameraview)
		{
			int Speed=3;
			Body *body=HERO->has(Body);

			/*if( IsUpHeld() )
				body->Position.y += Speed * dt;
			if( IsDownHeld() )
				body->Position.y -= Speed * dt;*/
			if( IsLeftHeld() )
				body->Position.x  -= Speed * dt;
			if( IsRightHeld() )
				body->Position.x += Speed * dt;

			CAMERA->has(Transform)->Position.x=HERO->has(Transform)->Position.x;
			//std::cout<<body->Position.x<<"  "<<body->Position.y<<std::endl;
		}
		else
		{

			int Speed=40;
			if( IsUpHeld() )
				CAMERA->has(Transform)->Position.y += Speed * dt;
			if( IsDownHeld() )
				CAMERA->has(Transform)->Position.y -= Speed * dt;
			if( IsLeftHeld() )
				CAMERA->has(Transform)->Position.x  -= Speed * dt;
			if( IsRightHeld() )
				CAMERA->has(Transform)->Position.x  += Speed * dt;
		}
		ray->Update(HERO->has(Body)->Position,WorldMousePosition);
		//ray->Update(Vec2(0,0),WorldMousePosition);

	}
	

	GOC* GameLogic::CreateObjectAt(const char* archId, Vec2& position, float rotation, Vec3& scale)
	{
		//std::cout<<"Creating obj with Arch: "<<archId<<", pos: "<<position<<", rotation: "<<rotation<<", scale: "<<scale<<"\n";
		GOC * newObject = FACTORY->BuildFromArchetype(archId);
		//Get the transform and adjust the position
		Transform * transform = newObject->has(Transform);

		transform->Position = Vec3(position.x, position.y, 0.0f);
		transform->Rotation = rotation;
		transform->Scale = scale;

		Body * body = newObject->has(Body);

		if (body){
			body->Position=Vec2(position.x, position.y);
			if (body->BodyShape->Id==Shape::SidBox){
				static_cast<ShapeAAB*>(body->BodyShape)->Extents=Vec2(scale.x/2.0f,scale.y/2.0f);
			}
		}

		if (strcmp(archId, "arc_1")==0)	{
			newObject->CompositionTypeId=GOC_Type_Hero ;
		}
		else if (strcmp(archId, "arc_2")==0){
			newObject->CompositionTypeId=GOC_Type_ButtonPlatform  ;
		}

		//Initialize the composition
		newObject->Initialize();

		return newObject;
	}
	GOC* GameLogic::CreateObjectAt(const char* archId, Vec2& position, float rotation)
	{
		return CreateObjectAt(archId, position, rotation, Vec3(1.0f, 1.0f, 1.0f));
	}
	GOC* GameLogic::CreateObjectAt(const char* archId, Vec2& position)
	{
		return CreateObjectAt(archId, position, 0.0f, Vec3(1.0f, 1.0f, 1.0f));
	}
	void GameLogic::loadGameSettings(const std::string& filename)
	{
		_gameData = new GameData();
		_gameData->initialize();
		GAMEDATA->loadLevelData(filename.c_str());
	}
	void GameLogic::LoadLevelFile(const std::string& filename)
	{
		std::cout<<"Opening Level file: "<<filename.c_str()<<"\n";
		XMLReader *reader = new XMLReader();
		if(reader->open(filename.c_str()))
		{
			ISerializer* str = reader->data().clone();
			str->readProperty("Level", *str);
			str->readProperty("Layers", *str);
			str->readProperty("Layer", *str);
			str->readProperty("Items", *str);
			//
			std::vector<ISerializer*> comps;
			str->readProperty("Item", comps);
			//
			for(std::vector<ISerializer*>::iterator it1 = comps.begin(), it2 = comps.end(); it1 != it2; ++it1)
			{
				ISerializer* stream = *it1;
				std::string id, arch;
				//
				stream->readProperty("ArchetypeId", *stream);
				
				stream->readValue("value", arch);
			
				GOC* obj;

		
				//
				if(!arch.empty())
				{
					Vec2 pos;
					float rot;
					Vec3 scl;
					//
					stream->goToParent();
					if(stream->readProperty("Position", *stream))
					{
						stream->readValue("value", pos);
						stream->goToParent();
						if(stream->readProperty("Rotation", *stream))
						{
							stream->readValue("value", rot);
							stream->goToParent();
							if(stream->readProperty("Scale", *stream))
							{
								stream->readValue("value", scl);
								//std::cout<<"Node: "<<stream->name()<<"\n";
								if (arch=="arc_1")
								{
									HERO=LOGIC->CreateObjectAt(arch.c_str(), pos, rot, scl);

								}
								else
									LOGIC->CreateObjectAt(arch.c_str(), pos, rot, scl);
								
							}
							else
							{
								LOGIC->CreateObjectAt(arch.c_str(), pos, rot);
							}
						}
						else
						{
							LOGIC->CreateObjectAt(arch.c_str(), pos);
						}
					}
					else
					{
						obj = FACTORY->Create(*stream);
						
					}
					
				}
				else
				{
					obj = FACTORY->Create(*stream);
				}
			
			
			}
			//
			deletePointersVec(comps);
			delete str;
		}
		delete reader;

	}

	Rays::Rays()
	{ 
		numberOfRays=0;
		line_list=new LineListType();
		GRAPHICS->RegisterLineList(line_list); 
	}

	void Rays::Update(Vec2 heroPos,Vec2 mousePos)
	{
		line_list->clear();
		Line line;
		line.start=heroPos;
		line.end=mousePos;
		line_list->push_back(line);
		numberOfRays=line_list->size();
		unsigned int i=0;
		while(i<line_list->size())
		{
			CollisionCheck();
			numberOfRays=line_list->size();
			++i;
			if(numberOfRays>5) break;
		}
		//std::cout<<numberOfRays<<std::endl;
		
	}
	
	void Rays::CollisionCheck()
	{
		ObjectLinkList<Body>::iterator first= PHYSICS->Bodies.begin();
		ObjectLinkList<Body>::iterator last= PHYSICS->Bodies.end();
		LineListType::iterator it;
		it=line_list->begin();
		Vec2 test,reflect;
		float x=0.0f,y=0.0f;
		test=(it+(numberOfRays-1))->end;
		for(;first!=last;++first)
		{
			if(first->GetOwner()->CompositionTypeId==GOC_Type_Hero)
				continue;
			float halfWidth,halfHeight;
			halfWidth=static_cast<ShapeAAB*>(first->BodyShape)->Extents.x;
			halfHeight=static_cast<ShapeAAB*>(first->BodyShape)->Extents.y;

			//lower side of the box
			y=first->Position.y-halfHeight;
			x=FindX(y);
			if(x<=(first->Position.x+halfWidth) && x>=(first->Position.x-halfWidth))
			{
				if(SquaredDistance((it+(numberOfRays-1))->start,Vec2(x,y))<=SquaredDistance((it+(numberOfRays-1))->start,test)
					&& SquaredDistance((it+(numberOfRays-1))->start,(it+(numberOfRays-1))->end)>=SquaredDistance((it+(numberOfRays-1))->end,Vec2(x,y))
					&& SquaredDistance((it+(numberOfRays-1))->start,Vec2(x,y))>=0.01)
				{
					test=Vec2(x,y);
					reflect.x=x+(x-(it+(numberOfRays-1))->start.x);
					reflect.y=(it+(numberOfRays-1))->start.y;
				}
					
			}

			//top of the box
			y=first->Position.y+halfHeight;
			x=FindX(y);
			if(x<=first->Position.x+halfWidth && x>=first->Position.x-halfWidth)
			{
				if(SquaredDistance((it+(numberOfRays-1))->start,Vec2(x,y))<=SquaredDistance((it+(numberOfRays-1))->start,test)
					&& SquaredDistance((it+(numberOfRays-1))->start,(it+(numberOfRays-1))->end)>=SquaredDistance((it+(numberOfRays-1))->end,Vec2(x,y))
					&& SquaredDistance((it+(numberOfRays-1))->start,Vec2(x,y))>=0.01)
				{
					test=Vec2(x,y);
					reflect.x=x+(x-(it+(numberOfRays-1))->start.x);
					reflect.y=(it+(numberOfRays-1))->start.y;
				}
			}

			//left side of the box
			x=first->Position.x-halfWidth;
			y=FindY(x);
			if(y<=first->Position.y+halfHeight && y>=first->Position.y-halfHeight)
			{
				if(SquaredDistance((it+(numberOfRays-1))->start,Vec2(x,y))<=SquaredDistance((it+(numberOfRays-1))->start,test)
					&& SquaredDistance((it+(numberOfRays-1))->start,(it+(numberOfRays-1))->end)>=SquaredDistance((it+(numberOfRays-1))->end,Vec2(x,y))
					&& SquaredDistance((it+(numberOfRays-1))->start,Vec2(x,y))>=0.01)
				{
					test=Vec2(x,y);
					reflect.x=(it+(numberOfRays-1))->start.x;
					reflect.y=y+(y-(it+(numberOfRays-1))->start.y);

				}
			}

			//right side of the box
			x=first->Position.x+halfWidth;
			y=FindY(x);
			if(y<=first->Position.y+halfHeight && y>=first->Position.y-halfHeight)
			{
				if(SquaredDistance((it+(numberOfRays-1))->start,Vec2(x,y))<=SquaredDistance((it+(numberOfRays-1))->start,test)
					&& SquaredDistance((it+(numberOfRays-1))->start,(it+(numberOfRays-1))->end)>=SquaredDistance((it+(numberOfRays-1))->end,Vec2(x,y))
					&& SquaredDistance((it+(numberOfRays-1))->start,Vec2(x,y))>=0.01)
				{
					test=Vec2(x,y);
					reflect.x=(it+(numberOfRays-1))->start.x;		
					reflect.y=y+(y-(it+(numberOfRays-1))->start.y);

				}
			}

		}
		if((it+(numberOfRays-1))->end!=test && (it+(numberOfRays-1))->start!=test)
			{
				(it+(numberOfRays-1))->end=test;
				bool reflection=true;
				if(reflection)
				{
					//normalize(reflect);
					//reflect*=8;
					Line line;
					line.start=test;
					Vec2 temp=reflect-test;
					normalize(temp);
					line.end=test+temp*20;
					line_list->push_back(line);
					return;
				}
			}
	}

	float Rays::FindX(float y)
	{
		LineListType::iterator it;
		it=line_list->begin();
		float slope=(((it+(numberOfRays-1))->end.y - (it+(numberOfRays-1))->start.y)/((it+(numberOfRays-1))->end.x - (it+(numberOfRays-1))->start.x));
		return ( (y-(it+(numberOfRays-1))->end.y) * (1/slope) + (it+(numberOfRays-1))->end.x  ); 
	}

	float Rays::FindY(float x)
	{
		LineListType::iterator it;
		it=line_list->begin();
		float slope=(((it+(numberOfRays-1))->end.y - (it+(numberOfRays-1))->start.y)/((it+(numberOfRays-1))->end.x - (it+(numberOfRays-1))->start.x));
		return ( (x-(it+(numberOfRays-1))->end.x) * (slope) + (it+(numberOfRays-1))->end.y  ); 
	}

	float Rays::SquaredDistance(Vec2 a,Vec2 b)
	{
		return((a.x-b.x)*(a.x-b.x)+(a.y-b.y)*(a.y-b.y));
	}




}