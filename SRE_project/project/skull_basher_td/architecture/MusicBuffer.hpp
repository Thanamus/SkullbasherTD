// Based on https://www.youtube.com/watch?v=fG2veGWNPJY
// git repo: https://github.com/codetechandtutorials/openal-impl
// Thanks Matthew Early for the OpenAL implementation

#pragma once
#include <AL\al.h>
#include <sndfile.h>
#include <chrono>

class MusicBuffer
{
public:
	void Play();
	void Pause();
	void Stop();
	void Resume();

	void UpdateBufferStream();

	ALint getSource();

	bool isPlaying();

	void SetGain(const float& val);

	void changeTracks(const char* filename);

	MusicBuffer(const char* filename);
	~MusicBuffer();
private:
	ALuint p_Source;
	static const int BUFFER_SAMPLES = 8192;
	static const int NUM_BUFFERS = 4;
	ALuint p_Buffers[NUM_BUFFERS];
	SNDFILE* p_SndFile;
	SF_INFO p_Sfinfo;
	short* p_Membuf;
	ALenum p_Format;

	const char* m_filename;

	const char* m_filename_to_be;
	bool changingTracks = false;
	std::chrono::steady_clock::time_point start_time;
	// std::chrono::steady_clock::time_point fade_out_Time;
	int fade_out_time = 1;
	int elapsedTimeInSec = 0;
	void fadeOutMusic();
	float originalGain = 1.0f;

	MusicBuffer() = delete;

	void ResetDecoders();
	void OpenDecoder();
	void CloseDecoder();
};

