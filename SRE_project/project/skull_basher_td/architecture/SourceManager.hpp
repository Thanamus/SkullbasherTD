#pragma once

#include <map>
#include <AL/al.h>

class SourceManager
{
private:
    /* data */
    SourceManager(/* args */);
    ~SourceManager();
    static void Init();

    ALuint source;
    ALuint p_buffer = 0;
public:
    static SourceManager* Get();

    // play source
    // void playSource(ALuint source_To_Play);
    // const ALuint& buffer_to_play
    void playSource(ALuint buffer_To_play);
    // make source
    // Allocate Source
        // Rents out a source to a given game object
    // Unallocate Source
        // Free's a source from a game object

    // Delete all sources
    std::map<int, ALuint> stuff;
};


