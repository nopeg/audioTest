#ifndef EMITTER_H
#define EMITTER_H

#include "AudioSystem.h"
#include <chrono>
#include "Oscillator.h"

namespace audio
{
	class Emitter : public al::AudioSystem
	{
	private:
		ALuint buffer, source;
		ALsizei length = SEC * 5;
		std::vector<short> samples;
		std::vector<short> result;
		//short* samples = new short[length];
		ALint sourceState;

		void setSource();
	public:

		Emitter();
		~Emitter();

		void addVoice();
		void generateSamples();
		void play();
		void setBuffer();
	};
}

#endif