#include "cod.h"
#include "panicAI.h"
#include "boidsAI.h"
#include "taskList.h"
void cod::updateBodyParts() const
{
	mainBodyPart->translate = position;
	mainBodyPart->changed = true;
}

cod::cod() : mob(entityID::cod)
{
	tasks = new taskList(this, {
		new panicAI(this),
		new boidsAI(this)
		});
	//intialize body parts
	mainBodyPart = new bodyPart2D(codBodyTextureRect, nullptr, vec2(position.x, position.y), codBodySize, codBodyRotationCenter);

	head = new bodyPart2D(codHeadTextureRect, mainBodyPart, vec2(codBodySize.x * 0.5, 0), codHeadSize, codHeadRotationCenter);

	bodyPart2D* nose = new bodyPart2D(codNoseTextureRect, head, cvec2(head->size.x, 0), codNoseSize, codNoseRotationCenter);
	bodyPart2D* dorsalFins = new bodyPart2D(codDorsalFinsTextureRect, mainBodyPart, cvec2(0, codBodySize.y * 0.5), codDorsalFinsSize, codDorsalFinsRotationCenter, 0, 0, true);
	bodyPart2D* caudalFin = new bodyPart2D(codCaudalFinTextureRect, mainBodyPart, cvec2(codBodySize.x * -0.5, 0), codCaudalFinSize, codCaudalFinRotationCenter, 0, 180, true);
	bodyPart2D* frontFin = new bodyPart2D(codFrontFinTextureRect, mainBodyPart, cvec2(codBodySize.x * 0.5, codBodySize.y * -0.5), codFrontFinSize, codFrontFinRotationCenter, 0, 90, true);


	head->children.push_back(nose);

	mainBodyPart->children.push_back(frontFin);
	mainBodyPart->children.push_back(dorsalFins);
	mainBodyPart->children.push_back(caudalFin);
	mainBodyPart->drawOrderIndex = mainBodyPart->children.size();
	mainBodyPart->children.push_back(head);
}

