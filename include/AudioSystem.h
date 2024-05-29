#ifndef AUDIO_H
#define AUDIO_H

#include <AL/al.h>
#include <AL/alc.h>
#include "Common.h"

namespace al
{
	class AudioSystem
	{
	private:
		ALboolean enumeration;
		const ALCchar* devices;
		const ALCchar* defaultDeviceName = nullptr;

		ALCdevice* device;
		ALCcontext* context;

	public:
		void AL_ERROR(const char* msg);
		void listDevices(const ALCchar* list);

		void setDevice();
		void setContext();
		void setListener();

		void exit();
	};
}
#endif