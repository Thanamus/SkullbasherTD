
#include <iostream>
#include <array>
#include <vector>
// #include <algorithm>

#include "ProjectileManager.hpp"

#include "../scenes/LevelScene.hpp"

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


ProjectileManager::ProjectileManager(unsigned int num_of_projectiles)
{
		
	/* Make the designated number of sources*/
	// for(unsigned int i = 0; i < num_of_projectiles; ++i) {
	// 	// init a source ALuint
	// 	ALuint source;

	// 	alGenSources(1,&source); //Gererate source and store refernce
	// 	ALenum error = alGetError(); // check for errors

	// 	if(error == AL_NO_ERROR)	{
	// 		// If no error, push the source back into the list of sources
	// 		sources.push_back(new OALSource(source));
	// 	}
	// 	else{
	// 		break;
	// 	}
	// }

	// std::cout << "SoundSystem has " << sources.size() << " channels available!"  << std::endl;
    
}

ProjectileManager::~ProjectileManager()
{
	// Iterate through all sources stored and delete them
    // TODO delete all instances of projectiles from pooles
}



/*Singleton pattern*/
static ProjectileManager* _instance = nullptr;

ProjectileManager* ProjectileManager::Get()
{
    Init();
    return _instance;
}

void ProjectileManager::Init(){
    if (_instance == nullptr) _instance = new ProjectileManager();
}




