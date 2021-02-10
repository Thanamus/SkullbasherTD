/*
    Thanks Newcastle Uni for the design pattern
    https://research.ncl.ac.uk/game/mastersdegree/workshops/audio/

    Note, a lot of things were changed to in their design for use in this setup
    but the general idea of attaching a source has been kept


*/

#pragma once

#include <map>
#include <vector>
#include <AL/al.h>

#include "glm/glm.hpp"

struct OALSource {
    /* Struct that holds the source information*/
	ALuint	source;
	bool	inUse;

	OALSource(ALuint src) {
		source	= src;
		inUse	= false;
	}
};

class SourceManager
{
private:
    /* data */
    SourceManager(unsigned int num_of_sources = 32); // Sets the number of sources in the pool
    ~SourceManager();
    static void Init();

    ALuint source;
    std::vector<OALSource*>	sources;
    ALuint p_buffer = 0;
    OALSource*		oalSource;
    float			radius;
    OALSource*	GetSource();

    void checkSoundIsLoaded();

public:
    static SourceManager* Get();

    // void playSource(ALuint buffer_To_play); // Plays a sound based on buffer, depricated
    void playMyJam(std::string sound_to_play, glm::vec3 position, float max_distance_of_Sound); // Triggers a search for sound's buffer, then plays that at a specific position
    void playMyJam_global(std::string sound_to_play); // plays a sound centered on a person
    
    // make source
    // void checkDistanceBetweenSourceAndListener(); // TODO, check distance and then not assign a source if way too way away

    // Allocate Source
        // Rents out a source to a given game object
    void attachSource (OALSource* s, ALuint buffer_to_play);

    // Unallocate Source
        // Free's a source from a game object
    void CheckAndReleaseOALSource();

};


