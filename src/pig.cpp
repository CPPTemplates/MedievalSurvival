#include "pig.h"
pig::pig() : mob(entityID::pig), fourLeggedPassiveMob(entityID::pig)
{

	//intialize body parts
	mainBodyPart = new bodyPart2D(pigBodyTextureRect, nullptr, vec2(position.x, position.y + pigLegSize.y), pigBodySize, pigBodyRotationCenter, 0, 90);

	frontRightLeg = new bodyPart2D(pigFrontRightLegTextureRect, mainBodyPart, pigFrontLegOffset, pigLegSize, pigLegRotationCenter);
	frontLeftLeg = new bodyPart2D(pigFrontLeftLegTextureRect, mainBodyPart, pigFrontLegOffset, pigLegSize, pigLegRotationCenter);

	backRightLeg = new bodyPart2D(pigFrontRightLegTextureRect, mainBodyPart, pigBackLegOffset, pigLegSize, pigLegRotationCenter);
	backLeftLeg = new bodyPart2D(pigFrontLeftLegTextureRect, mainBodyPart, pigBackLegOffset, pigLegSize, pigLegRotationCenter);

	head = new bodyPart2D(pigHeadTextureRect, mainBodyPart, vec2(pigBodySize.x - pigBodyRotationCenter.x - pigHeadOffset + pigHeadRotationCenter.x, pigBodySize.y + pigHeadOffset - pigHeadRotationCenter.y), pigHeadSize, pigHeadRotationCenter);
	connectBodyParts();
}