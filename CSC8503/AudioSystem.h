#pragma once
#include <AL/al.h>
#include <AL/alc.h>
#include <vector>
#include <algorithm>
#include "AudioEmitter.h"
#include "Matrix4.h"


namespace NCL {
	namespace CSC8503 {
		

		class AudioSystem {
		public:
			static void Initialise(unsigned int channels = 32) {
				if (!instance) { instance = new AudioSystem(channels);}
			}

			static void Destroy() { delete instance;}

			inline static AudioSystem* GetAudioSystem() { return instance; }

			void SetListenerTransform(const Maths::Matrix4& transform) {
				listenerTransform = transform;
			}
			NCL::Maths::Matrix4 GetListenerTransform() {
				return listenerTransform;
			}
			void AddSoundEmitter(AudioEmitter* s) { if(s)emitters.push_back(s); }
			void RenoveAudioEmitter(AudioEmitter* s);

			void Update(float msec);

			void SetMasterVolume(float value);
			void SetMainMenuStatus(bool state) { mainMenu = state; };

		private:
			Maths::Matrix4 listenerTransform;
			float masterVolume;
			ALCcontext* context;
			ALCdevice* device;

			bool mainMenu;

			std::vector<OALSource*> sources;
			std::vector<AudioEmitter*> emitters;
			std::vector<AudioEmitter*> frameEmitters;

			static AudioSystem* instance;

			AudioSystem(unsigned int channels = 32);
			~AudioSystem();

			void UpdateListener();
			void UpdateTemporaryEmitters(float msec);

			void DetachSources(std::vector<AudioEmitter*>::iterator from, std::vector<AudioEmitter*>::iterator to);
			void AttachSources(std::vector <AudioEmitter* >::iterator from, std::vector <AudioEmitter* >::iterator to);

			void CullNodes();

			OALSource* GetSource();
		};
	}
}

