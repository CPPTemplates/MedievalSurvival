#pragma once
#include "math/graphics/brush/brush.h"
#include <math/graphics/color/colorFunctions.h>
struct squareInterpolator final : public brush<color, veci2>
{
	color cornerColors[4]{};
	inline color getValue(cveci2& pos) const
	{
		return interpolateColorBilinear(cornerColors, pos);
	}

	inline squareInterpolator()
	{

	}

	inline squareInterpolator(ccolor (&cornerColors)[4])
	{
		std::copy(&cornerColors[0], &cornerColors[4], this->cornerColors);
	}
};