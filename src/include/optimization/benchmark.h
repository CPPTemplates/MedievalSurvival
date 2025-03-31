#include "globalFunctions.h"
#include "math/timemath.h"
#pragma once
template<typename T>
struct benchMarkPoint
{
	benchMarkPoint(const T& processingType):processingType(processingType),measureTime(getmicroseconds()){}
	T processingType;
	microseconds measureTime;
};

template<typename T>
struct benchmarkContainer
{
	std::vector<benchMarkPoint<T>> benchMarks;
	inline microseconds measureBenchmarkTime(const T& processingType) const
	{
		microseconds totalMicroSeconds = 0;
		for (size_t i = 0; i < benchMarks.size(); i++)
		{
			const benchMarkPoint<T> point = benchMarks[i];
			if (point.processingType == processingType && i + 1 < benchMarks.size())
			{
				totalMicroSeconds += benchMarks[i + 1].measureTime - point.measureTime;
			}
		}
		return totalMicroSeconds;
	}
	inline microseconds measureTotalBenchmarkTime() const
	{
		return benchMarks.size() > 1 ? benchMarks[benchMarks.size() - 1].measureTime - benchMarks[0].measureTime : 0;
	}
	inline void removeOldBenchmarks()
	{
		benchMarks = std::vector<benchMarkPoint<T>>({ benchMarks[benchMarks.size() - 1] });
	}
	inline void addBenchmarkPoint(const T& processingType) {
		benchMarks.push_back(benchMarkPoint<T>(processingType));
	}
};