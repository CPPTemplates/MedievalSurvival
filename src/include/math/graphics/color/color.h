#pragma once
#include "math/vector/vectn.h"
#include "globalFunctions.h"
#include <type_traits>
#include <limits>

// bgra structure
template <typename T, fsize_t channelCount>
struct colortn : public vectn<T, channelCount>
{
	typedef vectn<T, channelCount> base;
	typedef T channelType;
	typedef const colortn<T, channelCount> ccolort;

	addMemberName(b, this->axis[0], channelCount > 0);
	addMemberName(g, this->axis[1], channelCount > 1);
	addMemberName(r, this->axis[2], channelCount > 2);
	addMemberName(a, this->axis[3], channelCount > 3);

	// memory alignment for constructors:
	//(r, g, b) -> b, g, r
	//(h, s, v) -> v, s, h
	addMemberName(v, this->axis[0], channelCount > 0);
	addMemberName(s, this->axis[1], channelCount > 1);
	addMemberName(h, this->axis[2], channelCount > 2);

	static constexpr size_t byteSize = sizeof(T) * channelCount;

	static constexpr size_t bitSize = byteSize * ByteToBits;

	static constexpr bool isByteColor = std::is_same_v<std::remove_const_t<T>, byte>;
	static constexpr T maxValue = std::is_integral<T>::value ? std::numeric_limits<T>::max() : 1;

	static constexpr T halfMaxValue = maxValue / 2;
	static constexpr T quarterMaxValue = maxValue / 4;

	constexpr colortn(const colortn& other) = default;
	constexpr colortn& operator=(const colortn& other) = default;

	constexpr colortn(std::array<T, channelCount> initializerValues) : base(initializerValues) {}

	constexpr colortn(ccolort& c, const T& a) : vectn<T, channelCount>(c.axis[0], c.axis[1], c.axis[2], a)
	{
	}
	constexpr colortn(const T& a, const T& r, const T& g, const T& b) requires(channelCount >= 4)
	{
		this->a() = a;
		this->r() = r;
		this->g() = g;
		this->b() = b;
	}
	constexpr colortn(const T& r, const T& g, const T& b) : vectn<T, channelCount>(b, g, r) {
		if constexpr (channelCount > 3) {
			a() = maxValue;
		}
	}
	constexpr colortn(const T& a, const T& grayscale) : colortn(a, grayscale, grayscale, grayscale) {}
	constexpr colortn(const T& grayscale) : colortn(grayscale, grayscale, grayscale) {}
	constexpr colortn() // dont initialize a to maxvalue, let it be transparent by default
	{
	}

	explicit constexpr colortn(const vectn<T, channelCount>& in) : vectn<T, channelCount>(in)
	{
	}
	// for converting bgr to bgra
	// template <typename = std::enable_if_t<channelCount == 4>>
	// explicit constexpr colortn(const colortn<T, 3 + 4 - channelCount>& in);
	// for converting bgra to bgr
	// template <typename = std::enable_if_t<channelCount == 3>>
	// when channelcount = 3, then the other channelcount = 4
	// do not add explicit, then you'd have to use it like a constructor
	explicit constexpr colortn(const colortn<T, 7 - channelCount>& in) : vectn<T, channelCount>(in)
	{
		if constexpr (channelCount == 4)
		{
			a() = maxValue;
		}
	}

	// https://stackoverflow.com/questions/41011900/equivalent-ternary-operator-for-constexpr-if
	template <typename t2>
	constexpr colortn(const colortn<t2, channelCount>& other)
	{
		for (fsize_t i = 0; i < channelCount; i++)
		{
			constexpr t2 otherMaxValue = colortn<t2, channelCount>::maxValue;
			if constexpr (maxValue == otherMaxValue)
			{
				this->axis[i] = other.axis[i];
			}
			else
			{
				// multiply to guarantee correct result
				if constexpr (std::is_floating_point<T>::value)
				{
					constexpr T multiplier = (maxValue / (T)otherMaxValue);
					this->axis[i] = (T)other.axis[i] * multiplier;
				}
				else if constexpr (std::is_floating_point<t2>::value) // T is not a floating point type, t2 is
				{
					constexpr t2 multiplier = ((t2)maxValue / otherMaxValue);
					this->axis[i] = (T)(other.axis[i] * multiplier);
				}
				else // both not a floating point, we can'T make a constexpr multiplier
				{
					this->axis[i] = (T)((other.axis[i] * maxValue) / otherMaxValue);
				}
			}
		}
	}
	/*a((maxValue == colort<t2>::maxValue) ? in.a() : (in.a() * maxValue) / colort<t2>::maxValue),
	r((maxValue == colort<t2>::maxValue) ? in.r() : (in.r() * maxValue) / colort<t2>::maxValue),
	g((maxValue == colort<t2>::maxValue) ? in.g() : (in.g() * maxValue) / colort<t2>::maxValue),
	b((maxValue == colort<t2>::maxValue) ? in.b() : (in.b() * maxValue) / colort<t2>::maxValue)*/
	//{}

	template <typename t2>
	constexpr static T divideByMaxValue(const t2& toDivide)
	{
		if constexpr (maxValue == 1)
		{
			return (T)toDivide;
		}
		else
		{
			if constexpr (std::is_integral_v<t2>)
			{
				return (T)(toDivide / maxValue);
			}
			else
			{
				constexpr fp inverseMaxValue = 1.0 / maxValue;
				return (T)(toDivide * inverseMaxValue);
			}
		}
	}

	template <typename t2>
	inline static constexpr t2 multiplyByMaxValue(const t2& toMultiply)
	{
		if constexpr (maxValue != 1)
		{
			return toMultiply * maxValue;
		}
		else
		{
			return toMultiply;
		}
	}

	template <typename t2, typename t3>
	inline static constexpr T multiplyColorChannels(const t2& channel0, const t3& channel1)
	{
		return divideByMaxValue(channel0 * channel1);
	}

	template<int channelCount = 3>
	inline static constexpr colortn multiplyColors(ccolort& color0, ccolort& color1)
	{
		return ccolort(
			multiplyColorChannels(color0.r(), color1.r()),
			multiplyColorChannels(color0.g(), color1.g()),
			multiplyColorChannels(color0.b(), color1.b()));
	}
	template<>
	inline static constexpr colortn multiplyColors<4>(ccolort& color0, ccolort& color1)
	{
		return ccolort(
			multiplyColorChannels(color0.a(), color1.a()),
			multiplyColorChannels(color0.r(), color1.r()),
			multiplyColorChannels(color0.g(), color1.g()),
			multiplyColorChannels(color0.b(), color1.b())
		);
	}

	// get the highest value for each channel
	inline static constexpr colortn maximizeColors(ccolort& color1, ccolort& color2)
	{
		// will be converted to << 8
		return ccolort(math::maximum(color1.r(), color2.r()),
			math::maximum(color1.g(), color2.g()),
			math::maximum(color1.b(), color2.b()));
	}
};
typedef byte colorChannel;

constexpr fsize_t bgraColorChannelCount = 4;
constexpr fsize_t rgbColorChannelCount = 3;

//defaulting to BGRA, because of:
//size: 4 is 100 in binary, so the compiler can optimize more
//alpha support
constexpr fsize_t defaultColorChannelCount = bgraColorChannelCount;

typedef colortn<colorChannel, defaultColorChannelCount> color;
typedef const color ccolor;
typedef colortn<colorChannel, rgbColorChannelCount> colorRGB;

typedef colortn<byte, bgraColorChannelCount> colorb;
typedef const colorb ccolorb;

typedef colortn<fp, bgraColorChannelCount> colorf;
typedef const colorf ccolorf;
