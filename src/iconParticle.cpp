#include "iconParticle.h"

vec2 iconParticle::applyNaturalForces(cvec2& speed) const
{
	return speed;
}

fp iconParticle::getGravityForce() const
{
	return 0;
}
