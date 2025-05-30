#pragma once
#include "globalFunctions.h"
#include <climits>
#include <cmath>
#include "optimization/debug.h"
#include <bit>
// #include <stdint.h>

namespace math
{
	constexpr fp PI = 3.14159265359;
	constexpr fp PI2 = PI + PI; // a whole circle
	constexpr fp goldenRatio = 1.618033988749894848204586834;
	constexpr fp degreesToRadians = PI / 180;
	constexpr fp radiansToDegrees = 180 / PI;
	constexpr fp fpepsilon = 0.0001;
	constexpr fp averageSinusHillValue = 2.0 / PI;

	template <typename T>
	constexpr T sqrt(const T& x, const T& curr, const T& prev)
	{
		return curr == prev ? curr : sqrt(x, (T)0.5 * (curr + x / curr), curr);
	}

	template <typename T>
	constexpr T sqrt(const T& x)
	{
		return x >= (T)0 && x < std::numeric_limits<T>::infinity()
			? sqrt(x, x, (T)0)
			: std::numeric_limits<T>::quiet_NaN();
	}
	constexpr fp sqrt2 = sqrt(2.0);

	template <typename T = fsize_t>
	constexpr T getNextPowerOf2Multiplied(const T& n)
	{
		T i = 1;
		for (; i < n; i *= 2)
			;
		return i;
	}
	template <typename T = fsize_t>
	constexpr T getNextPowerOf2(const T& n)
	{
		T power = 1;
		for (T i = 1; i < n; i *= 2)
		{
			power++;
		}
		return power;
	}

	// degrees can be 0, 90, 180 or 270
	constexpr int sinDegrees(cint& degrees)
	{
		switch (degrees)
		{
		case 0:
			return 0;
		case 90:
			return 1;
		case 180:
			return 0;
		case 270:
			return -1;
		default:
			if constexpr (isDebugging)
			{
				throw std::wstring(L"only 0, 90, 180 or 270");
			}
			else
			{
				return 0;
			}
		}
	}
	// degrees can be 0, 90, 180 or 270
	inline int cosDegrees(cint& degrees)
	{
		cint& sinusEquivalentDegrees = degrees + 90;
		return sinDegrees(sinusEquivalentDegrees == 360 ? 0 : sinusEquivalentDegrees);
	}
	template <typename testFunction>
	inline bool isSummitBetween(cfp& min, cfp& max, testFunction func)
	{
		cfp& valueMin = func(min);
		return func(min - fpepsilon) < valueMin && valueMin > func(max);
	}
	inline fp Log(cfp value, cfp base)
	{
		return std::log(value) / std::log(base);
	}
	template <typename T>
	constexpr decltype(std::declval<T>()* std::declval<T>()) squared(const T& value)
	{
		return value * value;
	}

	template <typename T>
	constexpr T absolute(const T& value)
	{
		return value < 0 ? -value : value;
	}

	// https://stackoverflow.com/questions/824118/why-is-floor-so-slow
	template <typename outputType = int, typename InputType>
	constexpr outputType floor(const InputType& x)
	{
		if (std::is_integral_v<outputType>)
		{
			outputType i = (outputType)x; /* truncate */
			return i - (i > x);			  /* convert trunc to floor */
		}
		else
		{
			int i = (int)x;					  /* truncate */
			return (outputType)(i - (i > x)); /* convert trunc to floor */
		}
	}

	template <typename outputType = int, typename InputType>
	constexpr outputType ceil(const InputType& x)
	{
		if (std::is_integral_v<outputType>)
		{
			const outputType& i = (outputType)x;	   /* truncate */
			return i + (outputType)((InputType)i < x); /* convert trunc to floor */
		}
		else
		{
			int i = (int)x;									  /* truncate */
			return (outputType)(i + (int)((InputType)i < x)); /* convert trunc to floor */
		}
	}

	// floors a value to a unit (floor(5,2) will return 4)
	template <typename T>
	constexpr T floor(const T value, const T unit)
	{
		cfp divided = (fp)value / (fp)unit;			// divide by the value
		cfp flooredValue = std::floor(divided); // round towards zero
		return (T)(flooredValue * (fp)unit);		// multiply back, so we basically rounded it down in units of [unit]
	}
	// floors a value to a unit and substracts the input from the result (mod(5,2) will return 1)
	template <typename T>
	constexpr T mod(const T value, const T unit)
	{
		return value - floor(value, unit); // return difference
	}

	// returns a when w = 0
	// returns b when w = 1
	template <typename T>
	constexpr T lerp(const T& a, const T& b, cfp& w)
	{
		return (T)(a + (b - a) * w);
	}

	// 1 0 1 2 4

	template <typename T>
	constexpr T mapValue(const T& in, const T& imin, const T& imax, const T& omin, const T& omax)
	{
		const T& mult = (omax - omin) / (imax - imin);
		const T& plus = omin - (imin * mult);
		return in * mult + plus;
	}

	// creates an exponential curve, steeper at the min side.

	// steepness: 0 =
	template <typename T>
	constexpr T mapValueExponentiallyMinCurve(const T& in, const T& imin, const T& imax, const T& omin, const T& omax, const T& steepness)
	{
		// imin: 1
		// imax: 0
		const T& inRange0To1Inverted = (imax - in) / (imax - imin);

		const T& maxPow = 1;
		const T& minPow = pow(0.5, steepness);

		const T& outputInverted = pow(0.5, inRange0To1Inverted * steepness);

		const T& outputInRange0To1Inverted = (outputInverted - minPow) / (maxPow - minPow);

		// invert
		return omin + (omax - omin) * outputInRange0To1Inverted;
	}

	// creates an exponential curve, steeper at the max side.
	// swapped order
	template <typename T>
	constexpr T mapValueExponentiallyMaxCurve(const T& in, const T& imin, const T& imax, const T& omin, const T& omax, const T& steepness)
	{
		return mapValueExponentiallyMinCurve(in, imax, imin, omax, omin, steepness);
	}

	// to limit a value between bounds
	template <typename T>
	constexpr T clamp(const T& value, const T& min, const T& max)
	{
		return value < min ? min : value > max ? max
			: value;
	}
	template <typename T>
	constexpr int GetSign(const T& value)
	{
		return value > 0 ? 1 : value < 0 ? -1
			: 0;
	}

	template <typename T>
	constexpr T powSizeTSimple(const T& value, cfsize_t& power)
	{
		if (power)
		{
			T result = value;
			fsize_t raisedPower = 1;

			while (raisedPower < power)
			{
				fsize_t doubleRaisedPower = raisedPower + raisedPower;

				cbool& fastMethod = doubleRaisedPower <= power;

				result *= fastMethod ? result : value;
				raisedPower = fastMethod ? doubleRaisedPower : raisedPower + 1;
			}
			return result;
		}
		else
		{
			return (T)1;
		}
	}

	// https://stackoverflow.com/questions/2622441/c-integer-floor-function
	inline int FloorDiv(int a, int b)
	{
		if (a < 0 || b < 0)
		{
			ldiv_t res = ldiv(a, b);
			return (res.rem) ? res.quot - 1
				: res.quot;
		}
		else
		{
			return a / b;
		}
	}

	// https://en.wikipedia.org/wiki/Fast_inverse_square_root
	// DONT CHANGE PRECISION!
	// returns 1 / sqrt(number)
	inline constexpr20 float fastInverseSqrt(float number) noexcept
	{
		constexpr float threehalfs = 1.5F;

		const float& x2 = number * 0.5F;
#if __cplusplus >= 202002L
		std::uint32_t i = std::bit_cast<std::uint32_t>(number);
		i = 0x5f3759df - (i >> 1);
		number = std::bit_cast<float>(i);
#else
		// can be dangerous!
		std::uint32_t* i = reinterpret_cast<std::uint32_t*>(&number);
		*i = 0x5f3759df - (*i >> 1);
		number = *reinterpret_cast<float*>(&i);
#endif
		number *= threehalfs - (x2 * math::squared(number));
		return number;
	}

	// https://www.gamedev.net/forums/topic/704525-3-quick-ways-to-calculate-the-square-root-in-c/
	// returns 1 / sqrt(number) * number
	inline constexpr20 float fastSqrt(const float& number) noexcept
	{
		return number * fastInverseSqrt(number);
	}

	// a + (b - a) * w = c
	//(b - a) * w = c - a
	// w = (c - a)/(b - a)
	template <typename T>
	inline fp getw(const T a, const T b, const T c)
	{
		return (c - a) / (b - a);
	}
	template <typename T>
	constexpr T maximum(const T& left, const T& right)
	{
		return left > right ? left : right;
	}
	template <typename T>
	constexpr T minimum(const T& left, const T& right)
	{
		return left < right ? left : right;
	}

	// same slope = 0
	inline fp calculateAngle(cfp& slope0, cfp& slope1)
	{
		return math::absolute(atan((slope1 - slope0) / ((fp)1 + slope1 * slope0)));
	}
	inline fp Remainder1(cfp& value)
	{
		return value - floor(value);
	}
	template <typename T>
	inline int sign(const T& value)
	{
		return value > 0 ? 1 : value < 0 ? -1
			: 0;
	}
	// simulates 1 + 2 + 3 + 4 + 5...
	inline fp calculateIterativeAddition(cfp& iterationCount)
	{
		return iterationCount * (iterationCount + 1) * 0.5;
	}

	inline int floorToBase(cint& val, cint& base)
	{
		int floored = 1;
		while (floored * base < val)
		{
			floored *= base;
		}
		return floored;
	}
}
