// Based on https://www.youtube.com/watch?v=fG2veGWNPJY
// git repo: https://github.com/codetechandtutorials/openal-impl
// Thanks Matthew Early for the OpenAL implementation

#include "MusicBuffer.hpp"
#include <cstddef>
#include <AL\alext.h>
#include <malloc.h>
#include "../sound/OpenALErrorCheck.hpp"
#include <iostream> //for cout

#include <inttypes.h>

static MusicBuffer* _instance = nullptr;
MusicBuffer * MusicBuffer::Get(){
	Init();
	return _instance;
}

void MusicBuffer::Init(){
	if (_instance == nullptr) _instance = new MusicBuffer();
}

void MusicBuffer::Load(const char* filename){
	m_filename = filename;
	OpenDecoder();
}


void MusicBuffer::Play()
{
	std::cout << "starting to play music: " << p_Source << "\n";
	ALsizei i;

	// clear any al errors
	alGetError();

	/* Rewind the source position and clear the buffer queue */
	alSourceRewind(p_Source);
	alSourcei(p_Source, AL_BUFFER, 0);

	/* Fill the buffer queue */
	for (i = 0; i < NUM_BUFFERS; i++)
	{
		/* Get some data to give it to the buffer */
		sf_count_t slen = sf_readf_short(p_SndFile, p_Membuf, BUFFER_SAMPLES);
		if (slen < 1) break;

		slen *= p_Sfinfo.channels * (sf_count_t)sizeof(short);
		alBufferData(p_Buffers[i], p_Format, p_Membuf, (ALsizei)slen, p_Sfinfo.samplerate);
	}
	if (alGetError() != AL_NO_ERROR)
	{
		throw("Error buffering for playback");
	}

	/* Now queue and start playback! */
	alSourceQueueBuffers(p_Source, i, p_Buffers);
	alSourcePlay(p_Source);
	if (alGetError() != AL_NO_ERROR)
	{
		throw("Error starting playback");
	}

}

void MusicBuffer::Pause()
{
	alSourcePause(p_Source);
	AL_CheckAndThrow();
}

void MusicBuffer::Stop()
{
	alSourceStop(p_Source);
	AL_CheckAndThrow();
}

void MusicBuffer::Resume()
{
	alSourcePlay(p_Source);
	AL_CheckAndThrow();
}

void MusicBuffer::UpdateBufferStream()
{
	ALint processed, state;

	// clear error 
	alGetError();

	/* Get relevant source info */
	alGetSourcei(p_Source, AL_SOURCE_STATE, &state);
	alGetSourcei(p_Source, AL_BUFFERS_PROCESSED, &processed);
	AL_CheckAndThrow();

	/* Unqueue and handle each processed buffer */
	while (processed > 0)
	{
		ALuint bufid;
		sf_count_t slen;

		alSourceUnqueueBuffers(p_Source, 1, &bufid);
		processed--;

		/* Read the next chunk of data, refill the buffer, and queue it
		 * back on the source */
		slen = sf_readf_short(p_SndFile, p_Membuf, BUFFER_SAMPLES);
		if (slen > 0)
		{
			slen *= p_Sfinfo.channels * (sf_count_t)sizeof(short);
			alBufferData(bufid, p_Format, p_Membuf, (ALsizei)slen,
				p_Sfinfo.samplerate);
			alSourceQueueBuffers(p_Source, 1, &bufid);
		}
		if (alGetError() != AL_NO_ERROR)
		{
			throw("error buffering music data");
		}
	}

	/* Make sure the source hasn't underrun */
	if (state != AL_PLAYING && state != AL_PAUSED)
	{
		ALint queued;

		/* If no buffers are queued, playback is finished */
		alGetSourcei(p_Source, AL_BUFFERS_QUEUED, &queued);
		AL_CheckAndThrow();
		if (queued == 0) {
			// If Queue is empty, the sound file has reached the end
			// Restart music
			std::cout << "attempting to restart music" << std::endl;
			ResetDecoders(); // To restart means to reload the music file from the start - i.e. start from scratch
			Play(); // Gets the music to start playing again after being re-queued
			return; // return
		}
		// if (queued == 0)
		// 	return;

		alSourcePlay(p_Source);
		AL_CheckAndThrow();

	}
	if (changingTracks == true)
	{
		/* 	If music is in the process of changing tracks
			smoothly fade out the first track */
		fadeOutMusic();
	}
	

}

ALint MusicBuffer::getSource()
{
	// returns the id of the source used by OpenAL for the music
	return p_Source;
}

bool MusicBuffer::isPlaying()
{
	// Queries the playing state of the source (i.e. the music)
	ALint state;
	alGetSourcei(p_Source, AL_SOURCE_STATE, &state);
	AL_CheckAndThrow();
	return (state == AL_PLAYING);
}

void MusicBuffer::SetGain(const float& val)
{
	// sets the gain of the music, but does not ovverride the original gain
	// meaning only the current music is affected 
	float newval = val;
	if (newval < 0)
		newval = 0;
	alSourcef(p_Source, AL_GAIN, val);
	AL_CheckAndThrow();
}

// MusicBuffer::MusicBuffer(const char* filename)
MusicBuffer::MusicBuffer()
{
	// Empyty now, as the Init() takes care of opening the decoder and saving the filename
	// m_filename = filename;
	// OpenDecoder();
}

MusicBuffer::~MusicBuffer()
{
	CloseDecoder(); // close out the MusicBuffers sourcem unload stuff from memory etc.
}


void MusicBuffer::OpenDecoder(){
	// Open decoder sets up the source (i.e. the 'thing' that plays music)
	// it generates the buffers to play on.

	alGenSources(1, &p_Source); // Open the source (one source), and return the source ID used by OpenAL into p_Source
	alGenBuffers(NUM_BUFFERS, p_Buffers); // Generate buffers that the music will be streamed into -> NUM_BUFFERS determines how many buffers are in rotation
	// If we're desperate for memory, we could try out different numbers of NUM_BUFFERS, to make sure we don't have any under-runs

	std::size_t frame_size;

	/* Open the audio file and check that it's usable. */
	p_SndFile = sf_open(m_filename, SFM_READ, &p_Sfinfo); // Filename, read the file and extract the sound related information.
	if (!p_SndFile)
	{
		// Error check that the audio file could be opened and read correctly
		fprintf(stderr, "Could not open audio in %s: %s\n", m_filename, sf_strerror(p_SndFile));
		return;
	}

	/* Checks that the audio file is usable */
	if (p_Sfinfo.frames < 1 || p_Sfinfo.frames >(sf_count_t)(INT_MAX / sizeof(short)) / p_Sfinfo.channels)
	{
		fprintf(stderr, "Bad sample count in %s (%" PRId64 ")\n", m_filename, p_Sfinfo.frames);
		sf_close(p_SndFile);
		return;
	}

	/* Get the sound format (MONO, STERO, number of channels etc), and figure out the OpenAL format */
	p_Format = AL_NONE; // Reset format
	if (p_Sfinfo.channels == 1)
		p_Format = AL_FORMAT_MONO16;
	else if (p_Sfinfo.channels == 2)
		p_Format = AL_FORMAT_STEREO16;
	else if (p_Sfinfo.channels == 3)
	{
		if (sf_command(p_SndFile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
			p_Format = AL_FORMAT_BFORMAT2D_16;
	}
	else if (p_Sfinfo.channels == 4)
	{
		if (sf_command(p_SndFile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
			p_Format = AL_FORMAT_BFORMAT3D_16;
	}

	if (!p_Format)
	{
		// Error checking for the channel count
		sf_close(p_SndFile);
		p_SndFile = NULL;
		throw("Unsupported channel count from file");
	}

	/* Allocate memory for the Music*/
	frame_size = ((size_t)BUFFER_SAMPLES * (size_t)p_Sfinfo.channels) * sizeof(short);
	p_Membuf = static_cast<short*>(malloc(frame_size));

	// set Gain on source
	alSourcef(p_Source, AL_GAIN, originalGain);
}

void MusicBuffer::CloseDecoder(){
	/* Clean up sources, close the file, free the memory buffers*/
	// Clean up sources
	alDeleteSources(1, &p_Source);

	// Close the audio file
	if (p_SndFile)
		sf_close(p_SndFile);
	p_SndFile = nullptr;

	// Free memory and buffers
	free(p_Membuf);
	alDeleteBuffers(NUM_BUFFERS, p_Buffers);
}

void MusicBuffer::ResetDecoders(){
	/* Close everything, then re-open from scratch*/
	CloseDecoder();
	OpenDecoder();
}

void MusicBuffer::changeTracks(const char* filename){
	/* Triggers a change of tracks */

	// set changing tracks to true
	changingTracks = true; // sets the flag to true so the update function can fade out

	// set music track to be
	m_filename_to_be = filename;

	// Set the start time for checking in fadeOutMusic()
	start_time = std::chrono::steady_clock::now();
	// fade out float t = now - stopTime;
}

void MusicBuffer::fadeOutMusic() {
	/* 	Gracefully fades out music over a predetermined time
		Fade out is set in the header file, but in future we could make a function
		that accepts fade time as a prameter (eg, fast change on dramatic movement, slow on scenery change)
		
		- fadeOutMusic() could also be optimised in terms of the calculation for fading
		- we could also fade in the new music too if desired, but this would require a second OpenAL source to be instanced
	*/
	

	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now(); // Get the current time point
	elapsedTimeInSec = std::chrono::duration_cast<std::chrono::seconds>(now - start_time).count(); // Convert time point into elapsed seconds form start time

	float elapsedTimeInMillisec = std::chrono::duration_cast<std::chrono::milliseconds>(now - start_time).count(); // Convert to float milliseconds for calcs later
	float fade_out_time_milli = fade_out_time * 1000; // Convert fade out to millisecs for calcs

	// std::cout << "elapsed time of fade out: " << elapsedTimeInSec << std::endl;

	if (elapsedTimeInMillisec > fade_out_time_milli)
	{
		// time's up, fade completed.

		// close Decoder
		CloseDecoder();
		// Swap to be track to current track

		m_filename = m_filename_to_be;

		// reopen decoder
		OpenDecoder();
		
		// reset changing tracks state
		changingTracks = false;
		// std::cout << "tracks changed" << std::endl;
	} else{
		// Continue fading out the sound

		/* reduce the gain on the current music by the amount of time remaining */
		alSourcef(p_Source, AL_GAIN, originalGain*std::fabs(1.0f - (elapsedTimeInMillisec / fade_out_time_milli)));
		// std::cout << "fading out: " << originalGain*std::fabs(1.0f - (elapsedTimeInMillisec / fade_out_time_milli)) << std::endl;
		
	}
	
}