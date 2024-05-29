#include "Oscillator.h"

//simple waves
short osc::sine(unsigned int time, const float freq, const float ampl)
{
	float period = SAMPLE_RATE / freq;
	float cycles = time / period;
	float radian = 2 * PI * cycles;
	short result = 32767 * ampl * sin(radian);
	return result;
}

short osc::triangle(unsigned int time, float freq, float ampl)
{
	float period = SAMPLE_RATE / freq;
	float cycles = time / period;
	float value = 4.0f * fabs(cycles - floor(cycles + 0.5f)) - 1.0f;
	short result = 32767 * ampl * value;
	return result;
}

short osc::sawtooth(unsigned int time, float freq, float ampl)
{
	int period = SAMPLE_RATE / freq;
	int cyclesM = time % period;
	short result = 1.5f * freq * ampl * cyclesM;
	return result;
}

short osc::square(unsigned int time, float freq, float ampl)
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

short osc::noise(unsigned int time, float ampl)
{
	return randRange(0, 32767) * ampl;
}

//mixes samples into one
short osc::mixed(const std::vector<short>& samples)
{
	double sample1 = double(samples[0]);
	if (samples.size() > 1)
	{
		for (auto i = samples.size() - 1; i > 0; --i)
		{
			if (samples[i] != 0)
			{
				double sample2 = double(samples[i]);
				sample1 = (sample1 + sample2 / samples.size());
			}
		}
		sample1 /= samples.size();
	}
	return short(sample1);
}