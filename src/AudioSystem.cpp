#include "AudioSystem.h"

//error handling
void al::AudioSystem::AL_ERROR(const char* msg)
{
	ALenum error = alGetError();
	if (error != AL_NO_ERROR)
	{
		cout << msg << endl;
		exit();
	}
}

void al::AudioSystem::listDevices(const ALCchar* list)
{
	if (!list || *list == '\0')
		printf("none\n");
	else do 
	{
		printf("    %s\n", list);
		list += strlen(list) + 1;
	} while (*list != '\0');
}

void al::AudioSystem::setDevice()
{
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
		listDevices(alcGetString(NULL, ALC_ALL_DEVICES_SPECIFIER));
	else
		listDevices(alcGetString(NULL, ALC_DEVICE_SPECIFIER));

	printf("Available capture devices:\n");
	listDevices(alcGetString(NULL, ALC_CAPTURE_DEVICE_SPECIFIER));

	device = alcOpenDevice(defaultDeviceName);
	//device error
	if (!device)
	{
		fprintf(stderr, "unable to open default device\n");
		exit();
	}
}

void al::AudioSystem::setContext()
{
	context = alcCreateContext(device, NULL);
	//context error
	if (!alcMakeContextCurrent(context))
	{
		fprintf(stderr, "failed to make default context\n");
		exit();
	}
	AL_ERROR("make default context");
}

void al::AudioSystem::setListener()
{
	//setup listener
	ALfloat listenerOrientation[] = {0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f};
	alListener3f(AL_POSITION, 0, 0, 1.0f);
	AL_ERROR("listener position");
	alListener3f(AL_VELOCITY, 0, 0, 0);
	AL_ERROR("listener velocity");
	alListenerfv(AL_ORIENTATION, listenerOrientation);
	AL_ERROR("listener orientation");
}

void al::AudioSystem::exit()
{
	//exit
	device = alcGetContextsDevice(context);
	alcMakeContextCurrent(NULL);
	alcDestroyContext(context);
	alcCloseDevice(device);
	return;
}