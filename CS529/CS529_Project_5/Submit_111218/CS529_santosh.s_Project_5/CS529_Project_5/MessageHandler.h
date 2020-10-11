////////////////////////////////////////////////////////////////
//
//	MessageHandler.h
//	Handles various messages at the Logic level
//
//	Author: Santosh Shedbalkar
//	Copyright 2011, Digipen Institute of Technology
//
////////////////////////////////////////////////////////////////
#ifndef MESSAGE_HANDLER_H
#define MESSAGE_HANDLER_H
//
#include "Message.h"
//
namespace Wye
{
	class Sprite;
	//
	class MessageHandler: public Object
	{
#pragma message("Including class: MessageHandler")
	private:
		MessageHandler();
		~MessageHandler();
		//
		static MessageHandler* _instance;
		bool _debug;
		bool _showSettings;
		Sprite* _settings;
	public:
		static MessageHandler* getInstance()
		{
			if(!_instance)
			{
				_instance = new MessageHandler();
			}
			return _instance;
		}
		//
		void handleMessage(const Message& msg);
	};
}
//
#endif