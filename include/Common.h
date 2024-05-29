#ifndef COMMON_H
#define COMMON_H

#include <iostream>
#include <vector>
#include <random>

#define PI 3.1415926f 
#define SAMPLE_RATE 44100
#define SEC SAMPLE_RATE * 2

using std::cout;
using std::endl;

inline int randRange(int min, int max)
{
	std::random_device randDevice;
	std::mt19937_64 generator(randDevice());
	std::uniform_int_distribution<int> dist(min, max);
	return dist(randDevice);
}
inline float randRange(float min, float max)
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

#endif