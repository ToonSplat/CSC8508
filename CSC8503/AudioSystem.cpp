#include "AudioSystem.h"

using namespace NCL;
using namespace CSC8503;

AudioSystem* AudioSystem::instance = NULL;

AudioSystem::AudioSystem(unsigned int channels) {
	masterVolume = 1.0f;

	std::cout << " Creating SoundSystem !" << std::endl;

	std::cout << " Found the following devices : " << alcGetString(nullptr, ALC_DEVICE_SPECIFIER) << std::endl;

    const ALCchar* defaultDeviceString = alcGetString(nullptr, ALC_DEFAULT_DEVICE_SPECIFIER);
    device = alcOpenDevice(defaultDeviceString);
    if (!device)
        throw("failed to get device");
    context = alcCreateContext(device, nullptr);
    if (!context)
        throw("failed to create context");
    if (!alcMakeContextCurrent(context))
        throw("failed to make context current");

    const ALCchar* name = nullptr;
    if (alcIsExtensionPresent(device, "ALC_ENUMERATE_ALL_EXT"))
        name = alcGetString(device, ALC_ALL_DEVICES_SPECIFIER);
    if (!name || alcGetError(device) != AL_NO_ERROR)
        name = alcGetString(device, ALC_DEVICE_SPECIFIER);
    printf("Opened \"%s\"\n", name);

    alDistanceModel(AL_LINEAR_DISTANCE_CLAMPED);

    for (unsigned int i = 0; i < channels; ++i) {
        ALuint source;
        alGenSources(1, &source);
        ALenum error = alGetError();

        if (error == AL_NO_ERROR) {
            sources.push_back(new OALSource(source));
        }
        else { break; }
    }
    std::cout << " SoundSystem has " << sources.size()
        << " channels available !" << std::endl;
}

AudioSystem::~AudioSystem() {

    for (std::vector < OALSource* >::iterator i = sources.begin();
        i != sources.end(); ++i) {
        alDeleteSources(1, &(*i)->source);
        delete (*i);
    }
    alcDestroyContext(context);
    alcMakeContextCurrent(NULL);
    alcCloseDevice(device);
}

void AudioSystem::SetMasterVolume(float value) {
    value = std::max(0.0f, value);
    value = std::min(1.0f, value);
    masterVolume = value;
    alListenerf(AL_GAIN, masterVolume);
}

void AudioSystem::UpdateListener() {
    Maths::Vector3 worldPos = listenerTransform.GetPositionVector();

    Maths::Vector3 dirup[2];
    //forward
    dirup[0].x = -listenerTransform.array[0][2];
    dirup[0].y = -listenerTransform.array[1][2];
    dirup[0].z = -listenerTransform.array[2][2];

    //Up
    dirup[1].x = listenerTransform.array[0][1];
    dirup[1].y = listenerTransform.array[1][1];
    dirup[1].z = listenerTransform.array[2][1];

    alListenerfv(AL_POSITION, (float*)&worldPos);
    alListenerfv(AL_ORIENTATION, (float*)&dirup);
}

void AudioSystem::Update(float msec) {
    UpdateListener();

    for (int i = 0; i < emitters.size(); i++) {
        if (!emitters.at(i) || (emitters.at(i)->GetTimeLeft() <= 0.0f && !emitters.at(i)->GetLooping())) {
            emitters.erase(emitters.begin() + i);
        }
        //emitters.at(i)->Update(msec);
    }

    for (std::vector <AudioEmitter* >::iterator i = emitters.begin();
        i != emitters.end(); ++i) {
        frameEmitters.push_back((*i));
        (*i)->Update(msec);
    }

    CullNodes();

    if (frameEmitters.size() > sources.size()){
        std::sort(frameEmitters.begin(), frameEmitters.end(),
            AudioEmitter::CompareNodesByPriority);

        DetachSources(frameEmitters.begin() + (sources.size() + 1),
            frameEmitters.end());
        AttachSources(frameEmitters.begin(),
            frameEmitters.begin() + sources.size());
    }
    else {
        AttachSources(frameEmitters.begin(), frameEmitters.end());
    }
    frameEmitters.clear();
}

void AudioSystem::CullNodes() {

    for (std::vector<AudioEmitter* >::iterator i = frameEmitters.begin();
        i != frameEmitters.end(); ) {
        AudioEmitter* e = (*i);
        float length = (listenerTransform.GetPositionVector() - e->GetPosition()).Length();

        if (length > e->GetRadius() || !e->GetSound() || e->GetTimeLeft() < 0 || (mainMenu && e->GetPriority() < SoundPriority::ALWAYS)) {
            e->DetachSource();
            i = frameEmitters.erase(i);
        }
        else {
            ++i;
        }
    }
}

void AudioSystem::DetachSources(std::vector<AudioEmitter* >::iterator from,
    std::vector < AudioEmitter* >::iterator to) {
    for (std::vector < AudioEmitter* >::iterator i = from; i != to; ++i) {
        (*i) -> DetachSource();
    }
}

void AudioSystem::AttachSources(std::vector < AudioEmitter* >::iterator from,
    std::vector <AudioEmitter* >::iterator to) {
    for (std::vector < AudioEmitter* >::iterator i = from; i != to; ++i) {
        if (!(*i) -> GetSource()) {
            (*i) -> AttachSource(GetSource());
        }
    }
}

OALSource* AudioSystem::GetSource() {
    for (std::vector < OALSource* >::iterator i = sources.begin(); i != sources.end(); ++i) {
        OALSource* s = *i;
        if (!s -> inUse) {
            return s;
        }
    }
    return NULL;
}
