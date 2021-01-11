// Based on https://www.youtube.com/watch?v=fG2veGWNPJY
// git repo: https://github.com/codetechandtutorials/openal-impl
// Thanks Matthew Early for the OpenAL implementation

#pragma once
#include <AL\al.h>
class SoundEffectsPlayer
{
public:
	SoundEffectsPlayer();
	~SoundEffectsPlayer();

	void Play(const ALuint& buffer_to_play);
	void Stop();
	void Pause();
	void Resume();

	void SetBufferToPlay(const ALuint& buffer_to_play);
	void SetLooping(const bool& loop);
	void SetPosition(const float& x, const float& y, const float& z);

	bool isPlaying();

private:
	ALuint p_Source;
	ALuint p_Buffer = 0;
};

