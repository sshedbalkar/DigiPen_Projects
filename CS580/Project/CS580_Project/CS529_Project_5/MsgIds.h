#ifndef MESSAGEIDS_H
#define MESSAGEIDS_H
//#pragma message("Compiling: "__FILE__)
//
namespace Wye
{
	struct Msgid // Message id
	{
//#pragma message("Including Class: Msgid")
		//struct MessageIdType
		//{
			enum MessageIdType
			{
				None,
				Quit,
				ToggleDebugInfo,
				CharacterKey,
				MouseButton,
				MouseMove,
				MouseWheel,
				FileDrop,
				Collide,
				FrameByFrame,
				AdvanceFrame,
				EnterFrame,
				BombEffectDone,
				BombExplode,
				Pause,
				//
				//
				LastVal = FileDrop
			};
			//bool operator==(const MessageIdType& rhs) const;
		//};
		/*
		struct extended_MessageIdType: MessageIdType
		{
			enum
			{
				new_msg_1 = MessageIdType::LastVal + 1,
				new_msg_2,
				new_msg_2
			};
		};
		*/
	};
}
//
#endif