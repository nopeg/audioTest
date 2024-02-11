#include <iostream>

#include <AL/al.h>
#include <AL/alc.h>

#include "testHeader.h"

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

	enumeration = alcIsExtensionPresent(NULL, "ALC_ENUMERATION_EXT");
	if (enumeration == AL_FALSE)
	{
        std::cout << "no enumeration\n";
	}
	else
	{
        list_audio_devices(alcGetString(NULL, ALC_DEVICE_SPECIFIER));
	}
	func();
	return 0;
}