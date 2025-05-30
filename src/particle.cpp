#include "math/vector/vectn.h"
#include "particle.h"
#include "dimension.h"
#include "blockParticle.h"
#include "itemParticle.h"
#include "textureParticle.h"
#include "include/math/vector/vectorrandom.h"
particle::particle(const entityID& entityType) :entity(entityType)
{
}

particle::particle(const particleID& particleType): particle((entityID)((int)particleType + (int)entityID::particle))
{
	speed = getRandomVector(currentRandom, maxParticleSpeed);
}

void particle::tick()
{
	entity::tick();
	ticksToExist--;
	if (ticksToExist <= 0)
	{
		despawn = true;
	}
}

void particle::render(const gameRenderData& targetData) const
{

}

void particle::serializeMembers(nbtSerializer& s)
{
	s.serializeMembers(std::wstring(L"ticks to exist"), ticksToExist);
}

rectangle2 getParticleBrushRect(const resolutionTexture& tex)
{
	cfp texturePart = 0.2 * tex.defaultSize.x;
	cvec2 location = vec2(
		randFp(currentRandom, tex.defaultSize.x - texturePart),
		randFp(currentRandom, tex.defaultSize.x - texturePart)
	);
	return rectangle2(location, vec2(texturePart));
}