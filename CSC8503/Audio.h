#pragma once
#include <string>
#include <AL/alc.h>
#include <vector>
#include <AL/al.h>
#include <sndfile/sndfile.h>
#include <stdint.h>
#include <inttypes.h>
#include <AL/alext.h>
#include <vector>
#include <map>
#include <Assets.h>

namespace NCL
{
	namespace CSC8503
	{
		struct Sound {
			Sound();
			Sound(ALuint* buf, float len) { buffer = buf; length = len; }
			ALuint* buffer;
			float length;
		};
		
		static class Audio {

		public:;
			static void AddSound(const char* filename);
			static Sound* GetSound(std::string filename);

			static bool RemoveSound(std::string filename);

			static void DeleteSounds();
		private:
			Audio();
			~Audio();

			static std::map<std::string, Sound*> soundEffectBuffers;
		};
	}
}