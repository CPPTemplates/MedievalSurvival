#pragma once
#include "include/math/graphics/brush/brush.h"
template <typename brush0Type, typename brush1Type, typename i = std::conditional_t<
	//if both input types are the same, we accept the shared type
	std::is_same_v<typename brush0Type::InputType, typename brush1Type::InputType>, typename brush0Type::InputType,
	//else, it's vec2 (one of both must be vec2)
	vec2>>
struct combinebrush : public brush<color, i>
{
	typedef i InputType;

};