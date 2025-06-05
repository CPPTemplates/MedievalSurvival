#pragma once
#include "math/vector/vectn.h"
template <typename outputType = int, typename InputType, fsize_t axisCount>
constexpr vectn<outputType, axisCount> floorVector(const vectn<InputType, axisCount>& vec)
{
	vectn<outputType, axisCount> result = vectn<outputType, axisCount>();
	for (fsize_t i = 0; i < axisCount; i++)
	{
		result[i] = (outputType)math::floor<outputType, InputType>(vec[i]);
	}
	return result;
}

template <typename outputType = int, typename InputType, fsize_t axisCount>
constexpr vectn<outputType, axisCount> ceilVector(const vectn<InputType, axisCount>& vec)
{
	vectn<outputType, axisCount> result = vectn<outputType, axisCount>();
	for (fsize_t i = 0; i < axisCount; i++)
	{
		result[i] = (outputType)math::ceil<outputType, InputType>(vec[i]);
	}
	return result;
}

// https://www.omnicalculator.com/math/angle-between-two-vectors
// a and b have to be NORMALIZED
template <typename T>
inline fp angleBetween(const vect2<T>& a, const vect2<T>& b)
{
	return acos(vec2::dot(a, b));
}

// y is greater at the top
template <typename T>
constexpr bool woundClockwise(const vect2<T>& a, const vect2<T>& b, const vect2<T>& c)
{
	// counter-clockwise
	const vect2<T>& dab = b - a;
	const vect2<T>& dac = c - a;
	const T& winding = dab.x * dac.y - dab.y * dac.x;
	return winding < 0;
}

template<typename T, fsize_t N, typename indexType>
constexpr vectn<T, N> multiDimensionalIndex(indexType singleDimensionalIndex, const vectn<T, N>& size) {
	if constexpr (N == 1) {
		return vectn<T, N>((T)singleDimensionalIndex);
	}
	else {
		vectn<T, N> index = vectn<T, N>();
		for (fsize_t axisIndex = 0; axisIndex < N; axisIndex++) {
			index[axisIndex] = singleDimensionalIndex % size[axisIndex];
			singleDimensionalIndex /= size[axisIndex];
		}
		return index;
	}
}

template <typename indexT, typename sizeT, fsize_t n, typename returnIndexType = std::conditional_t < 1 < n && sizeof(indexT) < 4, fsize_t, indexT > >
constexpr returnIndexType singleDimensionalIndex(const vectn<indexT, n>& index, const vectn<sizeT, n>& size) requires(std::is_integral_v<indexT>)
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
		// can'T come here
		// assumeInRelease(false);
	}
}

template <typename T, fsize_t n>
constexpr bool inBounds(const vectn<T, n>& index, const vectn<T, n>& size)
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

template <typename T, fsize_t n>
constexpr vectn<T, n> reflect(const vectn<T, n>& vector, const vectn<T, n>& normal)
{
	return vector - (2 * vectn<T, n>::dot(vector, normal)) * normal;
}

//https://stackoverflow.com/questions/849211/shortest-distance-between-a-point-and-a-line-segment
constexpr fp distanceToLineSegment(cvec2& lineStart, cvec2& lineEnd, cvec2& p)
{
	// Return minimum distance between line segment vw and point p
	cfp& l2 = (lineStart - lineEnd).lengthSquared(); // i.e. |lineEnd-lineStart|^2 -  avoid a sqrt
	if (l2 == 0.0)
		return (p - lineStart).length(); // lineStart == lineEnd case
	// Consider the line extending the segment, parameterized as lineStart + T (lineEnd - lineStart).
	// We find projection of point p onto the line.
	// It falls where T = [(p-lineStart) . (lineEnd-lineStart)] / |lineEnd-lineStart|^2
	// We clamp T from [0,1] to handle points outside the segment vw.
	cfp& T = math::clamp(vec2::dot(p - lineStart, lineEnd - lineStart) / l2, (fp)0, (fp)1);
	const cvec2& projection = math::lerp(lineStart, lineEnd, T); // Projection falls on the segment
	return (p - projection).length();
}