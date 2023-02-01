#include "Audio.h"

Audio* Audio::get() {
    static Audio* snd_divice = new Audio();
    return snd_divice;
}

Audio::Audio() {

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

}

Audio::~Audio() {

    alDeleteSources(soundSources.size(), soundSources.data());
    soundSources.clear();
    for (std::map<std::string, ALuint>::iterator it = soundEffectBuffers.begin(); it != soundEffectBuffers.end(); ++it) {
        soundSources.push_back(it->second);
    }
    alDeleteBuffers(soundSources.size(), soundSources.data());
   
    soundEffectBuffers.clear();
    soundSources.clear();

    if (!alcMakeContextCurrent(nullptr))
        throw("failed to make context not-current");
    alcDestroyContext(context);
    alcCloseDevice(device);
}

ALuint Audio::AddSound(const char* filename)
{
    ALenum err, format;
    ALuint buffer;
    SNDFILE* sndfile;
    SF_INFO sfinfo;
    short* membuf;
    sf_count_t num_frames;
    ALsizei num_bytes;
    
    std::string fullFilePath(NCL::Assets::SOUNDSDIR + filename);
    char* filepath;
    strcpy(filepath, fullFilePath.c_str());
    
    

    /* Open the audio file and check that it's usable. */
    sndfile = sf_open(filepath, SFM_READ, &sfinfo);
    if (!sndfile)
    {
        fprintf(stderr, "Could not open audio in %s: %s\n", filepath, sf_strerror(sndfile));
        return 0;
    }
    if (sfinfo.frames < 1 || sfinfo.frames >(sf_count_t)(INT_MAX / sizeof(short)) / sfinfo.channels)
    {
        fprintf(stderr, "Bad sample count in %s (%" PRId64 ")\n", filepath, sfinfo.frames);
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
        fprintf(stderr, "Failed to read samples in %s (%" PRId64 ")\n", filepath, num_frames);
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

    this->soundEffectBuffers.insert({ filepath, buffer});

    return buffer;
}

bool Audio::RemoveSound(const ALuint& buffer) {
    for (std::map<std::string, ALuint>::iterator it = soundEffectBuffers.begin(); it != soundEffectBuffers.end(); ++it) {
        if (buffer != it->second) {
            soundEffectBuffers.erase(it->first);
            alDeleteBuffers(1,&buffer);
            return true;
        }
    }
   
    return false;
}
bool Audio::RemoveSound(std::string filename) {
    alDeleteBuffers(1, &soundEffectBuffers.find(filename)->second);
    soundEffectBuffers.erase(filename);
    return true;
}


ALuint Audio::AddSource() {
    ALuint source;
    alGenSources(1, &source);
    alSourcef(source, AL_PITCH, 1);
    alSourcef(source, AL_GAIN, 1.0f);
    alSource3f(source, AL_POSITION, 0, 0, 0);
    alSource3f(source, AL_VELOCITY, 0, 0, 0);
    alSourcei(source, AL_LOOPING, AL_TRUE);

    this->soundSources.push_back(source);
    return source;
}

void PlaySound(ALuint buffer, ALuint source) {
    alSourcei(source, AL_BUFFER, buffer);
    alSourcePlay(source);
}
