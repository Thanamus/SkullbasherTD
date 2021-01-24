/* Based on https://www.youtube.com/watch?v=fG2veGWNPJY
 git repo: https://github.com/codetechandtutorials/openal-impl
 Thanks Matthew Early for the OpenAL implementation

	New additions include:

*/

#pragma once
#include <AL/alc.h>
#include <AL/al.h>


static void ALC_CheckAndThrow(ALCdevice* device)
{
	if (alcGetError(device) != ALC_NO_ERROR)
	{
		throw("error with alcDevice");
	}
}


static void AL_CheckAndThrow()
{
	if (alGetError() != AL_NO_ERROR)
	{
		throw("error with al");
	}
}