/* 	Based on https://www.youtube.com/watch?v=fG2veGWNPJY
 	git repo: https://github.com/codetechandtutorials/openal-impl
 	Thanks Matthew Early for the OpenAL implementation

	New additions include:

*/

#include "SoundEffectsLibrary.hpp"
#include <sndfile.h>
#include <inttypes.h>
#include <AL\alext.h>

#include <iostream>
#include <string>

/// <summary>
/// Static class access.
/// </summary>
/// <returns>A pointer to the only instantiation allowed.</returns>
SoundEffectsLibrary* SoundEffectsLibrary::Get()
{
	static SoundEffectsLibrary* sndbuf = new SoundEffectsLibrary();
	return sndbuf;
}

/// <summary>
/// Loads the sound file into memory.
/// </summary>
/// <param name="filename">path to the file to load</param>
/// <returns>access id</returns>
ALuint SoundEffectsLibrary::Load(const char* filename)
{

	// Get base filename and load it into the map
	// filename = "C:\\MyDirectory\\MyFile.bat";
	std::string filenameExtract = filename;
	// Remove directory if present.
	// Do this before extension removal incase directory has a period character.
	const size_t last_slash_idx = filenameExtract.find_last_of("\\/");
	if (std::string::npos != last_slash_idx)
	{
		filenameExtract.erase(0, last_slash_idx + 1);
	}

// from: https://stackoverflow.com/questions/1939953/how-to-find-if-a-given-key-exists-in-a-c-stdmap
	if ( loadedSounds.find(filenameExtract) == loadedSounds.end() ) {
		// not found
		// // Remove extension if present.
		// const size_t period_idx = filenameExtract.rfind('.');
		// if (std::string::npos != period_idx)
		// {
		// 	filenameExtract.erase(period_idx);

		// }


		ALenum err, format;
		ALuint buffer;
		SNDFILE* sndfile;
		SF_INFO sfinfo;
		short* membuf;
		sf_count_t num_frames;
		ALsizei num_bytes;

		/* Open the audio file and check that it's usable. */
		sndfile = sf_open(filename, SFM_READ, &sfinfo);
		if (!sndfile)
		{
			fprintf(stderr, "Could not open audio in %s: %s\n", filename, sf_strerror(sndfile));
			return 0;
		}
		if (sfinfo.frames < 1 || sfinfo.frames >(sf_count_t)(INT_MAX / sizeof(short)) / sfinfo.channels)
		{
			fprintf(stderr, "Bad sample count in %s (%" PRId64 ")\n", filename, sfinfo.frames);
			sf_close(sndfile);
			return 0;
		}

		/* Get the sound format, and figure out the OpenAL format */
		format = AL_NONE;
		if (sfinfo.channels == 1)
			format = AL_FORMAT_MONO16;
		else if (sfinfo.channels == 2)
			format = AL_FORMAT_STEREO16;
		else if (sfinfo.channels == 3)
		{
			if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
				format = AL_FORMAT_BFORMAT2D_16;
		}
		else if (sfinfo.channels == 4)
		{
			if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
				format = AL_FORMAT_BFORMAT3D_16;
		}
		if (!format)
		{
			fprintf(stderr, "Unsupported channel count: %d\n", sfinfo.channels);
			sf_close(sndfile);
			return 0;
		}

		/* Decode the whole audio file to a buffer. */
		membuf = static_cast<short*>(malloc((size_t)(sfinfo.frames * sfinfo.channels) * sizeof(short)));

		num_frames = sf_readf_short(sndfile, membuf, sfinfo.frames);
		if (num_frames < 1)
		{
			free(membuf);
			sf_close(sndfile);
			fprintf(stderr, "Failed to read samples in %s (%" PRId64 ")\n", filename, num_frames);
			return 0;
		}
		num_bytes = (ALsizei)(num_frames * sfinfo.channels) * (ALsizei)sizeof(short);

		/* Buffer the audio data into a new buffer object, then free the data and
		* close the file.
		*/
		buffer = 0;
		alGenBuffers(1, &buffer);
		alBufferData(buffer, format, membuf, num_bytes, sfinfo.samplerate);

		free(membuf);
		sf_close(sndfile);

		/* Check if an error occured, and clean up if so. */
		err = alGetError();
		if (err != AL_NO_ERROR)
		{
			fprintf(stderr, "OpenAL Error: %s\n", alGetString(err));
			if (buffer && alIsBuffer(buffer))
				alDeleteBuffers(1, &buffer);
			return 0;
		}

		p_SoundEffectBuffers.push_back(buffer);  // add to the list of known buffers


		// loadedSounds.emplace(filenameExtract, buffer);
		loadedSounds[filenameExtract] = (int)buffer;
		std::cout << "just loaded: " << filenameExtract << std::endl;
		std::cout << "just loaded buffer: " << buffer << std::endl;
		return buffer;
	} else {
		// found
		ALuint buffer = loadedSounds[filenameExtract]; 
		std::cout << "file already loaded, it's buffer is: " <<  buffer << std::endl;
		return buffer;
	}

}

/// <summary>
/// Unloads the sound file from memory.
/// </summary>
/// <param name="buffer"></param>
/// <returns></returns>
bool SoundEffectsLibrary::UnLoad(const ALuint& buffer)
{
	auto it = p_SoundEffectBuffers.begin();
	while (it != p_SoundEffectBuffers.end())
	{
		if (*it == buffer)
		{
			alDeleteBuffers(1, &*it);

			it = p_SoundEffectBuffers.erase(it);

			return true;
		}
		else {
			++it;
		}
	}
	return false;  // couldn't find to remove
}

/// <summary>
/// Class initialization.
/// </summary>
SoundEffectsLibrary::SoundEffectsLibrary()
{
	p_SoundEffectBuffers.clear();
}

/// <summary>
/// Class Destructor. Removes all loaded sounds from memory.
/// </summary>
SoundEffectsLibrary::~SoundEffectsLibrary()
{
	alDeleteBuffers((ALsizei)p_SoundEffectBuffers.size(), p_SoundEffectBuffers.data());

	p_SoundEffectBuffers.clear();
}


bool SoundEffectsLibrary::retrive_buffer_of_loaded_sound_effect(std::string sound_to_retrive, ALuint* const &returned_buffer){
	// retrives the buffer (where the sound 'lives') of a sound based on it's name
	// name = filename + file extension, e.g. pestilence.ogg
	// returns a reference to the buffer to the caller 

	// Look trough map to find name
	if (loadedSounds.find(sound_to_retrive) == loadedSounds.end() ) {
		// not found, return false
		return false;
	} else {
	// sound found, return the buffer
		*returned_buffer = loadedSounds[sound_to_retrive];
		return true;
	}
	
}