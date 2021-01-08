/*
    Based on the tutorials from: https://www.youtube.com/watch?v=kWQM1iQ1W0E
*/


#pragma once

// #include <AL\alc.h> //ALC means Audio Library
#include <AL/alc.h> //ALC means Audio Library


class SoundDevice {
public:
    static SoundDevice* get(); //sets up the sound device as a singleton, to use multiple devices we will need to change the design pattern
private:
    SoundDevice();
    ~SoundDevice();

    ALCdevice* p_ALCDevice;
    ALCcontext* p_ALCContext;
};
