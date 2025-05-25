#pragma once
#include "math/graphics/brush/brush.h"
template<typename brush0Type, typename brush1Type>
struct alphaMask final :public combinebrush<brush0Type, brush1Type>
{
	//alpha will be sampled from this brush
	const brush0Type& brush0 = nullptr;
	//rgb components will be sampled from this brush
	const brush1Type& brush1 = nullptr;

	alphaMask(const brush0Type& brush0, const brush1Type& brush1) :brush0(brush0), brush1(brush1) {}

	constexpr color combine(const color& brush0, const color& brush1) const {
		return color(brush1, brush0.a());
	}

	inline color getValue(cvec2& pos) const
	{
		const color c = brush1.getValue((typename brush0Type::InputType)pos);
		return color(c, brush0.getValue((typename brush1Type::InputType)pos).a());
	}

};