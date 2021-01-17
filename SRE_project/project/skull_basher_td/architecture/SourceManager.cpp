#include "SourceManager.hpp"

#include <iostream>
#include <AL/al.h>
#include "OpenALErrorCheck.hpp"

SourceManager::SourceManager(/* args */)
{
    // make sources
    // assign sources to a map
    // alSourcePlay(0);
    alGenSources(1, &source);
    alSourcei(/**/source,/**/AL_BUFFER,/**/0);
    
}

SourceManager::~SourceManager()
{
    alDeleteSources(1, &source);
}

static SourceManager* _instance = nullptr;

SourceManager* SourceManager::Get()
{
    Init();
    return _instance;
}

void SourceManager::Init(){
    if (_instance == nullptr) _instance = new SourceManager;
}

void SourceManager::playSource(ALuint buffer_to_play){
    	if (buffer_to_play != p_buffer) //p_buffer defaults to 0, otherwise it should be 'the last used buffer'
	{
			p_buffer = buffer_to_play;
			alSourcei(source, AL_BUFFER, (ALint)p_buffer);
			AL_CheckAndThrow();
	}
	// alSourceRewind(p_Source);
	alSourcePlay(source);
	std::cout << "playing a sound from source Manager: " << p_buffer << std::endl;
	AL_CheckAndThrow();
}