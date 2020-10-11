#include "MovieClip.h"
#include "ComponentTypeIds.h"
#include <algorithm>
#include "Utility.h"
//#include "Graphics.h"
#include <iostream>
//
Wye::Frame f;
namespace Wye
{
	Frame::Frame()
		:Object(),
		Next(0),
		Prev(0),
		_frame(0),
		_spriteName("")
	{
		//
	}
	Frame::Frame(unsigned frame, const char* sprite)
		:Object(),
		Next(0),
		Prev(0),
		_frame(frame),
		_spriteName(sprite)
	{
		//
	}
	Frame::~Frame()
	{
		//
	}
	inline unsigned Frame::frameNum()const
	{
		return _frame;
	}
	inline std::string& Frame::spriteName()
	{
		return _spriteName;
	}
	////////////////////////////////////////////////////
	MovieClip::MovieClip()
		:Sprite(),
		_frames(),
		_currFrame(0),
		_frameId(0),
		_frameObj(0),
		_iterator(0),
		_paused(false)
	{
		setType(CT_MovieClip);
	}
	MovieClip::~MovieClip()
	{
		Frame* fr = _frames.front();
		while(fr)
		{
			delete fr;
			_frames.pop_front();
			if(_frames.size()>0)
			{
				fr = _frames.front();
			}
			else
			{
				fr = NULL;
			}
		}
		delete _iterator;
	}
	void MovieClip::initialize()
	{
		std::cout<<"MovieClip: Frames: " <<_frames.size()<<"\n";
		Sprite::initialize();
	}
	void MovieClip::serialize(const ISerializer& stream)
	{
		Sprite::serialize(stream);
		ISerializer* str = stream.clone();
		str->readProperty("Frames", *str);
		if(str->isGood())
		{
			std::vector<ISerializer*> frs;
			Frame* fr;
			str->readProperty("Frame", frs);
			for(std::vector<ISerializer*>::iterator it1 = frs.begin(), it2 = frs.end(); it1 != it2;++it1)
			{
				fr = Frame::createFrom(**it1);
				addFrame(fr);
			}
			deletePointersVec(frs);
			//
			//testFn();
		}
		//
		delete str;
	}
	struct FrameSorter
	{
		bool operator()(const Frame* left, const Frame* right)const
		{
			return left->frameNum() < right->frameNum();
		}
	};
	void MovieClip::addFrame(Frame* frame)
	{
		_frames.push_back(frame);
		_frames.sort(FrameSorter());
		_frameObj = NULL;
	}
	void MovieClip::removeFrame(Frame* frame)
	{
		_frames.remove(frame);
		_frames.sort(FrameSorter());
		_frameObj = NULL;
	}
	void MovieClip::testFn()
	{
		for(std::list<Frame*>::iterator it1 = _frames.begin(), it2 =_frames.end(); it1 != it2; ++it1)
		{
			std::cout<<"Frame2: "<<(*it1)->frameNum()<<"\n";
		}
	}
	void MovieClip::draw(IDirect3DDevice9* pDevice)
	{
		updateFrame();
		//
		Sprite::draw(pDevice);
	}
	std::list<Frame*>::iterator MovieClip::findFrameId()
	{
		std::list<Frame*>::iterator it1 = _frames.begin(), it2 = _frames.end(), it3 = _frames.begin();
		int id = 0;
		while(it1 != it2 && (*it1)->frameNum() < _currFrame)
		{
			it3 = it1;
			++it1;
			++id;
		}
		_frameId = id;
		return it3;
	}
	void MovieClip::updateFrame()
	{
		if(!_frameObj)
		{
			if(_frames.size())
			{
				std::list<Frame*>::iterator it;
				if(_currFrame > 0)
				{
					it = findFrameId();
					Sprite::setSpriteName((*it)->spriteName());
				}
				delete _iterator;
				_iterator = new std::list<Frame*>::iterator(_frames.begin());
				advanceIterator(*_iterator, _frameId);
				_frameObj = **_iterator;
			}
			else if(Sprite::getSpriteName().empty())
			{
				Sprite::setTexture(0);
			}
		}
		if(_frameObj && _currFrame >= _frameObj->frameNum())
		{
			//std::cout<<"Frame: " <<_currFrame<<" name: " << _frameObj->spriteName()<<"\n";
			Sprite::setSpriteName(_frameObj->spriteName());
			++_frameId;
			if(++*_iterator == _frames.end())
			{
				delete _iterator;
				_iterator = new std::list<Frame*>::iterator(_frames.begin());
				_currFrame = 0;
				_frameId = 0;
			}
			_frameObj = **_iterator;
		}
		//
		if(!_paused)
		{
			++_currFrame;
		}
	}
	unsigned MovieClip::currentFrame()
	{
		return _currFrame;
	}
	unsigned MovieClip::totalFrames()
	{
		return _frames.back()->frameNum() - 1;
	}
	void MovieClip::play()
	{
		//std::cout<<"Play:\n";
		_paused = false;
	}
	void MovieClip::stop()
	{
		_paused = true;
	}
	void MovieClip::gotoAndStop(unsigned frame)
	{
		_frameObj = NULL;
		_currFrame = frame;
		_paused = true;
	}
	void MovieClip::gotoAndPlay(unsigned frame)
	{
		//std::cout<<"gotoAndPlay from: "<<frame<<"\n";
		_frameObj = NULL;
		Frame* last = _frames.back();
		_currFrame = (frame > last->frameNum()) ? last->frameNum() : frame;
		_paused = false;
	}
}