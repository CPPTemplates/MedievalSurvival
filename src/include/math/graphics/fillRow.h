#pragma once
#include "brush/brushes/solidColorBrush.h"
#include "brush/brushes/colorMultiplier.h"
#include "brush/brushes/colorMixer.h"

//overloads
template<typename BrushType>
constexpr auto makeRowIterator(const BrushType& brush, const typename BrushType::inputType& pos);
template<typename brush0Type, typename brush1Type>
constexpr auto makeRowIterator(const colorMultiplier<brush0Type, brush1Type>& brush, const typename combinebrush<brush0Type, brush1Type>::inputType& pos);
template<typename brush0Type, typename brush1Type>
constexpr auto makeRowIterator(const colorMixer<brush0Type, brush1Type>& brush, const typename combinebrush<brush0Type, brush1Type>::inputType& pos);
template<typename brush0Type>
constexpr auto makeRowIterator(const transformBrush<brush0Type>& brush, const typename transformBrush<brush0Type>::inputType& pos);

template<typename brushType>
using iteratorType = decltype(makeRowIterator(std::declval<brushType>(), std::declval<typename brushType::inputType>()));

//iterators
template<typename brushType>
struct baseIterator {
	const brushType& brush;
	typename brushType::inputType position;
	constexpr baseIterator(const brushType& brush, typename brushType::inputType position) : brush(brush), position(position) {}
};

template<typename brush0Type>
struct rowIterator : public baseIterator<brush0Type> {
	typedef baseIterator<brush0Type> base;
	constexpr rowIterator(const brush0Type& brush, typename brush0Type::inputType position) : base(brush, position) {}
	constexpr typename brush0Type::resultingType operator*() {
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
	typedef rowIterator<transformBrush<brush0Type>> base;
	const vec2 step;
	constexpr transformBrushIterator(const transformBrush<brush0Type>& brush, cvec2& position) :
		base(brush, brush.modifiedTransform.multPointMatrix(position)),
		step(brush.modifiedTransform.getStep(axisID::x)) {
	}
	//for when anything needs to be incremented without getting value
	constexpr transformBrushIterator& operator++() {
		base::position += step;
		return *this;
	}
	constexpr typename brush0Type::resultingType operator*() {
		return base::brush.baseBrush.getValue((typename brush0Type::inputType)base::position);
	}
};

template<typename CombineBrushType, typename brush0Type, typename brush1Type>
struct combineBrushIterator {
	typedef CombineBrushType combineBrushType;
	const CombineBrushType& activeBrush;
	iteratorType<brush0Type> iterator0;
	iteratorType<brush1Type> iterator1;
	constexpr combineBrushIterator(const CombineBrushType& brush, const typename CombineBrushType::inputType& position) :
		activeBrush(brush),
		iterator0(makeRowIterator(brush.brush0, (typename brush0Type::inputType) position)),
		iterator1(makeRowIterator(brush.brush1, (typename brush1Type::inputType) position))
	{
	}
	constexpr auto& operator++() {
		++iterator0;
		++iterator1;
		return *this;
	}
	constexpr typename CombineBrushType::resultingType operator*() {
		return activeBrush.combine(*iterator0, *iterator1);
	}
};

template<typename brush0Type, typename brush1Type, typename base = combineBrushIterator<colorMixer<brush0Type, brush1Type>, brush0Type, brush1Type>>
struct ColorMixerIterator :base {
	using base::base;
	constexpr typename base::combineBrushType::resultingType operator*() {
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
	constexpr typename base::combineBrushType::resultingType operator*() {
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
constexpr auto makeRowIterator(const BrushType& brush, const typename BrushType::inputType& pos) {
	return rowIterator<BrushType>(brush, pos);
}

template<typename brush0Type, typename brush1Type>
constexpr auto makeRowIterator(const colorMixer<brush0Type, brush1Type>& brush, const typename combinebrush<brush0Type, brush1Type>::inputType& pos) {
	return ColorMixerIterator<brush0Type, brush1Type>(brush, pos);
}
template<typename brush0Type, typename brush1Type>
constexpr auto makeRowIterator(const colorMultiplier<brush0Type, brush1Type>& brush, const typename combinebrush<brush0Type, brush1Type>::inputType& pos) {
	return ColorMultiplierIterator<brush0Type, brush1Type>(brush, pos);
}
template<typename brush0Type>
constexpr auto makeRowIterator(const transformBrush<brush0Type>& brush, const typename transformBrush<brush0Type>::inputType& pos) {
	return transformBrushIterator<brush0Type>(brush, pos);
}


// fill a row of pixels with a brush
template <typename T, typename brush0Type>
inline void fillRowUnsafe(const array2d<T>& array, cfsize_t& rowY, cfsize_t& minX, cfsize_t& maxX, const brush0Type& b)
{
	typedef typename brush0Type::inputType vectorType;
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
//		*ptr = b.baseBrush.getValue((typename brush0Type::inputType)pos);
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

		typedef typename brush0Type::inputType vectorType;
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