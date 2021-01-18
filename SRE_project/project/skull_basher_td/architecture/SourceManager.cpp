#include "SourceManager.hpp"

#include <iostream>
#include <array>
#include <vector>
// #include <algorithm>
#include <AL/al.h>
#include "OpenALErrorCheck.hpp"

SourceManager::SourceManager(unsigned int num_of_sources)
{
    // make sources
    // assign sources to a map
    // alSourcePlay(0);
    // alGenSources(1, &source);
    // alGenSources(2, *sourceArray);
	// sizeof(arrayTest)/sizeof(*arrayTest);
	// int iterator = sizeof(sourceArray)/sizeof(*sourceArray);
	// for (size_t i = 0; i <  iterator; i++)
	// {
	// 	/* code */
    // 	alSourcei(/**/i,/**/AL_BUFFER,/**/0);
	// }
	
    	// alSourcei(/**/source,/**/AL_BUFFER,/**/0);
		

	for(unsigned int i = 0; i < num_of_sources; ++i) {
		ALuint source;

		alGenSources(1,&source);
		ALenum error = alGetError();

		if(error == AL_NO_ERROR)	{
			sources.push_back(new OALSource(source));
		}
		else{
			break;
		}
	}

	std::cout << "SoundSystem has " << sources.size() << " channels available!"  << std::endl;
    
}

SourceManager::~SourceManager()
{
    // alDeleteSources(1, &source);
	// int iterator = sizeof(sourceArray)/sizeof(*sourceArray);
    // alDeleteSources(iterator, *sourceArray);
	for(std::vector<OALSource*>::iterator i = sources.begin(); i != sources.end(); ++i) {
		alDeleteSources(1, &(*i)->source);
		delete (*i);
	}
}

static SourceManager* _instance = nullptr;

SourceManager* SourceManager::Get()
{
    Init();
    return _instance;
}

void SourceManager::Init(){
    if (_instance == nullptr) _instance = new SourceManager();
}

void SourceManager::playSource(ALuint buffer_to_play){

	OALSource* source_that_will_play_sound = GetSource();

	attachSource(source_that_will_play_sound, buffer_to_play);

    	if (buffer_to_play != p_buffer) //p_buffer defaults to 0, otherwise it should be 'the last used buffer'
	{
			p_buffer = buffer_to_play;
			// alSourcei(source, AL_BUFFER, (ALint)p_buffer);
			// alSourcei(*sourceArray[0], AL_BUFFER, (ALint)p_buffer);
			AL_CheckAndThrow();
	}
	// alSourceRewind(p_Source);
	// alSourcePlay(*sourceArray[0]);
	std::cout << "playing a sound from source Manager: " << p_buffer << std::endl;
	AL_CheckAndThrow();
}


void SourceManager::attachSource (OALSource* s, ALuint buffer_to_play){
	oalSource = s;

	if(!oalSource) {
		std::cout << "hey, there was no OALSource object" << std::endl;
		return;
	}

	oalSource->inUse = true;
	radius = 100.0f;
	alSourceStop(oalSource->source);	
	alSourcef(oalSource->source, AL_MAX_DISTANCE, radius);
	alSourcef(oalSource->source, AL_REFERENCE_DISTANCE, radius * 0.2f);

	//if(timeLeft > 0) {
		// if(sound->IsStreaming()) {	//Part 2
		// 	streamPos = timeLeft;
		// 	int numBuffered = 0;
		// 	while(numBuffered < NUM_STREAM_BUFFERS) {
		// 		double streamed = sound->StreamData(streamBuffers[numBuffered],streamPos);

		// 		if(streamed) {
		// 			streamPos -= streamed;
		// 			++numBuffered;
		// 		}
		// 		else{
		// 			break;
		// 		}
		// 	}
		// 	alSourceQueueBuffers(oalSource->source, numBuffered, &streamBuffers[0]);
		// }
		// else{
			// alSourcei(oalSource->source,AL_BUFFER,sound->GetBuffer());
			// alSourcei(oalSource->source,AL_BUFFER,0);
			alSourcei(oalSource->source,AL_BUFFER,buffer_to_play);
			// alSourcef(oalSource->source,AL_SEC_OFFSET,(sound->GetLength()/ 1000.0) - (timeLeft / 1000.0));

			// std::cout << "Attaching! Timeleft: " << (sound->GetLength()/ 1000.0) - (timeLeft / 1000.0) << std::endl;
			alSourcePlay(oalSource->source);
		// }
		// alSourcePlay(oalSource->source);
}

OALSource*	SourceManager::GetSource() {
	// Iterates over the available OALSource objects and gets a free one.
	// TODO release sources when playback is finished
	for(std::vector<OALSource*>::iterator i = sources.begin(); i != sources.end(); ++i) {
		OALSource*s = *i;
		if(!s->inUse) {
			return s;
		}
	}
	std::cout << "No available OALSources" << std::endl;
	return NULL;
}

void SourceManager::CheckAndReleaseOALSource (){
// Go through vector of OALSources, and set inUse to false for anything that isn't playing
	for(std::vector<OALSource*>::iterator i = sources.begin(); i != sources.end(); ++i) {
		OALSource*s = *i;
		if(s->inUse) {
			//only check the ones that are in use
			ALuint source_getting_checked = s->source; //get the current OALSource source object

			ALint playState;
			alGetSourcei(source_getting_checked, AL_SOURCE_STATE, &playState);
				// alGetSourcei(p_Source, AL_SOURCE_STATE, &playState);
				if (playState == AL_STOPPED)
				{
					s->inUse = false;
					std::cout << "just released a source" << std::endl;
				}
				
				// return (playState == AL_PLAYING);
			// return s;
		}
	}
	// std::cout << "No available OALSources" << std::endl;
	// return NULL;

	// bool SoundEffectsPlayer::isPlaying()
	// {
	// 	ALint playState;
	// 	alGetSourcei(p_Source, AL_SOURCE_STATE, &playState);
	// 	return (playState == AL_PLAYING);
	// }
}


// void	SoundSystem::AttachSources(vector<SoundNode*>::iterator from, vector<SoundNode*>::iterator to) {
// 	for(vector<SoundNode*>::iterator i = from; i != to; ++i) {
// 		if(!(*i)->GetSource()) {	//Don't attach a new source if we already have one!
// 			(*i)->AttachSource(GetSource());
// 		}
// 	}
// }