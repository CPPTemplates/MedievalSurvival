#pragma once
#include "math/uuid.h"
template<typename t>
struct SoundReference: t {
	uuid id = uuid();
	using t::t;
};