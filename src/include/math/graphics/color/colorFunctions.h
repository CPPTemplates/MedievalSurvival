#pragma once
#include "color.h"
template <typename T, fsize_t channelCount, size_t colorCount, typename WeightType>
inline static constexpr colortn<T, channelCount> interpolateColorBilinear(const colortn<T, channelCount>(&colorsToInterpolate)[colorCount], const WeightType(&weight)[colorCount], const WeightType& maxWeight)
{
	colortn<WeightType, channelCount> result = colortn<T, channelCount>();
	const T* colorPtr = &colorsToInterpolate[0][0];
	for (auto colorIndex = 0; colorIndex < colorCount; colorIndex++)
	{

		//for (auto channelIndex = 0; channelIndex < rgbColorChannelCount; channelIndex++) {
		//	//fused multiply add: x * y + z
		//	result[channelIndex] = std::fma(colorsToInterpolate[colorIndex][channelIndex], weight[colorIndex], result[channelIndex]);
		//	//result[channelIndex] += colorsToInterpolate[colorIndex][channelIndex] * weight[colorIndex];
		//}
		for (auto channelIndex = 0; channelIndex < channelCount; channelIndex++) {
			result[channelIndex] += *colorPtr++ * weight[colorIndex];
		}
	}

	//when maxvalue != 1, maxweight will also not be 1
	if constexpr (colortn<T, channelCount>::maxValue != 1) {
		for (auto channelIndex = 0; channelIndex < rgbColorChannelCount; channelIndex++) {
			result[channelIndex] /= maxWeight;
		}
	}
	return colortn<T, channelCount>(std::array<T, channelCount>({ (T)result[0], (T)result[1], (T)result[2], colortn<T, channelCount>::maxValue }));
}

template <typename T, fsize_t channelCount, typename WeightType = decltype(std::declval<T>()* std::declval<T>())>
// https://en.wikipedia.org/wiki/Bilinear_interpolation
inline static constexpr colortn<T, channelCount> interpolateColorBilinear(const colortn<T, channelCount>(&colorsToInterpolate)[4], cvect2<WeightType>& weights)
{
	constexpr WeightType maxWeight = std::is_integral_v<T> ? (WeightType)std::numeric_limits<T>::max() + (WeightType)1 : (T)1;
	constexpr WeightType maxWeightSquared = maxWeight * maxWeight;
	cvect2<WeightType>& invertedWeigths = maxWeight - weights;
	const decltype(std::declval<T>() * std::declval<T>())(&weightArray)[4] {
		invertedWeigths.x* invertedWeigths.y,
			weights.x* invertedWeigths.y,
			invertedWeigths.x* weights.y,
			weights.x* weights.y,
	};


	return colortn<T, channelCount>(std::array<T, channelCount>({
		(T)((colorsToInterpolate[0][0] * weightArray[0] + colorsToInterpolate[1][0] * weightArray[1] + colorsToInterpolate[2][0] * weightArray[2] + colorsToInterpolate[3][0] * weightArray[3]) / maxWeightSquared),
		(T)((colorsToInterpolate[0][1] * weightArray[0] + colorsToInterpolate[1][1] * weightArray[1] + colorsToInterpolate[2][1] * weightArray[2] + colorsToInterpolate[3][1] * weightArray[3]) / maxWeightSquared),
		(T)((colorsToInterpolate[0][2] * weightArray[0] + colorsToInterpolate[1][2] * weightArray[1] + colorsToInterpolate[2][2] * weightArray[2] + colorsToInterpolate[3][2] * weightArray[3]) / maxWeightSquared),
		colortn<T, channelCount>::maxValue
		}));

	//return interpolateColorBilinear(colorsToInterpolate, weightArray, maxWeight * maxWeight);

	//return colortn<T, channelCount>({
	//   (colorsToInterpolate[0].b() * weightArray[0]) + (colorsToInterpolate[1].b() * weightArray[1]) + (colorsToInterpolate[2].b() * weightArray[2]) + (colorsToInterpolate[3].b() * weightArray[3]),
	//   (colorsToInterpolate[0].g() * weightArray[0]) + (colorsToInterpolate[1].g() * weightArray[1]) + (colorsToInterpolate[2].g() * weightArray[2]) + (colorsToInterpolate[3].g() * weightArray[3]),
	//   (colorsToInterpolate[0].r() * weightArray[0]) + (colorsToInterpolate[1].r() * weightArray[1]) + (colorsToInterpolate[2].r() * weightArray[2]) + (colorsToInterpolate[3].r() * weightArray[3]),
	//   1
	//	}
	//);
}

template <typename colorType = color>
inline static constexpr colorType hexToColor(cuint& hex)
{
	if constexpr (colorType::maxValue != bytemax)
	{
		constexpr fp multiplier = colorType::maxValue / bytemax;
		return colorType(
			(typename colorType::channelType)((hex / 0x10000) * multiplier),
			(typename colorType::channelType)(((hex / 0x100) & 0xff) * multiplier),
			(typename colorType::channelType)((hex & 0xff) * multiplier));
	}
	else
	{
		return colorType((typename colorType::channelType)(hex / 0x10000), (typename colorType::channelType)((hex / 0x100) & 0xff), (typename colorType::channelType)(hex & 0xff));
	}
}
template <typename colorType = color>
inline static colorType stringToColor(const std::wstring& str)
{
	if (str[0] == L'#')
	{
		long l;
		if (convertToLong(str.substr(1, 6), l, 16))
		{
			return hexToColor(l);
		}
	}
	throw "can'T parse the color";
}
// returns the average of 4 colors.
// inline static colortn<T, channelCount> Average(colortn<T,channelCount>& c1, colortn<T,channelCount>& c2, colortn<T,channelCount>& c3, colortn<T,channelCount>& c4)
//{
//	return colortn<T,channelCount>(
//		(c1.a() + c2.a() + c3.a() + c4.a()) / 4,
//		(c1.r() + c2.r() + c3.r() + c4.r()) / 4,
//		(c1.g() + c2.g() + c3.g() + c4.g()) / 4,
//		(c1.b() + c2.b() + c3.b() + c4.b()) / 4
//	);
// }

// returns the average of 4 colors.
template <typename T, fsize_t channelCount>
inline static colortn<T, channelCount> averageColor(const colortn<T, channelCount>& c1, const colortn<T, channelCount>& c2, const colortn<T, channelCount>& c3, const colortn<T, channelCount>& c4)
{
	const auto& totalA = c1.a() + c2.a() + c3.a() + c4.a();
	if (totalA == 0)
	{
		return colortn<T, channelCount>();
	}
	else
	{
		return colortn<T, channelCount>(
			(T)((c1.a() + c2.a() + c3.a() + c4.a()) / 4),
			(T)((c1.r() * c1.a() + c2.r() * c2.a() + c3.r() * c3.a() + c4.r() * c4.a()) / totalA),
			(T)((c1.g() * c1.a() + c2.g() * c2.a() + c3.g() * c3.a() + c4.g() * c4.a()) / totalA),
			(T)((c1.b() * c1.a() + c2.b() * c2.a() + c3.b() * c3.a() + c4.b() * c4.a()) / totalA));
	}
}

// returns c1 on 0 and c2 on 1 and lerps between
template <typename T, fsize_t channelCount>
inline static constexpr colortn<T, channelCount> lerpColor(const colortn<T, channelCount>& c1, const colortn<T, channelCount>& c2, cfp& weight)
{
	return colortn<T, channelCount>(
		c1.r() + (T)(weight * (fp)(c2.r() - c1.r())),
		c1.g() + (T)(weight * (fp)(c2.g() - c1.g())),
		c1.b() + (T)(weight * (fp)(c2.b() - c1.b())));
}
// returns the 'top view' of the colors, above eachother
template <typename T, fsize_t channelCount>
inline static constexpr colortn<T, channelCount> transitionColor(const colortn<T, channelCount>& topcolor, const colortn<T, channelCount>& bottomcolor)
{
	return colortn<T, channelCount>(
		colortn<T, channelCount>::maxValue - colortn<T, channelCount>::multiplyColorChannels(colortn<T, channelCount>::maxValue - bottomcolor.a(), colortn<T, channelCount>::maxValue - topcolor.a()),
		bottomcolor.r() + colortn<T, channelCount>::multiplyColorChannels(topcolor.a(), topcolor.r() - bottomcolor.r()),
		bottomcolor.g() + colortn<T, channelCount>::multiplyColorChannels(topcolor.a(), topcolor.g() - bottomcolor.g()),
		bottomcolor.b() + colortn<T, channelCount>::multiplyColorChannels(topcolor.a(), topcolor.b() - bottomcolor.b()));
}

constexpr uint getUint(const color& c)
{
	return static_cast<uint>(c.axis[0]) + static_cast<uint>(c.axis[1]) * 0x100 + static_cast<uint>(c.axis[2]) * 0x10000 + static_cast<uint>(c.axis[3]) * 0x1000000;
}

colorf rgb2hsv(const colorf& in);
colorf hsv2rgb(const colorf& in);