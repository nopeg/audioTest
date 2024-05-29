#include "Emitter.h"

audio::Emitter::Emitter()
{
	setSource();
}
audio::Emitter::~Emitter()
{
	alDeleteSources(1, &source);
	alDeleteBuffers(1, &buffer);
}

void audio::Emitter::setSource()
{
	//setup source
	alGenSources(1, &source);
	AL_ERROR("source generation");
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
}

void audio::Emitter::addVoice()
{

}

void audio::Emitter::generateSamples()
{
	cout << "generating..." << endl;
	samples.resize(4);
	auto begin = std::chrono::steady_clock::now();
	for (auto i = 0; i < length; ++i)
	{
		samples[0] = osc::sine(i, 440, 1);
		samples[1] = osc::triangle(i, 220, 1);
		samples[2] = osc::sawtooth(i, 880, 1);
		samples[3] = osc::square(i, 110, 1);
		result.push_back(osc::mixed(samples));
	}
	auto end = std::chrono::steady_clock::now();
	long long time = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
	cout << "generated in " << time << " milliseconds" << endl;
}

void audio::Emitter::play()
{
	alSourcePlay(source);
	AL_ERROR("source playing");

	alGetSourcei(source, AL_SOURCE_STATE, &sourceState);
	AL_ERROR("source state get");
	while (sourceState == AL_PLAYING)
	{
		alGetSourcei(source, AL_SOURCE_STATE, &sourceState);
		AL_ERROR("source state get");
	}
}

void audio::Emitter::setBuffer()
{
	alGenBuffers(1, &buffer);
	AL_ERROR("buffer generation");

	generateSamples();

	alBufferData(buffer, AL_FORMAT_MONO16, result.data(), result.size(), SAMPLE_RATE);
	AL_ERROR("failed to load buffer data");

	alSourcei(source, AL_BUFFER, buffer);
	AL_ERROR("buffer binding");
}