// #include <cstdlib>
#include <random>
#include <stdexcept>
#include "globalFunctions.h"
#include "math/mathFunctions.h"
#include "math/uuid.h"
#pragma once

/// @brief 
/// @return a generator based on the current nanosecond
std::mt19937 getGeneratorBasedOnTime();

template <typename T = int, typename = std::enable_if_t<std::is_integral_v<T>>>
inline T rand(std::mt19937 &generator)
{
	std::uniform_int_distribution<T> distribution(0);
	return distribution(generator);
}
// CAN ALSO RETURN MAX
template <typename T = int, typename = std::enable_if_t<std::is_integral_v<T>>>
inline T rand(std::mt19937 &generator, const T &max)
{
	std::uniform_int_distribution<T> distribution(0, max);
	return distribution(generator);
}
// CAN ALSO RETURN MAX
template <typename T = int, typename = std::enable_if_t<std::is_integral_v<T>>>
inline T rand(std::mt19937 &generator, const T &min, const T &max)
{
	std::uniform_int_distribution<T> distribution(min, max);
	return distribution(generator);
}
// DOES RETURN MAX
// averageRollCount: the average amount of rolls to get a number out of this sequence
inline bool randChance(std::mt19937 &generator, cint &averageRollCount)
{
	std::uniform_int_distribution<int> distribution(0, averageRollCount - 1);
	return distribution(generator) == 0;
}
template <typename T, typename = std::enable_if<std::is_integral<T>::value>>
inline T randIndex(std::mt19937 &generator, const T &arraySize)
{
	std::uniform_int_distribution<T> distribution(0, arraySize - 1);
	return distribution(generator);
}
inline fsize_t randIndex(std::mt19937 &generator, const std::vector<fp> weights, cfp &totalWeight)
{
	std::uniform_real_distribution<fp> distribution(0, totalWeight);
	fp value = distribution(generator);
	for (fsize_t i = 0; i < weights.size(); i++)
	{
		value -= weights[i];
		if (value <= 0) //<= because the distribution can also return the totalweight
		{
			return i;
		}
	}
	throw std::runtime_error("total weight too heigh");
}
inline int randIndex(std::mt19937 &generator, const std::vector<int> weights, cint &totalWeight)
{
	std::uniform_int_distribution<int> distribution(0, totalWeight - 1);
	int value = distribution(generator);
	for (int i = 0; i < (int)weights.size(); i++)
	{
		value -= weights[i];
		if (value < 0)
		{
			return i;
		}
	}
	throw std::runtime_error("total weight too heigh");
}

inline std::mt19937 getRandomFromSeed(const ull &seed)
{
	// Initialize the seed sequence with the full 64-bit seed
	std::seed_seq seedSeq{
		static_cast<uint>(seed),
		static_cast<uint>(seed >> 0x20)};
	// Initialize the Mersenne Twister random number generator with the seed sequence
	std::mt19937 mt(seedSeq);
	return mt;
}

inline int rand(cint &maxPlus1)
{
	return rand() % maxPlus1;
}
inline int rand(cint &min, cint &maxPlus1)
{
	return rand() % (maxPlus1 - min) + min;
}

template <typename T = fp, typename = std::enable_if_t<std::is_floating_point_v<T>>>
inline T randFp(std::mt19937 &generator)
{
	std::uniform_real_distribution<T> distribution(0, 1);
	return distribution(generator);
}
template <typename T = fp, typename = std::enable_if_t<std::is_floating_point_v<T>>>
inline T randFp(std::mt19937 &generator, const T &max)
{
	std::uniform_real_distribution<T> distribution(0, max);
	return distribution(generator);
}
template <typename T = fp, typename = std::enable_if_t<std::is_floating_point_v<T>>>
inline T randFp(std::mt19937 &generator, const T &min, const T &max)
{
	std::uniform_real_distribution<T> distribution(min, max);
	return distribution(generator);
}

template <typename T = fp, typename = std::enable_if_t<std::is_floating_point_v<T>>>
inline T randFp()
{
	return (T)rand() / RAND_MAX;
}
template <typename T = fp, typename = std::enable_if_t<std::is_floating_point_v<T>>>
inline T randFp(const T &max)
{
	return randFp() * max;
}
template <typename T = fp, typename = std::enable_if_t<std::is_floating_point_v<T>>>
inline T randFp(const T &min, const T &max)
{
	return math::lerp(min, max, randFp());
}

template <typename T = fp, typename = std::enable_if_t<std::is_floating_point_v<T>>>
inline int roundRandom(std::mt19937 &generator, const T &value)
{
	cint flooredValue = (int)floor(value);
	const T fractionalValue = value - flooredValue;
	return randFp(generator) < fractionalValue ? flooredValue + 1 : flooredValue;
}

// returns in range [0,1]
inline fp getRandomDistanceFromCenter(std::mt19937 &generator)
{
	return sqrt(randFp(generator));
}

inline uuid randomUUID(std::mt19937 &generator)
{
	uuid id = uuid();
	for (uint &n : id.idInts)
	{
		n = rand<uint>(generator);
	}
	return id;
}

std::wstring randomName(std::mt19937 &generator);