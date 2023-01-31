#pragma once
#include <string>
#include <AL/alc.h>
#include <vector>
#include <AL/al.h>
#include <sndfile.h>
#include <stdint.h>
#include <inttypes.h>
#include <AL/alext.h>
#include <vector>

class Audio {

public:
	static Audio* get();
	ALuint AddSound(const char* filename);
	bool RemoveSound(const ALuint& buffer);

	ALuint AddSource();

	void PlaySound(ALuint buffer, ALuint source);

private:
	Audio();
	~Audio();

	std::vector<ALuint> soundEffectBuffers;
	std::vector<ALuint> soundSources;

	ALCdevice* device;
	ALCcontext* context;
 };