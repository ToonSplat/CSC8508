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
			Sound() {}
			Sound(ALuint* buf, float len) { buffer = buf; length = len; }
			ALuint* buffer;
			float length;
		};
		
		class Audio {

		public:
			static Sound* AddSound(const char* filename);
			static Sound* GetSound(std::string filename);

			static bool RemoveSound(Sound* sound);
		private:
			Audio();
			~Audio() {}
		};
	}
}