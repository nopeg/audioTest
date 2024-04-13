#include <iostream>

#include <AL/al.h>
#include <AL/alc.h>
#include <vector>

#include "testHeader.h"

#define PI 3.1415926f 

#define TEST_ERROR(_msg)		\
	error = alGetError();		\
	if (error != AL_NO_ERROR) {	\
		fprintf(stderr, _msg "\n");	\
		return -1;		\
	}

short SineWave(unsigned int time, float freq, float ampl)
{
	float period = 44100 / freq;
	float cycles = time / period;
	float radian = 2 * float(PI) * cycles;
	short result = 32767 * ampl * sin(radian);
	return result;
}

static void list_audio_devices(const ALCchar* devices)
{
    const ALCchar* device = devices, * next = devices + 1;
    size_t len = 0;

    std::cout << "Devices list:\n";
    std::cout << "----------\n";
    while (device && *device != '\0' && next && *next != '\0') 
    {
        std::cout << "%s\n" << device;
        len = strlen(device);
        device += (len + 1);
        next += (len + 2);
    }
    std::cout << "----------\n";
}

int main(int argc, char *argv[], char *envp[])
{
    ALboolean enumeration;
    const ALCchar* devices;
    const ALCchar* defaultDeviceName = argv[1];

	char* bufferData;
	ALCdevice* device;
	ALvoid* data;
	ALCcontext* context;
	ALsizei size, freq;
	ALenum format;
	ALuint buffer, source;
	ALfloat listenerOri[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f };
	ALboolean loop = AL_FALSE;
	ALCenum error;
	ALint source_state;

	enumeration = alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT");
	if (enumeration == AL_FALSE)
		fprintf(stderr, "enumeration extension not available\n");

	list_audio_devices(alcGetString(NULL, ALC_DEVICE_SPECIFIER));

	if (!defaultDeviceName)
		defaultDeviceName = alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);

	device = alcOpenDevice(defaultDeviceName);
	if (!device) {
		fprintf(stderr, "unable to open default device\n");
		return -1;
	}

	fprintf(stdout, "Device: %s\n", alcGetString(device, ALC_DEVICE_SPECIFIER));

	alGetError();

	context = alcCreateContext(device, NULL);
	if (!alcMakeContextCurrent(context)) {
		fprintf(stderr, "failed to make default context\n");
		return -1;
	}
	TEST_ERROR("make default context");

	/* set orientation */
	alListener3f(AL_POSITION, 0, 0, 1.0f);
	TEST_ERROR("listener position");
	alListener3f(AL_VELOCITY, 0, 0, 0);
	TEST_ERROR("listener velocity");
	alListenerfv(AL_ORIENTATION, listenerOri);
	TEST_ERROR("listener orientation");

	alGenSources((ALuint)1, &source);
	TEST_ERROR("source generation");

	alSourcef(source, AL_PITCH, 1);
	TEST_ERROR("source pitch");
	alSourcef(source, AL_GAIN, 1);
	TEST_ERROR("source gain");
	alSource3f(source, AL_POSITION, 0, 0, 0);
	TEST_ERROR("source position");
	alSource3f(source, AL_VELOCITY, 0, 0, 0);
	TEST_ERROR("source velocity");
	alSourcei(source, AL_LOOPING, AL_FALSE);
	TEST_ERROR("source looping");

	alGenBuffers(1, &buffer);
	TEST_ERROR("buffer generation");

	int length = 44100 * 2;
	short* samples = new short[length];
	for (int i = 0; i < length; i++) 
	{
		samples[i] = SineWave(i, 440, 0.9);
	}

	alBufferData(buffer, AL_FORMAT_MONO16, samples, length, 44100);
	TEST_ERROR("failed to load buffer data");

	alSourcei(source, AL_BUFFER, buffer);
	TEST_ERROR("buffer binding");

	alSourcePlay(source);
	TEST_ERROR("source playing");

	alGetSourcei(source, AL_SOURCE_STATE, &source_state);
	TEST_ERROR("source state get");
	while (source_state == AL_PLAYING) {
		alGetSourcei(source, AL_SOURCE_STATE, &source_state);
		TEST_ERROR("source state get");
	}

	/* exit context */
	alDeleteSources(1, &source);
	alDeleteBuffers(1, &buffer);
	device = alcGetContextsDevice(context);
	alcMakeContextCurrent(NULL);
	alcDestroyContext(context);
	alcCloseDevice(device);

	return 0;
}