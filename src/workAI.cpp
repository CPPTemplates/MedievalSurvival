#include "workAI.h"
#include "Villager.h"
#include "tickableBlockContainer.h"

bool WorkAI::shouldExecute()
{
	Villager* connectedVillager = (Villager*)connectedEntity;
	if (isEmployed(connectedVillager->profession)) {
		return true;
	}
	//look for job site blocks
	connectedVillager->updateSelection();
	blockID selectedBlock = connectedVillager->selectedBlockContainer->getBlockID(connectedVillager->selectedBlockPosition);
	ProfessionID newProfession = getProfession(selectedBlock);
	if (newProfession) {
		connectedVillager->profession = newProfession;
	}
	return false;
}
