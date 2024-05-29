#ifndef OSCILLATOR_H
#define OSCILLATOR_H

#include "Common.h"

namespace osc
{
	//wave generator
	short sine(unsigned int time, const float freq, const float ampl);

	short triangle(unsigned int time, float freq, float ampl);

	short sawtooth(unsigned int time, float freq, float ampl);

	short square(unsigned int time, float freq, float ampl);

	short noise(unsigned int time, float ampl);

	//mixes samples into one
	short mixed(const std::vector<short>& samples);
};

#endif