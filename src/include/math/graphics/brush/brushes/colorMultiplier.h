#pragma once
#include "math/graphics/brush/brush.h"
#include "math/graphics/brush/brushes/combineBrush.h"
#include "math/graphics/brush/brushes/solidColorBrush.h"
template<typename brush0Type, typename brush1Type>
struct colorMultiplier final : public combinebrush<brush0Type, brush1Type>
{
	typedef combinebrush<brush0Type, brush1Type> base;
	const brush0Type& brush0;
	const brush1Type& brush1;
	colorMultiplier(const brush0Type& baseBrush, const brush1Type& multColors) : brush0(baseBrush), brush1(multColors) {}
	constexpr color combine(const color& original, const color& multiplyWith) const {
		return color::multiplyColors<4>(original, multiplyWith);
	}
	constexpr color getValue(const typename base::inputType& pos) const
	{
		const color& original = brush0.getValue((typename brush0Type::inputType)pos);
		const color& multiplyWith = brush1.getValue((typename brush1Type::inputType)pos);

		return combine(original, multiplyWith);
	}
};

template<typename brush0Type, typename brush1Type, typename targetType>
inline void multiplyRectangle(crectangle2& rect, const brush0Type& baseBrush, const brush1Type& multiplyColorsWith, const targetType& renderTarget)
{
	const auto& multiplier = colorMultiplier<brush0Type, brush1Type>(baseBrush, multiplyColorsWith);
	fillRectangle(renderTarget, rect, multiplier);
}
template<typename brush0Type, typename targetType>
inline void multiplyRectangle(crectangle2& rect, const brush0Type& multiplyColorsWith, const targetType& renderTarget)
{
	multiplyRectangle(rect, renderTarget, multiplyColorsWith, renderTarget);
}

template<typename targetType>
inline void multiplyRectangle(crectangle2& rect, const color& multiplyColorsWith, const targetType& renderTarget)
{
	const solidColorBrush b = solidColorBrush(multiplyColorsWith);
	multiplyRectangle(rect, b, renderTarget);
}