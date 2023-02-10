#include "AudioEmitter.h"

using namespace NCL;
using namespace CSC8503;

NCL::CSC8503::AudioEmitter::AudioEmitter(){
    Reset();
}

NCL::CSC8503::AudioEmitter::AudioEmitter(Sound* sound) {
    Reset();
    SetSound(sound);
}

void AudioEmitter::Reset() {
	paused = false;
	sound = NULL;
	priority = LOW;
	Maths::Vector3 position = Maths::Vector3(0, 0, 0);
	Maths::Vector3 velocity = Maths::Vector3(0, 0, 0);
	isLooping = true;
	volume = 1.0f;
	radius = 500.0f;
	timeLeft = 0.0f;
	currentSource = NULL;
}

AudioEmitter::~AudioEmitter() {
	DetachSource();
}

bool AudioEmitter::CompareNodesByPriority(AudioEmitter* a,
	AudioEmitter* b) {
	return (a->priority > b->priority) ? true : false;
}
void AudioEmitter::SetSound(Sound* s) {
	sound = s;
	DetachSource();
	if (s) {
		timeLeft = s->length;
	}
}

void AudioEmitter::AttachSource(OALSource* s) {
	currentSource = s;

	if (!currentSource)
		return;

	currentSource->inUse = true;

	alSourcef(currentSource->source, AL_MAX_DISTANCE, radius);
	alSourcef(currentSource->source, AL_REFERENCE_DISTANCE, radius * 0.2f);
	alSourcei(currentSource->source, AL_BUFFER, *sound->buffer);

	Play();
	//alSourcef(currentSource->source, AL_SEC_OFFSET, (sound->length / 1000.0f) - (timeLeft / 1000.0f));
	//alSourcePlay(currentSource->source);
}

void AudioEmitter::DetachSource() {
	if (!currentSource)
		return;
	alSourcef(currentSource->source, AL_GAIN, 0.0f);
	alSourceStop(currentSource->source);
	alSourcei(currentSource->source, AL_BUFFER, 0);

	currentSource->inUse = false;
	currentSource = NULL;
}

void AudioEmitter::Update(float msec) {
	if(!paused)
		timeLeft -= msec;
	while (isLooping && timeLeft < 0.0f )
		timeLeft += sound->length;
	if (currentSource) {
		
		alSourcefv(currentSource->source, AL_POSITION, (float*)&position);

		alSourcef(currentSource->source, AL_GAIN, volume);
		alSourcei(currentSource->source, AL_LOOPING, isLooping ? 1 : 0);
		alSourcef(currentSource->source, AL_REFERENCE_DISTANCE, radius * 0.2f);
	}
}

void  AudioEmitter::Play() {
	if (currentSource) {
		paused = false;
		alSourcef(currentSource->source, AL_SEC_OFFSET, sound->length  - timeLeft );
		alSourcePlay(currentSource->source);
	}
}

void  AudioEmitter::Pause() {
	if (currentSource) {
		alSourcePause(currentSource->source);
		paused = true;
	}
}
