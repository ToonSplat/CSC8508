#include "Audio.h"

using namespace NCL;
using namespace CSC8503;

void Audio::DeleteSounds() {
    for (std::map<std::string, Sound*>::iterator it = soundEffectBuffers.begin(); it != soundEffectBuffers.end(); ++it) {
        alDeleteBuffers(1, it->second->buffer);
        delete it->second->buffer;
        delete it->second;
    }
    soundEffectBuffers.clear();
}

void Audio::AddSound(const char* filename)
{
    ALenum err, format;
    ALuint* buffer = new ALuint;
    SNDFILE* sndfile;
    SF_INFO sfinfo;
    short* membuf;
    sf_count_t num_frames;
    ALsizei num_bytes;
    
    std::string fullFilePath(NCL::Assets::SOUNDSDIR + filename);
    const char* filepath = fullFilePath.c_str();
    
    
    /* Open the audio file and check that it's usable. */
    sndfile = sf_open(filepath, SFM_READ, &sfinfo);
    if (!sndfile)
    {
        fprintf(stderr, "Could not open audio in %s: %s\n", filepath, sf_strerror(sndfile));
        return;
    }
    if (sfinfo.frames < 1 || sfinfo.frames >(sf_count_t)(INT_MAX / sizeof(short)) / sfinfo.channels)
    {
        fprintf(stderr, "Bad sample count in %s (%" PRId64 ")\n", filepath, sfinfo.frames);
        sf_close(sndfile);
        return;
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
        return;
    }

    /* Decode the whole audio file to a buffer. */
    membuf = static_cast<short*>(malloc((size_t)(sfinfo.frames * sfinfo.channels) * sizeof(short)));

    num_frames = sf_readf_short(sndfile, membuf, sfinfo.frames);
    if (num_frames < 1)
    {
        free(membuf);
        sf_close(sndfile);
        fprintf(stderr, "Failed to read samples in %s (%" PRId64 ")\n", filepath, num_frames);
        return;
    }


    num_bytes = (ALsizei)(num_frames * sfinfo.channels) * (ALsizei)sizeof(short);
    float len = (float)sfinfo.frames / (float)sfinfo.samplerate;
    /* Buffer the audio data into a new buffer object, then free the data and
     * close the file.
     */

    alGenBuffers(1, buffer);
    alBufferData(*buffer, format, membuf, num_bytes, sfinfo.samplerate);

    free(membuf);
    sf_close(sndfile);

    /* Check if an error occured, and clean up if so. */
    err = alGetError();
    if (err != AL_NO_ERROR)
    {
        fprintf(stderr, "OpenAL Error: %s\n", alGetString(err));
        if (buffer && alIsBuffer(*buffer))
            alDeleteBuffers(1, buffer);
        return;
    }
   
    Sound* s = new Sound(buffer, len);
   soundEffectBuffers.insert({ filename,s});
}

bool Audio::RemoveSound(std::string filename) {
    alDeleteBuffers(1, soundEffectBuffers.find(filename)->second->buffer);
    delete soundEffectBuffers.find(filename)->second->buffer;
    delete soundEffectBuffers.find(filename)->second;
    soundEffectBuffers.erase(filename);
    return true;
}

Sound* Audio::GetSound(std::string filename) {
    return soundEffectBuffers.find(filename)->second;
}
