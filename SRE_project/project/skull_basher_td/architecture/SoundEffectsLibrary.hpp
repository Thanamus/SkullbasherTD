// Based on https://www.youtube.com/watch?v=fG2veGWNPJY
// git repo: https://github.com/codetechandtutorials/openal-impl
// Thanks Matthew Early for the OpenAL implementation

#pragma once
#include <AL\al.h>
#include <vector>
#include <map>
#include <string>

/// <summary>
/// SoundEffectsLibrary: Singleton class that keeps track of the shorter sounds
/// we've loaded into memory. Use the Play function from an instantiation of the
/// SoundEffectsPlayer on an ID returned from the load function.
/// 
/// load function: Use the SE_LOAD(...) function to add sounds, programmer should keep track of
/// the returned ALuint to use the sound.
/// 
/// unload function: Use SE_UNLOAD(...) remove sounds from memory. 
/// 
/// </summary>

#define SE_LOAD SoundEffectsLibrary::Get()->Load
#define SE_UNLOAD SoundEffectsLibrary::Get()->UnLoad

class SoundEffectsLibrary
{
public:
	static SoundEffectsLibrary* Get();

	ALuint Load(const char* filename);
	bool UnLoad(const ALuint& bufferId);

	bool retrive_buffer_of_loaded_sound_effect(std::string sound_to_retrive, ALuint* const &returned_buffer);

private:
	SoundEffectsLibrary();
	~SoundEffectsLibrary();

	std::vector<ALuint> p_SoundEffectBuffers; //TODO change this to a map

	std::map<std::string, ALuint> loadedSounds;

	//add a load sound effects function in scene
};

