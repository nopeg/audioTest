#include <iostream>
#include <AL/al.h>
#include <AL/alc.h>
#include <vector>
#include <random>

#include <chrono>

#include "testHeader.h"

#define PI 3.1415926f 
#define SAMPLE_RATE 44100
#define SEC SAMPLE_RATE * 2

using std::cout;
using std::endl;

//error handling
static void AL_ERROR(const char* msg)
{
	ALenum error = alGetError();
	if (error != AL_NO_ERROR)
	{
		fprintf(stderr, msg, "\n");
		exit(-1);
	}
}

int randRange(int min, int max)
{
	std::random_device randDevice;
	std::mt19937_64 generator(randDevice());
	std::uniform_int_distribution<int> dist(min, max);
	return dist(randDevice);
}
float randRange(float min, float max)
{
	std::random_device randDevice;
	std::mt19937_64 generator(randDevice());
	std::uniform_real_distribution<float> dist(min, max);
	return dist(randDevice);
}

//returns random element of vector
template <class T>
inline T randPick(std::vector<T> vector)
{
	if (vector.size() > 0)
	{
		int r = randRange(0, vector.size() - 1);
		return vector[r];
	}
}


//wave generator
short Sine(unsigned int time, const float freq, const float ampl)
{
	float period = SAMPLE_RATE / freq;
	float cycles = time / period;
	float radian = 2 * float(PI) * cycles;
	short result = 32767 * ampl * sin(radian);
	return result;
}

short Triangle(unsigned int time, float freq, float ampl)
{
	float period = SAMPLE_RATE / freq;
	float cycles = time / period;
	float value = 4.0f * fabs(cycles - floor(cycles + 0.5f)) - 1.0f;
	short result = 32767 * ampl * value;
	return result;
}

short Sawtooth(unsigned int time, float freq, float ampl)
{
	int period = SAMPLE_RATE / freq;
	int cyclesM = time % period;
	short result = 1.5f * freq * ampl * cyclesM;
	return result;
}

short Square(unsigned int time, float freq, float ampl)
{
	short result = 0;
	int period = int(SAMPLE_RATE / freq);
	int cyclesM = time % period;

	if (cyclesM < period / 2)
	{
		result = ampl * 32767;
	}
	else
	{
		result = ampl * -32767;
	}

	return result;
}

short Noise(unsigned int time, float ampl)
{
	return randRange(0, 32767) * ampl;
}

static void listAudioDevices(const ALCchar* list)
{
	if (!list || *list == '\0')
		printf("none\n");
	else do {
		printf("    %s\n", list);
		list += strlen(list) + 1;
	} while (*list != '\0');
}



int main(int argc, char *argv[], char *envp[])
{
	setlocale(LC_ALL, "ru");

    ALboolean enumeration;
    const ALCchar* devices;
    const ALCchar* defaultDeviceName = argv[1];

	ALsizei length = SEC * 1;
	std::vector<short> samples;
	//short* samples = new short[length];

	ALCdevice* device;
	ALCcontext* context;
	ALuint buffer, source;

	ALfloat listenerOrientation[] = {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f};
	ALint sourceState;

	enumeration = alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT");
	//enumeration error
	if (enumeration == AL_FALSE)
	{
		fprintf(stderr, "enumeration extension not available\n");
	}

	if (!defaultDeviceName)
	{
		defaultDeviceName = alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);
	}

	printf("Available playback devices:\n");
	if (alcIsExtensionPresent(NULL, "ALC_ENUMERATE_ALL_EXT") != AL_FALSE)
		listAudioDevices(alcGetString(NULL, ALC_ALL_DEVICES_SPECIFIER));
	else
		listAudioDevices(alcGetString(NULL, ALC_DEVICE_SPECIFIER));
	printf("Available capture devices:\n");
	listAudioDevices(alcGetString(NULL, ALC_CAPTURE_DEVICE_SPECIFIER));

	device = alcOpenDevice(defaultDeviceName);
	//device error
	if (!device) 
	{
		fprintf(stderr, "unable to open default device\n");
		return -1;
	}

	alGetError();

	context = alcCreateContext(device, NULL);
	//context error
	if (!alcMakeContextCurrent(context)) 
	{
		fprintf(stderr, "failed to make default context\n");
		return -1;
	}
	AL_ERROR("make default context");

	//setup listener
	alListener3f(AL_POSITION, 0, 0, 1.0f);
	AL_ERROR("listener position");
	alListener3f(AL_VELOCITY, 0, 0, 0);
	AL_ERROR("listener velocity");
	alListenerfv(AL_ORIENTATION, listenerOrientation);
	AL_ERROR("listener orientation");

	alGenSources((ALuint)1, &source);
	AL_ERROR("source generation");

	//setup source
	alSourcef(source, AL_PITCH, 1);
	AL_ERROR("source pitch");
	alSourcef(source, AL_GAIN, 1);
	AL_ERROR("source gain");
	alSource3f(source, AL_POSITION, 0, 0, 0);
	AL_ERROR("source position");
	alSource3f(source, AL_VELOCITY, 0, 0, 0);
	AL_ERROR("source velocity");
	alSourcei(source, AL_LOOPING, AL_FALSE);
	AL_ERROR("source looping");

	alGenBuffers(1, &buffer);
	AL_ERROR("buffer generation");

	cout << "generating..." << endl;
	auto begin = std::chrono::steady_clock::now();
	for (auto i = 0; i < length; ++i) 
	{
		samples.push_back(Noise(i, 0.9));
		//samples[i] = SineWave(i, 440, 0.9);
	}
	auto end = std::chrono::steady_clock::now();
	long long time = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
	cout << "generated in " << time << " milliseconds" << endl;

	alBufferData(buffer, AL_FORMAT_MONO16, samples.data(), samples.size(), SAMPLE_RATE);
	//alBufferData(buffer, AL_FORMAT_MONO16, samples, length, sampleRate);
	AL_ERROR("failed to load buffer data");

	alSourcei(source, AL_BUFFER, buffer);
	AL_ERROR("buffer binding");

	begin = std::chrono::steady_clock::now();
	alSourcePlay(source);
	AL_ERROR("source playing");

	alGetSourcei(source, AL_SOURCE_STATE, &sourceState);
	AL_ERROR("source state get");
	while (sourceState == AL_PLAYING) 
	{
		alGetSourcei(source, AL_SOURCE_STATE, &sourceState);
		AL_ERROR("source state get");
	}
	end = std::chrono::steady_clock::now();
	time = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
	cout << "played for " << time << " milliseconds" << endl;

	//exit
	alDeleteSources(1, &source);
	alDeleteBuffers(1, &buffer);
	device = alcGetContextsDevice(context);
	alcMakeContextCurrent(NULL);
	alcDestroyContext(context);
	alcCloseDevice(device);

	return 0;
}