#include "cow.h"
cow::cow(dimension *dimensionIn, cvec2 &position) : mob(dimensionIn, position, entityID::cow), fourLeggedPassiveMob(dimensionIn, position, entityID::cow)
{
	// intialize body parts
	mainBodyPart = new bodyPart2D(cowBodyTextureRect, nullptr, vec2(position.x, position.y + cowLegSize.y), cowBodySize, cowBodyRotationCenter, 0, 90);

	frontRightLeg = new bodyPart2D(cowFrontRightLegTextureRect, mainBodyPart, cowFrontLegOffset, cowLegSize, cowLegRotationCenter);
	frontLeftLeg = new bodyPart2D(cowFrontLeftLegTextureRect, mainBodyPart, cowFrontLegOffset, cowLegSize, cowLegRotationCenter);

	backRightLeg = new bodyPart2D(cowFrontRightLegTextureRect, mainBodyPart, cowBackLegOffset, cowLegSize, cowLegRotationCenter);
	backLeftLeg = new bodyPart2D(cowFrontLeftLegTextureRect, mainBodyPart, cowBackLegOffset, cowLegSize, cowLegRotationCenter);

	head = new bodyPart2D(cowHeadTextureRect, mainBodyPart, vec2(cowBodySize.x - cowBodyRotationCenter.x - cowHeadOffset + cowHeadRotationCenter.x, cowBodySize.y + cowHeadOffset - cowHeadRotationCenter.y), cowHeadSize, cowHeadRotationCenter);
	connectBodyParts();
}