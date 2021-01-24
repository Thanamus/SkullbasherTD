
#include <iostream>
#include <array>
#include <vector>
// #include <algorithm>
#include <AL/al.h>

#include "OpenALErrorCheck.hpp"
#include "SourceManager.hpp"
#include "SoundEffectsLibrary.hpp"
#include "SoundDevice.hpp"

/*
	Source manager makes a pool of sources that can be used to play sounds
	It is a Singleton pattern, meaning that sounds can be played from anywhere in the code base

	To play a sound effect call
	playMyJam()
		for a sound that has a positon
		or 
	playMyJam_global()
		for a global sound, i.e one that doesn't have a position

	Source manager will take care of getting the sound's buffer and attaching it to a source that 
	OpenAL uses to play the sound

*/


SourceManager::SourceManager(unsigned int num_of_sources)
{
		
	/* Make the designated number of sources*/
	for(unsigned int i = 0; i < num_of_sources; ++i) {
		// init a source ALuint
		ALuint source;

		alGenSources(1,&source); //Gererate source and store refernce
		ALenum error = alGetError(); // check for errors

		if(error == AL_NO_ERROR)	{
			// If no error, push the source back into the list of sources
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
	// Iterate through all sources stored and delete them
	for(std::vector<OALSource*>::iterator i = sources.begin(); i != sources.end(); ++i) {
		alDeleteSources(1, &(*i)->source);
		delete (*i);
	}
}

static SourceManager* _instance = nullptr;


/*Singleton pattern*/
SourceManager* SourceManager::Get()
{
    Init();
    return _instance;
}

void SourceManager::Init(){
    if (_instance == nullptr) _instance = new SourceManager();
}





/* 	Attaches a source to the pool of sources available
	then attaches a sound effect buffer to that source
*/
void SourceManager::attachSource (OALSource* s, ALuint buffer_to_play){
	oalSource = s;

	if(!oalSource) {
		// basic check that there is a source object
		std::cout << "hey, there was no OALSource object" << std::endl;
		return;
	}

	oalSource->inUse = true; // set the source as 'in use'
	// radius = 100.0f;

	alSourceStop(oalSource->source); // make sure the source is paused / stopped for config	
	alSourcei(oalSource->source,AL_BUFFER,buffer_to_play); // Give the source the sound effect to play (i.e. the buffer the sound effect is stored in)
}




/* Get a source if one is available*/
OALSource*	SourceManager::GetSource() {


	// Iterates over the available OALSource objects and gets a free one.
	for(std::vector<OALSource*>::iterator i = sources.begin(); i != sources.end(); ++i) {
		OALSource*s = *i;
		if(!s->inUse) {
			return s; // return the free source that was found
		}
	}
	std::cout << "No available OALSources" << std::endl;
	return nullptr; // No source available, return NULL
}





/* Goes through the sources that are in use and frees them if the sound has stopped playing*/
void SourceManager::CheckAndReleaseOALSource (){

/* Go through vector of OALSources, and set inUse to false for anything that isn't playing */
	for(std::vector<OALSource*>::iterator i = sources.begin(); i != sources.end(); ++i) {
		OALSource*s = *i;
		if(s->inUse) {
			//only check the ones that are in use

			ALuint source_getting_checked = s->source; //get the current OALSource source object

			ALint playState;
			alGetSourcei(source_getting_checked, AL_SOURCE_STATE, &playState); // check the play state of the source and return int for enum check

			if (playState == AL_STOPPED)
			{
				// if source has stopped, free it (by marking it as available, doesn't actually free memory)
				s->inUse = false;
				std::cout << "just released a source" << std::endl;
			}
		}
	}
}




/* Play a global sound, eg, a gui sound - global means that the sound isn't positional*/
void SourceManager::playMyJam_global(std::string sound_to_play){
	// sound to play is the name of the sound that gets looked up in the Sound Effects Library map
	ALuint buffer_to_play;
	
	// Get sound and load it in to buffer (or return if that fails)
	if(!SoundEffectsLibrary::Get()->retrive_buffer_of_loaded_sound_effect(sound_to_play, &buffer_to_play)){
		std::cout << "tried to play a sound that wasn't loaded";
		return;
	}

	OALSource* source_that_will_play_sound = GetSource(); // Get a source if one is available

	if (source_that_will_play_sound != nullptr)
	{
		/* Source availabe, proceed with attaching */
		attachSource(source_that_will_play_sound, buffer_to_play);


	// so no need to set sounds position for global sounds, just set relative to true
		alSourcef(oalSource->source, AL_MAX_DISTANCE, 2); // sets the max distance for the sound to be heard
		alSourcef(oalSource->source, AL_REFERENCE_DISTANCE, 2.0f); // So sound doesn't roll off
		alSourcef(oalSource->source, AL_ROLLOFF_FACTOR, 0.0f); // No roll off
		alSourcei(oalSource->source, AL_SOURCE_RELATIVE, 1); // set relative to true so it always plays on the listeners head
		

			if (buffer_to_play != p_buffer) //p_buffer defaults to 0, otherwise it should be 'the last used buffer'
		{
				p_buffer = buffer_to_play;
				AL_CheckAndThrow();
		}
		alSourcePlay(oalSource->source); // play sound
	}
	

	// std::cout << "playing a global sound from source Manager: " << p_buffer << std::endl;
	AL_CheckAndThrow();
}





/* Plays a positional sound */
void SourceManager::playMyJam(std::string sound_to_play, glm::vec3 position, float max_distance_of_Sound){
	// Sound to play is the name of the sound
	// position is position
	// max_distance_of_Sound is the max distance of the sound

	ALuint buffer_to_play;
	
	// Get sound and load it in to buffer (or return if that fails)
	if(!SoundEffectsLibrary::Get()->retrive_buffer_of_loaded_sound_effect(sound_to_play, &buffer_to_play)){
		std::cout << "tried to play a sound that wasn't loaded";
		return;
	}

	OALSource* source_that_will_play_sound = GetSource(); // Gets a source if one is availalbe

	if (source_that_will_play_sound != nullptr)
	{
		/* Source was available, go ahead and attach sound */
		attachSource(source_that_will_play_sound, buffer_to_play); // attach the source

		alSourcefv(source_that_will_play_sound->source,AL_POSITION, (float*)&position); // Sources position
		alSourcef(oalSource->source, AL_MAX_DISTANCE, max_distance_of_Sound);
		alSourcef(oalSource->source, AL_REFERENCE_DISTANCE, max_distance_of_Sound * 0.1f);
		alSourcef(oalSource->source, AL_ROLLOFF_FACTOR, 1);
		alSourcei(oalSource->source, AL_SOURCE_RELATIVE, 0); // Sets relative back to 0, making the position absolute
		// std::cout << "Max distance should be: " << max_distance_of_Sound;


			if (buffer_to_play != p_buffer) //p_buffer defaults to 0, otherwise it should be 'the last used buffer'
		{
				p_buffer = buffer_to_play;
				AL_CheckAndThrow();
		}

		alSourcePlay(oalSource->source); // actually play the sound
		// std::cout << "playing a local sound from source Manager: " << p_buffer << std::endl;
	}
	
	AL_CheckAndThrow();
}

