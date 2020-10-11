////////////////////////////////////////////////////////////////
//
//	MovieClip.h
//	implements a movie clip class which consists of various key frames and each key frame representing a different texture
//
//	Author: Santosh Shedbalkar
//	Copyright 2011, Digipen Institute of Technology
//
////////////////////////////////////////////////////////////////
#ifndef MOVIECLIP_H
#define MOVIECLIP_H
//#pragma message("Compiling: "__FILE__)
//
//#include "Object.h"
#include "Sprite.h"
#include <list>
//
namespace Wye
{
	class Object;
	class ISerializer;
	//class IDirect3DDevice9;
	//
	class Frame: public Object
	{
#pragma message("Including Class: Frame")
	public:
		Frame();
		Frame(unsigned frame, const char* sprite);
		virtual ~Frame();
		//
		Frame* Next;
		Frame* Prev;
		//
		unsigned frameNum()const;
		std::string& spriteName();
		static Frame* createFrom(unsigned frame, const char* sprite)
		{
			return new Frame(frame, sprite);
		};
		static Frame* createFrom(const ISerializer& stream)
		{
			int fr;
			std::string name;
			stream.readProperty("frameNum", fr);
			stream.readProperty("sprite", name);
			//
			return createFrom(fr, name.c_str());
		};
		//
	private:
		unsigned _frame;
		std::string _spriteName;
	};
	//
	class MovieClip: public Sprite
	{
#pragma message("Including Class: MovieClip")
	public:
		MovieClip();
		virtual ~MovieClip();
		void serialize(const ISerializer& stream);
		void initialize();
		void draw(IDirect3DDevice9* pDevice);
		void addFrame(Frame* frame);
		void removeFrame(Frame* frame);
		void testFn();
		unsigned currentFrame();
		unsigned totalFrames();
		void play();
		void stop();
		void gotoAndStop(unsigned frame);
		void gotoAndPlay(unsigned frame);
		//
	protected:
		std::list<Frame*>::iterator& advanceIterator(std::list<Frame*>::iterator& it, int n)
		{
			if(n > 0)
			{
				while(n)
				{
					++it;
					--n;
				}
			}
			else
			{
				while(n)
				{
					--it;
					++n;
				}
			}
			return it;
		}
		void updateFrame();
		std::list<Frame*>::iterator findFrameId();
		//
		std::list<Frame*> _frames;
		unsigned _currFrame;
		unsigned _frameId;
		Frame* _frameObj;
		std::list<Frame*>::iterator* _iterator;
		bool _paused;
		//
	};
}
//
#endif