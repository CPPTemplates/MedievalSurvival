#pragma once
#include "math/graphics/color/color.h"
#include "math/graphics/brush/brushes/combineBrush.h"
#include "math/graphics/color/colorFunctions.h"
template <typename brush0Type, typename brush1Type>
struct colorMixer final : public combinebrush<brush0Type, brush1Type>
{
	typedef brush1Type::inputType inputType;
	//top brush
	const brush0Type& brush0;
	//bottom brush
	const brush1Type& brush1;

	colorMixer(const brush0Type& topBrush, const brush1Type& bottomBrush) : brush0(topBrush), brush1(bottomBrush) {}

	// inline static constexpr color getColor(ccolor& topColor, ccolor& bottomColor)
	//{
	//	//static functions
	//	return (topColor.a() == color::maxValue) ?
	//		topColor :
	//		(topColor.a() ? color::transition(topColor, bottomColor) : bottomColor);
	// }
	constexpr color combine(const color& topColor, const color& bottomColor) const {
		if (topColor.a() == color::maxValue)
		{
			return topColor;
		}
		else if (topColor.a())
		{
			return transitionColor(topColor, bottomColor);
		}
		else
		{
			return bottomColor;
		}
	}

	inline color getValue(const inputType& pos) const
	{
		ccolor& topColor = brush0.getValue((typename brush0Type::inputType)pos);
		if (topColor.a() == color::maxValue)
		{
			return topColor;
		}
		else
		{
			ccolor& bottomColor = brush1.getValue(pos);
			if (topColor.a())
			{
				return transitionColor(topColor, bottomColor);
			}
			else
			{
				return bottomColor;
			}
		}

		// the bottom color will be optimized away if the topcolor does not have transparency
		// return getColor(topBrush.getValue(pos), bottomBrush.getValue(pos));
	}
};