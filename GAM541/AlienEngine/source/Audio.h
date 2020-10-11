#pragma once //Makes sure this header is only included once

#include"fmod/fmod.hpp"
#include<string>
#include<vector>
#include "Engine.h"
#include<Windows.h>

namespace Framework
{
	using namespace std;

	class Sound
	{
	private:
		string name;
	public:
		Sound();
		~Sound();

		std::string getname() {return name;}
		void setName( string Name ){name=Name; }

		FMOD_SOUND *sound;
		FMOD_CHANNEL *channel;
	};

	class Audio : public ISystem
	{
	private:
		FMOD_SYSTEM *system;
		typedef std::vector<Sound*> Sounds;
		typedef std::vector<Sound*>::iterator Iterator;
		Sounds sounds;
	public:
		Audio();
		~Audio();
		FMOD_SYSTEM* getSystem() { return system; }

		void Initialize();
		void Update(float timeslice);
		virtual std::string GetName(){return "Audio";}
		
		bool serialize();

		bool Load(string filename,string name);
		Sound* Load(string filename);

		bool isPlaying(string name);
		bool soundExists(string name);

		Sound* findSound(string name);
	
		//loopcount=-1 for not loloping
		bool Play(Sound *sound, int loopCount=-1);
		bool Play(string name, int loopCount=-1);

		void Stop(string name);
		void stopAll();
		void stopAllExcept(string name);

		void Pause(string name);
		void pauseAll();

		void resume(string name);
		void resumeAll();

		//volume 0 to 1
		void changeVolume(float volume);
		void changeVolume(string name,float volume);

	};

	extern Audio* AUDIO;
}

