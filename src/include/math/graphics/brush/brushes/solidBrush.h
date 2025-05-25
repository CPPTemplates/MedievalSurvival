#pragma once
#include "math/graphics/brush/brush.h"
template<typename resultingType, typename InputType>
struct solidBrush final :public brush<resultingType, InputType>
{
	resultingType value = resultingType();
	solidBrush(const resultingType& value) :value(value) {}
	constexpr resultingType getValue(const InputType&) const noexcept
	{
		return value;
	}
};