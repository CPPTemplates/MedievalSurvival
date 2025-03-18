#pragma once
#include "math/vector/vectn.h"
template <typename outputType = int, typename inputType, fsize_t axisCount>
constexpr vectn<outputType, axisCount> floorVector(const vectn<inputType, axisCount> &vec)
{
	vectn<outputType, axisCount> result = vectn<outputType, axisCount>();
	for (fsize_t i = 0; i < axisCount; i++)
	{
		result[i] = (outputType)math::floor<outputType, inputType>(vec[i]);
	}
	return result;
}

template <typename outputType = int, typename inputType, fsize_t axisCount>
constexpr vectn<outputType, axisCount> ceilVector(const vectn<inputType, axisCount> &vec)
{
	vectn<outputType, axisCount> result = vectn<outputType, axisCount>();
	for (fsize_t i = 0; i < axisCount; i++)
	{
		result[i] = (outputType)math::ceil<outputType, inputType>(vec[i]);
	}
	return result;
}

// https://www.omnicalculator.com/math/angle-between-two-vectors
// a and b have to be NORMALIZED
template <typename t>
inline fp angleBetween(const vect2<t> &a, const vect2<t> &b)
{
	return acos(vec2::dot(a, b));
}

// y is greater at the top
template <typename t>
constexpr bool woundClockwise(const vect2<t> &a, const vect2<t> &b, const vect2<t> &c)
{
	// counter-clockwise
	const vect2<t> &dab = b - a;
	const vect2<t> &dac = c - a;
	const t &winding = dab.x * dac.y - dab.y * dac.x;
	return winding < 0;
}

template <typename indexT, typename sizeT, fsize_t n, typename returnIndexType = std::conditional_t<1 < n && sizeof(indexT) < 4, fsize_t, indexT>>
constexpr returnIndexType singleDimensionalIndex(const vectn<indexT, n> &index, const vectn<sizeT, n> &size) requires(std::is_integral_v<indexT>)
{
	if constexpr (n == 1)
	{
		return index.axis[0];
	}
	else if constexpr (n == 2)
	{
		return index.axis[0] + index.axis[1] * size.axis[0];
	}
	else
	{
		returnIndexType sdIndex = index[n - 1];
		for (fsize_t i = n - 2;; i--)
		{
			sdIndex *= size.axis[i];
			sdIndex += index.axis[i];
			if (i == 0)
			{
				return sdIndex;
			}
		}
		// can't come here
		// assumeInRelease(false);
	}
}

template <typename t, fsize_t n>
constexpr bool inBounds(const vectn<t, n> &index, const vectn<t, n> &size)
{
	for (auto it : std::views::zip(index, size))
	{
		if (it.template val<0>() < 0 || it.template val<0>() >= it.template val<1>())
		{
			return false;
		}
	}
	return true;
}

template <typename t, fsize_t n>
constexpr vectn<t, n> reflect(const vectn<t, n> &vector, const vectn<t, n> &normal)
{
	return vector - (2 * vectn<t, n>::dot(vector, normal)) * normal;
}

//https://stackoverflow.com/questions/849211/shortest-distance-between-a-point-and-a-line-segment
constexpr fp distanceToLineSegment(cvec2 &lineStart, cvec2 &lineEnd, cvec2 &p)
{
	// Return minimum distance between line segment vw and point p
	cfp &l2 = (lineStart - lineEnd).lengthSquared(); // i.e. |lineEnd-lineStart|^2 -  avoid a sqrt
	if (l2 == 0.0)
		return (p - lineStart).length(); // lineStart == lineEnd case
	// Consider the line extending the segment, parameterized as lineStart + t (lineEnd - lineStart).
	// We find projection of point p onto the line.
	// It falls where t = [(p-lineStart) . (lineEnd-lineStart)] / |lineEnd-lineStart|^2
	// We clamp t from [0,1] to handle points outside the segment vw.
	cfp &t = math::clamp(vec2::dot(p - lineStart, lineEnd - lineStart) / l2, (fp)0, (fp)1);
	const cvec2 &projection = math::lerp(lineStart, lineEnd, t); // Projection falls on the segment
	return (p - projection).length();
}