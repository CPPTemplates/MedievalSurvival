#pragma once
#include "globalFunctions.h"
#include "interface/idestructable.h"
#include "mathFunctions.h"
#include <vector>

template<typename T>
struct keyFrame
{
	constexpr keyFrame(cfp& location, const T& value)
	{
		this->location = location;
		this->value = value;
	}
	fp location;
	T value;
};
template<typename T>
struct transition :IDestructable
{
	//has to be sorted from least to most
	std::vector<keyFrame<T>> keyframes;

	constexpr transition(const std::vector<keyFrame<T>>& keyframes)
	{
		this->keyframes = keyframes;
	}

	constexpr T getValue(cfp& location) const
	{
		cfsize_t& KeyframeCount = (fsize_t)keyframes.size();
		if (keyframes[0].location >= location)
		{
			return keyframes[0].value;
		}
		//interpolate from i - 1 to i
		for (size_t i = 1; i < keyframes.size(); i++)
		{
			const keyFrame<T>& currentkeyframe = keyframes[i];
			if (currentkeyframe.location >= location)
			{
				const keyFrame<T>& lastkeyframe = keyframes[i - 1];
				//interpolate
				cfp& weight = math::getw(lastkeyframe.location, currentkeyframe.location, location);
				return math::lerp(lastkeyframe.value, currentkeyframe.value, weight);
			}
		}
		return keyframes[KeyframeCount - 1].value;
	}
};
