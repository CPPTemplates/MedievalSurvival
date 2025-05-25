#pragma once
#include "math/graphics/brush/brush.h"
#include <math/graphics/color/colorFunctions.h>
//expects requests in range [0, 1] on both axes when optimized, else in range [0, 255]
//set optimize to linear to false when the y step != 0 when filling a row of pixels
template<bool optimizeToLinear = true>
struct squareInterpolator final : public brush<color, vec2>
{
	//layout: 0 0, 1 0, 0 1, 1 1
	colorf cornerColors[4]{};
	inline color getValue(cvec2& pos) const
	{
		return interpolateColorBilinear(cornerColors, pos);
	}

	inline squareInterpolator()
	{

	}

	inline squareInterpolator(ccolorf (&cornerColors)[4])
	{
		std::copy(&cornerColors[0], &cornerColors[4], this->cornerColors);
	}
};