#pragma once
#include "textureParticle.h"
struct effectParticle : textureParticle {
	color effectColor;
	effectParticle(const color& effectColor = colorPalette::magenta) : effectColor(effectColor), textureParticle(particleID::effect) {}
	virtual void render(const gameRenderData& targetData) const override;
};