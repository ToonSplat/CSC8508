#pragma once
#include <AL/al.h>
#include <AL/alc.h>
#include <Vector3.h>
#include <algorithm> 
#include "Audio.h"

using namespace std;
namespace NCL 
{
	namespace CSC8503 {
		enum SoundPriority {
			LOW,
			MEDIUM,
			HIGH,
			ALWAYS
		};

		struct OALSource
		{
			ALuint source;
			bool inUse;

			OALSource(ALuint src) {
				source = src;
				inUse = false;
			}
		};

		class AudioEmitter {

		private:
			Sound* sound;
			SoundPriority priority;
			Maths::Vector3 position;
			Maths::Vector3 velocity;
			bool isLooping;
			bool paused;
			float volume;
			float radius;
			float  timeLeft;
			OALSource* currentSource;

		public:
			

			AudioEmitter();
			AudioEmitter(Sound* sound);
			~AudioEmitter();

			void Play();
			void Pause();


			void Reset();

			bool GetStatus() { return paused; }

			void SetSound(Sound* s);
			inline Sound* GetSound() { return sound; };

			void SetTarget(Maths::Vector3 pos) { position = pos; }
			Maths::Vector3 GetPosition() { return position; };

			inline void SetPriority(SoundPriority p) { priority = p; };
			inline SoundPriority GetPriority() { return priority; }

			inline void SetVolume(float volume) {
				volume = min(1.0f, max(0.0f, volume));
			}
			
			inline float GetVolume() { return volume; }

			inline void SetLooping(bool state) { isLooping = state; }
			inline bool GetLooping() { return isLooping; }

			inline void SetRadius(float value) {
				radius = max(0.0f, value);
			}
			inline float GetRadius() { return radius; }

			inline float GetTimeLeft() { return timeLeft; }

			inline OALSource* GetSource() { return currentSource; }

			void AttachSource(OALSource* s);
			void DetachSource();

			static bool CompareNodesByPriority(AudioEmitter* a, AudioEmitter* b);

			virtual void Update(float msec);
		};
	}
}
