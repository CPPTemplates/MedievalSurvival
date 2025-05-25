#pragma once
#include "brush/brushes/solidColorBrush.h"
#include "brush/brushes/colorMultiplier.h"
#include "brush/brushes/colorMixer.h"
#include "brush/brushes/squareInterpolator.h"
#include "brush/brushes/alphaMask.h"

//overloads
template<typename BrushType>
constexpr auto makeRowIterator(const BrushType& brush, const typename BrushType::InputType& pos);
//template<typename brush0Type, typename brush1Type>
//constexpr auto makeRowIterator(const colorMultiplier<brush0Type, brush1Type>& brush, const typename combinebrush<brush0Type, brush1Type>::InputType& pos);
//template<typename brush0Type, typename brush1Type>
//constexpr auto makeRowIterator(const colorMixer<brush0Type, brush1Type>& brush, const typename combinebrush<brush0Type, brush1Type>::InputType& pos);
//template<typename brush0Type>
//constexpr auto makeRowIterator(const transformBrush<brush0Type>& brush, const typename transformBrush<brush0Type>::InputType& pos);

template<typename brushType>
using iteratorType = decltype(makeRowIterator(std::declval<brushType>(), std::declval<typename brushType::InputType>()));

//iterators
template<typename brushType>
struct baseIterator {
	const brushType& brush;
	typename brushType::InputType position;
	constexpr baseIterator(const brushType& brush, const typename brushType::InputType& position) : brush(brush), position(position) {}
};

//optimization for bilinear interpolation: interpolating on one axis is linear. this will give a huge performance boost
struct bilinearIterator {
	colorf currentColor;
	colorf toAdd;
	fp offset = 0;
	constexpr bilinearIterator(const squareInterpolator<true>& brush, cvec2& startPosition, cfp& xStep) {
		//calculate start and ending color
		cfp& invY = (1.0 - startPosition.y);
		const colorf& startColor =
			(colorf)brush.cornerColors[0] * invY +
			(colorf)brush.cornerColors[2] * startPosition.y;
		const colorf& endColor =
			(colorf)brush.cornerColors[1] * invY +
			(colorf)brush.cornerColors[3] * startPosition.y;
		toAdd = (endColor - startColor) * xStep;
		currentColor = startColor + toAdd * startPosition.x;
	}
	constexpr color operator*() const {
		return (color)(currentColor + (toAdd * offset));
	}
	constexpr auto& operator++() {
		offset++;
		return *this;
	}
};

template<typename brushType>
struct rowIterator : public baseIterator<brushType> {
	typedef baseIterator<brushType> base;
	constexpr rowIterator(const brushType& brush, const typename brushType::InputType& position) : base(brush, position) {}
	constexpr typename brushType::resultingType operator*() const {
		return base::brush.getValue(base::position);
	}
	//prefix increment, for when anything needs to be incremented without getting value
	constexpr auto& operator++() {
		base::position.x++;
		//do nothing, as position is a reference
		return *this;
	}
};

template<typename brush0Type>
struct transformBrushIterator : public rowIterator<transformBrush<brush0Type>> {
	typedef transformBrush<brush0Type> brushType;
	typedef rowIterator<brushType> base;
	const vec2 step;
	constexpr transformBrushIterator(const brushType& brush, cvec2& position) :
		base(brush, brush.modifiedTransform.multPointMatrix(position)),
		step(brush.modifiedTransform.getStep(axisID::x))
	{
	}
	//for when anything needs to be incremented without getting value
	constexpr transformBrushIterator& operator++() {
		base::position += step;
		return *this;
	}
	constexpr typename brush0Type::resultingType operator*() const {
		return base::brush.baseBrush.getValue((typename brush0Type::InputType)base::position);
	}
};

template<typename CombineBrushType, typename brush0Type, typename brush1Type>
struct combineBrushIterator {
	typedef CombineBrushType combineBrushType;
	const CombineBrushType& activeBrush;
	iteratorType<brush0Type> iterator0;
	iteratorType<brush1Type> iterator1;
	constexpr combineBrushIterator(const CombineBrushType& brush, const typename CombineBrushType::InputType& position) :
		activeBrush(brush),
		iterator0(makeRowIterator(brush.brush0, (typename brush0Type::InputType) position)),
		iterator1(makeRowIterator(brush.brush1, (typename brush1Type::InputType) position))
	{
	}
	constexpr auto& operator++() {
		++iterator0;
		++iterator1;
		return *this;
	}
	constexpr typename CombineBrushType::resultingType operator*() const {
		return activeBrush.combine(*iterator0, *iterator1);
	}
};

template<typename brush0Type, typename brush1Type, typename base = combineBrushIterator<colorMixer<brush0Type, brush1Type>, brush0Type, brush1Type>>
struct ColorMixerIterator :base {
	using base::base;
	constexpr typename base::combineBrushType::resultingType operator*() const {
		color topColor = *base::iterator0;

		if (topColor.a() == color::maxValue)
		{
			return topColor;
		}
		else
		{
			ccolor& bottomColor = *base::iterator1;
			if (topColor.a())
			{
				return transitionColor(topColor, bottomColor);
			}
			else
			{
				return bottomColor;
			}
		}
	}
};

template<typename brush0Type, typename brush1Type, typename base = combineBrushIterator<colorMultiplier<brush0Type, brush1Type>, brush0Type, brush1Type>>
struct ColorMultiplierIterator :base {
	using base::base;
	constexpr typename base::combineBrushType::resultingType operator*() const {
		color color0 = *base::iterator0;

		if (color0.a())
		{
			return base::activeBrush.combine(color0, *base::iterator1);
		}
		else
		{
			return color0;
		}
	}
};


template<>
struct rowIterator<solidColorBrush> {
	const solidColorBrush& brush;
	typedef baseIterator<solidColorBrush> base;
	constexpr rowIterator(const solidColorBrush& brush, cvec2&) :
		brush(brush) {
	}
	constexpr typename solidColorBrush::resultingType operator*() const {
		return brush.value;
	}
	constexpr auto& operator++() {
		//do nothing
		return *this;
	}
};

//function definitions
template<typename BrushType>
constexpr auto makeRowIterator(const BrushType& brush, const typename BrushType::InputType& pos) {
	return rowIterator<BrushType>(brush, pos);
}

template<typename brush0Type, typename brush1Type, typename BrushType = alphaMask<brush0Type, brush1Type>>
constexpr auto makeRowIterator(const alphaMask<brush0Type, brush1Type>& brush, const typename BrushType::InputType& pos) {
	return combineBrushIterator<BrushType, brush0Type, brush1Type>(brush, pos);
}
template<typename brush0Type, typename brush1Type, typename BrushType = colorMixer<brush0Type, brush1Type>>
constexpr auto makeRowIterator(const colorMixer<brush0Type, brush1Type>& brush, const typename BrushType::InputType& pos) {
	return ColorMixerIterator<brush0Type, brush1Type>(brush, pos);
}
template<typename brush0Type, typename brush1Type, typename BrushType = colorMultiplier<brush0Type, brush1Type>>
constexpr auto makeRowIterator(const colorMultiplier<brush0Type, brush1Type>& brush, const typename BrushType::InputType& pos) {
	return ColorMultiplierIterator<brush0Type, brush1Type>(brush, pos);
}
template<typename brush0Type, typename BrushType = transformBrush<brush0Type>>
constexpr auto makeRowIterator(const transformBrush<brush0Type>& brush, const typename BrushType::InputType& pos) requires(!std::is_same_v<brush0Type, squareInterpolator<true>>)
{
	return transformBrushIterator<brush0Type>(brush, pos);
}
template<>
constexpr auto makeRowIterator(const transformBrush<squareInterpolator<true>>& brush, cvec2& pos) {
	return bilinearIterator(brush.baseBrush, brush.modifiedTransform.multPointMatrix(pos), brush.modifiedTransform.getStep(axisID::x).x);
}

// fill a row of pixels with a brush
template <typename T, typename brush0Type>
inline void fillRowUnsafe(const array2d<T>& array, cfsize_t& rowY, cfsize_t& minX, cfsize_t& maxX, const brush0Type& b)
{
	typedef typename brush0Type::InputType vectorType;
	typedef typename vectorType::axisType axisType;
	const vectorType& pos = vectorType((axisType)minX, (axisType)rowY);
	//combinebrush<solidColorBrush, solidColorBrush> brus;
	//auto it1 = rowIterator<combinebrush<solidColorBrush, solidColorBrush>>(brus, pos);
	//auto it2 = rowIterator(brus, pos);
	auto it = makeRowIterator(b, pos);// rowIterator<std::remove_const_t< std::remove_reference_t<decltype(b)>>>(b, pos);

	T* const rowPtr = array.baseArray + rowY * array.size.x;
	T* const endPtr = rowPtr + maxX;

	for (T* ptr = rowPtr + minX; ptr < endPtr; ptr++, ++it)
	{
		*ptr = *it;
	}
}
//template <typename T, typename brush0Type>
//inline void fillRowUnsafe(const array2d<T>& array, cfsize_t& rowY, cfsize_t& minX, cfsize_t& maxX, const transformBrush<brush0Type>& b)
//{
//	T* const rowPtr = array.baseArray + rowY * array.size.x;
//	T* const endPtr = rowPtr + maxX;
//
//	vec2 pos = b.modifiedTransform.multPointMatrix(vec2((fp)minX, (fp)rowY));
//	cvec2& step = b.modifiedTransform.getStep(axisID::x);
//
//	for (T* ptr = rowPtr + minX; ptr < endPtr; ptr++, pos += step)
//	{
//		*ptr = b.baseBrush.getValue((typename brush0Type::InputType)pos);
//	}
//}

template <typename T, typename brush0Type, typename brush1Type>
inline void fillRowUnsafe(const array2d<T>& array, cfsize_t& rowY, cfsize_t& minX, cfsize_t& maxX, const colorMixer<brush0Type, brush1Type>& b)
	requires(std::is_base_of_v<array2d<T>, brush1Type >)
{
	if (&b.brush1 == &array)
	{
		T* const rowPtr = array.baseArray + rowY * array.size.x;
		T* const endPtr = rowPtr + maxX;

		typedef typename brush0Type::InputType vectorType;
		typedef typename vectorType::axisType axisType;

		vectorType pos = vectorType((axisType)minX, (axisType)rowY);

		for (T* ptr = rowPtr + minX; ptr < endPtr; ptr++, pos.x++)
		{
			ccolor& topColor = b.brush0.getValue(pos);
			*ptr = topColor.a() == color::maxValue ? topColor : topColor.a() ? transitionColor(topColor, *ptr)
				: *ptr;
		}
	}
	else
	{
		fillRowUnsafe(array, rowY, minX, maxX, b);
	}
}

template <typename T, typename brush0Type>
inline void fillRow(const array2d<T>& array, cint& rowY, cint& minX, cint& maxX, const brush0Type& b)
{
	if ((rowY >= 0) && (rowY < (int)array.size.y) && (maxX > 0))
	{
		fillRowUnsafe(array, rowY, (fsize_t)math::maximum(minX, 0), math::minimum((fsize_t)maxX, array.size.x), b);
	}
}

template <typename T, typename brush0Type>
inline void fillRow(const array2d<T>& array, cint& rowY, cfp& minX, cfp& maxX, const brush0Type& b)
{
	// ceil rule
	fillRow(array, rowY, math::maximum((int)math::ceil(minX), 0), math::minimum((int)math::ceil(maxX), (int)array.size.x), b);
}

