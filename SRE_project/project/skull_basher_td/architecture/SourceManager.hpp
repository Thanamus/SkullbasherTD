#pragma once

#include <map>
#include <vector>
#include <AL/al.h>

struct OALSource {
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
    SourceManager(unsigned int num_of_sources = 8);
    ~SourceManager();
    static void Init(); //overwrites the default number of sources to make

    ALuint source;
    std::vector<OALSource*>	sources;
    ALuint p_buffer = 0;
    OALSource*		oalSource;
    float			radius;
    OALSource*	GetSource();
public:
    static SourceManager* Get();

    // play source
    // void playSource(ALuint source_To_Play);
    // const ALuint& buffer_to_play
    void playSource(ALuint buffer_To_play);
    // make source

    // Allocate Source
        // Rents out a source to a given game object
    void attachSource (OALSource* s, ALuint buffer_to_play);
    // Unallocate Source
        // Free's a source from a game object
    void CheckAndReleaseOALSource();

    // Delete all sources
    // std::map<int, ALuint> stuff;

    //array containing references to sources
    // std::array<ALuint> sourceArray(2);
    // std::array<ALuint*, 2> sourceArray;
    // ALuint* sourceArray[2];
    // source : array [0..numsources] of TALuint;

};


