#pragma once
#include "fourLeggedPassiveMob.h"

//pig
constexpr rectangle2 pigBodyTextureRect = crectangle2(46, 0, 8, 16);
constexpr rectangle2 pigFrontRightLegTextureRect = crectangle2(0, 6, 4, 6);
constexpr rectangle2 pigFrontLeftLegTextureRect = crectangle2(8, 6, 4, 6);
constexpr rectangle2 pigHeadTextureRect = crectangle2(0, 16, 8, 8);
constexpr fp pigHeadTextureOffset = 2;

constexpr vec2 pigHitboxSize = cvec2(0.9);

constexpr fp pigTextureScale = pigHitboxSize.getY() / (fp)(pigBodyTextureRect.size.getX() + pigFrontRightLegTextureRect.size.getY() + pigHeadTextureOffset);

constexpr fp pigHeadOffset = pigTextureScale * pigHeadTextureOffset;
constexpr fp pigLegOffset = pigTextureScale * 1;

constexpr vec2 pigBodySize = cvec2(pigBodyTextureRect.size.getY() * pigTextureScale, pigBodyTextureRect.size.getX() * pigTextureScale);
constexpr vec2 pigBodyRotationCenter = cvec2(pigBodySize.getX() * 0.5, 0);

constexpr vec2 pigLegSize = cvec2(pigFrontRightLegTextureRect.size.getX() * pigTextureScale, pigFrontRightLegTextureRect.size.getY() * pigTextureScale);
constexpr vec2 pigLegRotationCenter = cvec2(pigLegSize.getX() * 0.5, pigLegSize.getY());

constexpr vec2 pigFrontLegOffset = cvec2(pigBodySize.getX() * 0.5 - pigLegSize.getX() + pigLegRotationCenter.getX() - pigLegOffset, 0);
constexpr vec2 pigBackLegOffset = cvec2(-pigBodySize.getX() * 0.5 + pigLegRotationCenter.getX() - pigLegOffset, 0);
constexpr vec2 pigHeadSize = cvec2(pigHeadTextureRect.size.getX(), pigHeadTextureRect.size.getY()) * pigTextureScale;
constexpr vec2 pigHeadRotationCenter = cvec2(0, pigHeadSize.getY() * 0.5);


struct pig : public fourLeggedPassiveMob
{
	pig();
};