#include"Audio.h"

namespace Framework
{
	Audio* AUDIO=NULL;

	Sound::Sound()
	{
		sound=NULL;
		channel=NULL;
	}

	Sound::~Sound()
	{
		if(sound!=NULL)
		{
			FMOD_Sound_Release(sound);
			sound=NULL;
		}
	}

	Audio::Audio()
	{
		system = NULL;
	}

	Audio::~Audio()
	{
		//release all sounds
		for(Iterator it= sounds.begin();it != sounds.end(); ++it)
			(*it)= NULL;
		FMOD_System_Release(system);
	}

	void Audio::Update(float timeslice)
	{
		FMOD_System_Update(system);
	}

	void Audio::Initialize()
	{
		if(FMOD_System_Create(&system) != FMOD_OK)
			return;
		if (FMOD_System_Init(system,100,FMOD_INIT_NORMAL,NULL) != FMOD_OK)
			return;
		return;
	}

	Sound* Audio::Load(string filename)
	{
		if(filename.length()==0) return false;

		Sound *sound= new Sound();
		FMOD_RESULT result;
		result= FMOD_System_CreateSound(system,				//fmod system
										filename.c_str(),   //filename
										FMOD_DEFAULT,		//default audio
										NULL,				//i dont know
										&sound->sound);		//pointer to the sound
		if(result != FMOD_OK) sound=NULL;
		return sound;
	}

	bool Audio::Load(string filename,string name)
	{
		if (filename.length() == 0 || name.length() == 0) return false;
	
		Sound *sound= new Sound();
		sound->setName(name);
		FMOD_RESULT result;
		result= FMOD_System_CreateSound(system,				//fmod system
										filename.c_str(),   //filename
										FMOD_DEFAULT |		//default audio
										FMOD_LOOP_NORMAL,   //loop normal
										NULL,				//i dont know
										&sound->sound);		//pointer to the sound
		if(result!=FMOD_OK) return false;
		sounds.push_back(sound);
		return true;
	}

	bool Audio::isPlaying(string name)
	{
		Sound *sound=findSound(name);
		if(sound==NULL) return false;

		int index;
		FMOD_Channel_GetIndex(sound->channel, &index);
		return(index>0); //index=0 if not playing index=99 if playing
	}

	bool Audio::soundExists(string name)
	{
		for(Iterator it= sounds.begin();it != sounds.end(); ++it)
		{
			if((*it)->getname()==name) return true;
		}
		return false;
	}

	Sound* Audio::findSound(string name)
	{
		Sound *sound=NULL;
		for(Iterator it= sounds.begin();it != sounds.end(); ++it)
		{
			if((*it)->getname()==name) 
			{
				sound=(*it);
				break;
			}
		}
		return sound;
	}

	bool Audio::Play(string name,int loopCount)
	{
		FMOD_RESULT result;
		Sound *sound=findSound(name);
		if(sound->sound!=NULL)//check if the sound is present
		{
			result=FMOD_System_PlaySound(system,
										 FMOD_CHANNEL_FREE,
										 sound->sound,
										 true,
										 &sound->channel);
			if(result!=FMOD_OK) return false;
			FMOD_Channel_SetLoopCount(sound->channel,loopCount);
			FMOD_Channel_SetPaused(sound->channel,false);
		}
		return true;
	}

	bool Audio::Play(Sound *sound, int loopCount)
	{
		FMOD_RESULT result;
		if(sound==NULL || sound->sound==NULL) return false;
		result=FMOD_System_PlaySound(system,
									 FMOD_CHANNEL_FREE,
									 sound->sound,
									 true,
									 &sound->channel);
		if(result!=FMOD_OK) return false;
		FMOD_Channel_SetLoopCount(sound->channel,loopCount);
		FMOD_Channel_SetPaused(sound->channel,false);
		return true;
	}

	void Audio::Stop(string name)
	{
		if(!isPlaying(name)) return;
		Sound *sound=findSound(name);
		if(sound==NULL) return;
		FMOD_Channel_Stop(sound->channel);
	}

	void Audio::stopAll()
	{
		for(Iterator it= sounds.begin();it != sounds.end(); ++it)
			FMOD_Channel_Stop((*it)->channel);
	}

	void Audio::stopAllExcept(string name)
	{
		for(Iterator it= sounds.begin();it != sounds.end(); ++it)
		{
			if((*it)->getname()!= name)
				FMOD_Channel_Stop((*it)->channel);
		}
	}

	void Audio::changeVolume(float volume)
	{
		for(Iterator it= sounds.begin();it != sounds.end(); ++it)
			FMOD_Channel_SetVolume((*it)->channel,volume);

	}

	void Audio::changeVolume(string name,float volume)
	{
		if(!isPlaying(name)) return;
		Sound *sound=findSound(name);
		if(sound==NULL) return;
		FMOD_Channel_SetVolume(sound->channel,volume);
	}

	void Audio::Pause(string name)
	{
		if(isPlaying(name))
			FMOD_Channel_SetPaused(findSound(name)->channel,true);
	}

	void Audio::resume(string name)
	{
			FMOD_Channel_SetPaused(findSound(name)->channel,false);
	}

	void Audio::pauseAll()
	{
		for(Iterator it= sounds.begin();it != sounds.end(); ++it)
		{
			FMOD_Channel_SetPaused((*it)->channel,true);
		}
	}

	void Audio::resumeAll()
	{
		for(Iterator it= sounds.begin();it != sounds.end(); ++it)
		{
			FMOD_Channel_SetPaused((*it)->channel,false);
		}
	}
}


